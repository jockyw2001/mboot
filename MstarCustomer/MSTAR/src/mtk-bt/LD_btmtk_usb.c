/*
*  Copyright (c) 2014 MediaTek Inc.
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License version 2 as
*  published by the Free Software Foundation.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*  GNU General Public License for more details.
*/

/*steve wang 2015/11/26*/
#include <mtk-bt/LD_usbbt.h>
#include <mtk-bt/LD_btmtk_usb.h>
#include <mtk-bt/errno.h>

/* Local Configuration ========================================================= */
#define LD_VERSION "1.0.0.0"
#define LD_BT_ROM_PATCH_FROM_BIN 1
#define BT_SEND_HCI_CMD_BEFORE_SUSPEND 1
#define FW_PATCH "mt7662t_patch_e1_hdr.bin"

#define BUFFER_SIZE  (1024*4)    /* Size of RX Queue */
#define LD_SUPPORT_FW_DUMP 0

/* ============================================================================= */
#define LD_BT_ALLOC_BUF 0
#define LD_NOT_FIX_BUILD_WARN 0

#if LD_BT_ROM_PATCH_FROM_BIN
static int LOAD_CODE_METHOD = BIN_FILE_METHOD;
static unsigned char *mt7662_rom_patch;
static unsigned char *mt7662t_rom_patch;
#else
static int LOAD_CODE_METHOD = HEADER_METHOD;
#include <mtk-bt/mt7662_rom_patch.h>
#include <mtk-bt/mt7662t_rom_patch.h>
#endif

static char driver_version[64] = { 0 };
static char rom_patch_version[64] = { 0 };

static unsigned char probe_counter = 0;

static volatile int metaMode;
static volatile int metaCount;

static mtkbt_dev_t *meta_udev = NULL;

struct LD_btmtk_usb_data *g_Ldata = NULL;

/* 0: False; 1: True */
static int isbtready;
static int isUsbDisconnet;
static volatile int is_assert = 0;

static inline int is_mt7630(struct LD_btmtk_usb_data *data)
{
    return ((data->chip_id & 0xffff0000) == 0x76300000);
}

static inline int is_mt7650(struct LD_btmtk_usb_data *data)
{
    return ((data->chip_id & 0xffff0000) == 0x76500000);
}

static inline int is_mt7632(struct LD_btmtk_usb_data *data)
{
    return ((data->chip_id & 0xffff0000) == 0x76320000);
}

static inline int is_mt7662(struct LD_btmtk_usb_data *data)
{
    return ((data->chip_id & 0xffff0000) == 0x76620000);
}

static inline int is_mt7662T(struct LD_btmtk_usb_data *data)
{
    return ((data->chip_id & 0xffffffff) == 0x76620100);
}

static inline int is_mt7632T(struct LD_btmtk_usb_data *data)
{
    return ((data->chip_id & 0xffffffff) == 0x76320100);
}

static int btmtk_usb_io_read32(struct LD_btmtk_usb_data *data, u32 reg, u32 *val)
{
    u8 request = data->r_request;
    int ret;

    ret = data->hcif->usb_control_msg(data->udev, MTKBT_CTRL_RX_EP, request, DEVICE_VENDOR_REQUEST_IN,
                  0x0, (u16)reg, data->io_buf, 4, CONTROL_TIMEOUT_JIFFIES);

    if (ret < 0)
    {
        *val = 0xffffffff;
        usb_debug("%s error(%d), reg=%x, value=%x\n", __func__, ret, reg, *val);
        return ret;
    }

    os_memmove(val, data->io_buf, 4);

    *val = le32_to_cpu(*val);

    if (ret > 0)
        ret = 0;

    return ret;
}

static int btmtk_usb_io_write32(struct LD_btmtk_usb_data *data, u32 reg, u32 val)
{
    u16 value, index;
    u8 request = data->w_request;
    mtkbt_dev_t *udev = data->udev;
    int ret;

    index = (u16) reg;
    value = val & 0x0000ffff;

    ret = data->hcif->usb_control_msg(udev, MTKBT_CTRL_TX_EP, request, DEVICE_VENDOR_REQUEST_OUT,
              value, index, NULL, 0, CONTROL_TIMEOUT_JIFFIES);

    if (ret < 0)
    {
        usb_debug("%s error(%d), reg=%x, value=%x\n", __func__, ret, reg, val);
        return ret;
    }

    index = (u16) (reg + 2);
    value = (val & 0xffff0000) >> 16;

    ret = data->hcif->usb_control_msg(udev, MTKBT_CTRL_TX_EP, request, DEVICE_VENDOR_REQUEST_OUT,
                  value, index, NULL, 0, CONTROL_TIMEOUT_JIFFIES);

    if (ret < 0)
    {
        usb_debug("%s error(%d), reg=%x, value=%x\n", __func__, ret, reg, val);
        return ret;
    }
    if (ret > 0)
    {
        ret = 0;
    }
    return ret;
}


static int btmtk_usb_send_hci_suspend_cmd(struct LD_btmtk_usb_data *data)
{
    int ret = 0;
    mtkbt_dev_t *udev = data->udev;

#if SUPPORT_HISENSE_WoBLE
    unsigned char buf[] = {0xc9, 0xfc, 0x02, 0x01, 0x0d};//for Hisense WoBLE
#else
    unsigned char buf[] = {0xc9, 0xfc, 0x0d, 0x01, 0x0e, 0x00, 0x05, 0x43, 0x52, 0x4b, 0x54, 0x4d, 0x20, 0x04, 0x32, 0x00};
#endif
    usb_debug("%s issue wake up command for '0E: MTK WoBLE Ver2'\n", __func__);
    ret = data->hcif->usb_control_msg(udev, MTKBT_CTRL_TX_EP,0x0,DEVICE_CLASS_REQUEST_OUT, 0x00, 0x00, buf, sizeof(buf), CONTROL_TIMEOUT_JIFFIES);
    if (ret < 0)
    {
        usb_debug("%s error1(%d)\n", __func__, ret);
        return ret;
    }
    usb_debug("%s : send suspend cmd OK\n", __func__);
    return 0;
}

static int btmtk_usb_send_hci_reset_cmd(struct LD_btmtk_usb_data *data)
{
    int retry_counter = 0;
    if (data == NULL)
        return -1;
    /* Send HCI Reset */
    {
        int ret = 0;
        unsigned char buf[4] = { 0 };
        buf[0] = 0x03;
        buf[1] = 0x0C;
        ret = data->hcif->usb_control_msg(data->udev, MTKBT_CTRL_TX_EP,0x0, DEVICE_CLASS_REQUEST_OUT,
                      0x00, 0x00, buf, 0x03, 100);

        if (ret < 0)
        {
            usb_debug("%s error1(%d)\n", __func__, ret);
            return ret;
        }
    }

    /* Get response of HCI reset */
    {
        while (1)
        {
            int ret = 0;
            unsigned char buf[LD_BT_MAX_EVENT_SIZE] = { 0 };
            int actual_length;
            ret = data->hcif->usb_interrupt_msg(data->udev, MTKBT_INTR_EP, buf, LD_BT_MAX_EVENT_SIZE,
                    &actual_length, 2000);

            if (ret < 0)
            {
                usb_debug("%s error2(%d)\n", __func__, ret);
                return ret;
            }

            if (actual_length == 6 &&
                buf[0] == 0x0e &&
                buf[1] == 0x04 &&
                buf[2] == 0x01 && buf[3] == 0x03 && buf[4] == 0x0c && buf[5] == 0x00)
            {
                break;
            }
            else
            {
                int i;
                usb_debug("%s drop unknown event :\n", __func__);
                for (i = 0; i < actual_length && i < 64; i++)
                {
                    usb_debug("%02X ", buf[i]);
                }
                usb_debug("\n");
                MTK_MDELAY(10);
                retry_counter++;
            }

            if (retry_counter > 10)
            {
                usb_debug("%s retry timeout!\n", __func__);
                return ret;
            }
        }
    }

    usb_debug("btmtk_usb_send_hci_reset_cmd : OK\n");
    return 0;
}

static int btmtk_usb_send_hci_set_ce_cmd(struct LD_btmtk_usb_data *data)
{
    unsigned char result_buf[LD_BT_MAX_EVENT_SIZE] = { 0 };
    usb_debug("send hci reset cmd");

    if (data == NULL)
        return -1;
    /* Send HCI Reset, read 0x41070c */
    {
        int ret = 0;
        unsigned char buf[8] = { 0xd1, 0xFC, 0x04, 0x0c, 0x07, 0x41, 0x00 };
        ret = data->hcif->usb_control_msg(data->udev, MTKBT_CTRL_TX_EP, 0x0, DEVICE_CLASS_REQUEST_OUT,
                      0x00, 0x00, buf, 0x07, 100);

        if (ret < 0)
        {
            usb_debug("%s error1(%d)\n", __func__, ret);
            return ret;
        }
    }

    /* Get response of HCI reset */
    {
        int ret = 0;
        int actual_length;
        ret = data->hcif->usb_interrupt_msg(data->udev,MTKBT_INTR_EP, result_buf, LD_BT_MAX_EVENT_SIZE,
                &actual_length, 2000);

        if (ret < 0)
        {
            usb_debug("%s error2(%d)\n", __func__, ret);
            return ret;
        }
        else
        {
            if (result_buf[6] & 0x01)
            {
                usb_debug("warning, 0x41070c[0] is 1!\n");
            }
        }
    }

    /* Send HCI Reset, write 0x41070c[0] to 1 */
    {
        int ret = 0;
        unsigned char buf[12] = { 0xd0, 0xfc, 0x08, 0x0c, 0x07, 0x41, 0x00 };
        buf[7] = result_buf[6] | 0x01;
        buf[8] = result_buf[7];
        buf[9] = result_buf[8];
        buf[10] = result_buf[9];
        ret = data->hcif->usb_control_msg(data->udev, MTKBT_CTRL_TX_EP, 0x0, DEVICE_CLASS_REQUEST_OUT,
                      0x00, 0x00, buf, 0x0b, 100);

        if (ret < 0)
        {
            usb_debug("%s error3(%d)\n", __func__, ret);
            return ret;
        }
    }

    /* Get response of HCI reset */
    {
        int ret = 0;
        int actual_length;
        memset(result_buf, 0, sizeof(result_buf));

        ret = data->hcif->usb_interrupt_msg(data->udev, MTKBT_INTR_EP, result_buf, LD_BT_MAX_EVENT_SIZE,
                &actual_length, 2000);

        if (ret < 0)
        {
            usb_debug("%s error4(%d)\n", __func__, ret);
            return ret;
        }
    }

    usb_debug("btmtk_usb_send_hci_set_ce_cmd : OK\n");
    return 0;
}

static int btmtk_usb_send_check_rom_patch_result_cmd(struct LD_btmtk_usb_data *data)
{
    /* Send HCI Reset */
    {
        int ret = 0;
        unsigned char buf[8] = { 0 };
        buf[0] = 0xD1;
        buf[1] = 0xFC;
        buf[2] = 0x04;
        buf[3] = 0x00;
        buf[4] = 0xE2;
        buf[5] = 0x40;
        buf[6] = 0x00;
        ret = data->hcif->usb_control_msg(data->udev, MTKBT_CTRL_TX_EP,0x0, DEVICE_CLASS_REQUEST_OUT,
                      0x00, 0x00, buf, 0x07, 100);
        if (ret < 0)
        {
            usb_debug("%s error1(%d)\n", __func__, ret);
            return ret;
        }
    }
    /* Get response of HCI reset */
    {
        int ret = 0;
        unsigned char buf[LD_BT_MAX_EVENT_SIZE] = { 0 };
        int actual_length = 0;
        ret = data->hcif->usb_interrupt_msg(data->udev, MTKBT_INTR_EP, buf, LD_BT_MAX_EVENT_SIZE,
                &actual_length, 2000);
        if (ret < 0)
        {
            usb_debug("%s error2(%d)\n", __func__, ret);
            return ret;
        }
        usb_debug("Check rom patch result : ");

        if (buf[6] == 0 && buf[7] == 0 && buf[8] == 0 && buf[9] == 0)
        {
            usb_debug("NG\n");
        }
        else
        {
            usb_debug("OK\n");
        }
    }
    return 0;
}

static int btmtk_usb_switch_iobase(struct LD_btmtk_usb_data *data, int base)
{
    int ret = 0;

    switch (base)
    {
        case SYSCTL:
            data->w_request = 0x42;
            data->r_request = 0x47;
            break;
        case WLAN:
            data->w_request = 0x02;
            data->r_request = 0x07;
            break;

        default:
            return -EINVAL;
    }

    return ret;
}

static void btmtk_usb_cap_init(struct LD_btmtk_usb_data *data)
{
    btmtk_usb_io_read32(data, 0x00, &data->chip_id);

    usb_debug("chip id = %x\n", data->chip_id);

    if (is_mt7630(data) || is_mt7650(data))
    {
        data->need_load_fw = 1;
        data->need_load_rom_patch = 0;
        data->fw_header_image = NULL;
        data->fw_bin_file_name = (unsigned char*)strdup("mtk/mt7650.bin");
        data->fw_len = 0;
    }
    else if (is_mt7662T(data) || is_mt7632T(data))
    {
        usb_debug("btmtk:This is 7662T chip\n");
        data->need_load_fw = 0;
        data->need_load_rom_patch = 1;
        if (LOAD_CODE_METHOD == HEADER_METHOD)
        {
            data->rom_patch_header_image = mt7662t_rom_patch;
            data->rom_patch_len = sizeof(mt7662t_rom_patch);
            data->rom_patch_offset = 0xBC000;
        }
        else
        {
            data->rom_patch_bin_file_name = os_kmalloc(32, MTK_GFP_ATOMIC);
            if (!data->rom_patch_bin_file_name)
            {
                usb_debug("%s: Can't allocate memory (32)\n", __FUNCTION__);
                return;
            }
            os_memset(data->rom_patch_bin_file_name, 0, 32);

            if ( (data->chip_id & 0xf) < 0x2 )
            {
                os_memcpy(data->rom_patch_bin_file_name, "mt7662t_patch_e1_hdr.bin", 24);
            }
            else
            {
                os_memcpy(data->rom_patch_bin_file_name, "mt7662t_patch_e3_hdr.bin", 24);
            }
            data->rom_patch_offset = 0xBC000;
            data->rom_patch_len = 0;
        }
    }
    else if (is_mt7632(data) || is_mt7662(data))
    {
        usb_debug("btmtk:This is 7662 chip\n");
        data->need_load_fw = 0;
        data->need_load_rom_patch = 1;
        if (LOAD_CODE_METHOD == HEADER_METHOD)
        {
            data->rom_patch_header_image = mt7662_rom_patch;
            data->rom_patch_len = sizeof(mt7662_rom_patch);
            data->rom_patch_offset = 0x90000;
        }
        else
        {
            data->rom_patch_bin_file_name = os_kmalloc(32,MTK_GFP_ATOMIC);
            if (!data->rom_patch_bin_file_name)
            {
                usb_debug("btmtk_usb_cap_init(): Can't allocate memory (32)\n");
                return;
            }
            os_memset(data->rom_patch_bin_file_name, 0, 32);

            if ((data->chip_id & 0xf) < 0x2)
            {
                usb_debug("chip_id < 0x2\n");
                os_memcpy(data->rom_patch_bin_file_name, FW_PATCH, 23);
                /* memcpy(data->rom_patch_bin_file_name, "mt7662_patch_e1_hdr.bin", 23); */
            }
            else
            {
                usb_debug("chip_id >= 0x2\n");
                usb_debug("patch bin file name:%s\n", FW_PATCH);
                os_memcpy(data->rom_patch_bin_file_name, FW_PATCH, 23);
            }

            data->rom_patch_offset = 0x90000;
            data->rom_patch_len = 0;
        }
    }
    else
    {
        usb_debug("unknow chip(%x)\n", data->chip_id);
    }
}

#if CRC_CHECK
static u16 checksume16(u8 *pData, int len)
{
    int sum = 0;

    while (len > 1)
    {
        sum += *((u16 *) pData);

        pData = pData + 2;

        if (sum & 0x80000000)
        {
            sum = (sum & 0xFFFF) + (sum >> 16);
        }
        len -= 2;
    }

    if (len)
        sum += *((u8 *) pData);

    while (sum >> 16)
    {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return ~sum;
}

static int btmtk_usb_chk_crc(struct LD_btmtk_usb_data *data, u32 checksum_len)
{
    int ret = 0;
    mtkbt_dev_t *udev = data->udev;

    usb_debug("%s", __func__);

    os_memmove(data->io_buf, &data->rom_patch_offset, 4);
    os_memmove(&data->io_buf[4], &checksum_len, 4);

    ret = data->hcif->usb_control_msg(udev, MTKBT_CTRL_TX_EP,0x1, DEVICE_VENDOR_REQUEST_OUT,
                  0x20, 0x00, data->io_buf, 8, CONTROL_TIMEOUT_JIFFIES);

    if (ret < 0)
    {
        usb_debug("%s error(%d)\n", __func__, ret);
    }

    return ret;
}

static u16 btmtk_usb_get_crc(struct LD_btmtk_usb_data *data)
{
    int ret = 0;
    mtkbt_dev_t *udev = data->udev;
    u16 crc, count = 0;

    usb_debug("%s", __func__);

    while (1)
    {
        ret =
            data->hcif->usb_control_msg(udev, MTKBT_CTRL_RX_EP, 0x01, DEVICE_VENDOR_REQUEST_IN,
                    0x21, 0x00, data->io_buf, 2, CONTROL_TIMEOUT_JIFFIES);

        if (ret < 0)
        {
            crc = 0xFFFF;
            usb_debug("%s error(%d)\n", __func__, ret);
        }

        os_memmove(&crc, data->io_buf, 2);

        crc = le16_to_cpu(crc);

        if (crc != 0xFFFF)
            break;

        MTK_MDELAY(100);

        if (count++ > 100)
        {
            usb_debug("Query CRC over %d times\n", count);
            break;
        }
    }

    return crc;
}
#endif

static int btmtk_usb_reset_wmt(struct LD_btmtk_usb_data *data)
{
    int ret = 0;

    /* reset command */
    u8 cmd[9] = { 0x6F, 0xFC, 0x05, 0x01, 0x07, 0x01, 0x00, 0x04 };

    os_memmove(data->io_buf, cmd, 8);


    ret = data->hcif->usb_control_msg(data->udev, MTKBT_CTRL_TX_EP, 0x01,
                  DEVICE_CLASS_REQUEST_OUT, 0x30, 0x00, data->io_buf, 8,
                  CONTROL_TIMEOUT_JIFFIES);

    if (ret < 0)
    {
        usb_debug("%s:Err1(%d)\n", __func__, ret);
        return ret;
    }

    MTK_MDELAY(20);

    ret = data->hcif->usb_control_msg(data->udev, MTKBT_CTRL_RX_EP, 0x01,
                  DEVICE_VENDOR_REQUEST_IN, 0x30, 0x00, data->io_buf, 7,
                  CONTROL_TIMEOUT_JIFFIES);

    if (ret < 0)
    {
        usb_debug("%s Err2(%d)\n", __func__, ret);
    }

    if (data->io_buf[0] == 0xe4 &&
        data->io_buf[1] == 0x05 &&
        data->io_buf[2] == 0x02 &&
        data->io_buf[3] == 0x07 &&
        data->io_buf[4] == 0x01 &&
        data->io_buf[5] == 0x00 &&
        data->io_buf[6] == 0x00)
    {
        usb_debug("Check reset wmt result : OK\n");
    }
    else
    {
        usb_debug("Check reset wmt result : 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",data->io_buf[0],
        data->io_buf[1],data->io_buf[2],data->io_buf[3],data->io_buf[4],data->io_buf[5],data->io_buf[6]);
        usb_debug("Check reset wmt result : NG\n");
    }


    return ret;
}

static u16 btmtk_usb_get_rom_patch_result(struct LD_btmtk_usb_data *data)
{
    int ret = 0;

    ret = data->hcif->usb_control_msg(data->udev, MTKBT_CTRL_RX_EP, 0x01,
                  DEVICE_VENDOR_REQUEST_IN, 0x30, 0x00, data->io_buf, 7,
                  CONTROL_TIMEOUT_JIFFIES);

    if (ret < 0)
    {
        usb_debug("%s error(%d)\n", __func__, ret);
    }

    if (data->io_buf[0] == 0xe4 &&
        data->io_buf[1] == 0x05 &&
        data->io_buf[2] == 0x02 &&
        data->io_buf[3] == 0x01 &&
        data->io_buf[4] == 0x01 &&
        data->io_buf[5] == 0x00 &&
        data->io_buf[6] == 0x00)
    {
        usb_debug("Get rom patch result : OK\n");
    }
    else
    {
        usb_debug("Get rom patch result : NG\n");
    }
    return ret;
}

static int btmtk_usb_load_rom_patch(struct LD_btmtk_usb_data *data)
{
    u32 loop = 0;
    u32 value;
    s32 sent_len;
    int ret = 0;
    u32 patch_len = 0;
    u32 cur_len = 0;
    int real_len = 0;
    int first_block = 1;
    unsigned char phase;
    void *buf;
    char *pos;
    unsigned char *tmp_str;

    usb_debug("btmtk_usb_load_rom_patch begin\n");
 load_patch_protect:
    btmtk_usb_switch_iobase(data, WLAN);
    btmtk_usb_io_read32(data, SEMAPHORE_03, &value);
    loop++;

    if ((value & 0x01) == 0x00)
    {
        if (loop < 1000)
        {
            MTK_MDELAY(1);
            goto load_patch_protect;
        }
        else
        {
            usb_debug("btmtk_usb_load_rom_patch ERR! Can't get semaphore! Continue\n");
        }
    }

    btmtk_usb_switch_iobase(data, SYSCTL);

    btmtk_usb_io_write32(data, 0x1c, 0x30);

    btmtk_usb_switch_iobase(data, WLAN);

    /* check ROM patch if upgrade */
    if ((MT_REV_GTE(data, mt7662, REV_MT76x2E3)) || (MT_REV_GTE(data, mt7632, REV_MT76x2E3)))
    {
        btmtk_usb_io_read32(data, CLOCK_CTL, &value);
        if ((value & 0x01) == 0x01)
        {
            usb_debug("btmtk_usb_load_rom_patch : no need to load rom patch\n");
            btmtk_usb_send_hci_reset_cmd(data);
            goto error0;
        }
    }
    else
    {
        btmtk_usb_io_read32(data, COM_REG0, &value);
        if ((value & 0x02) == 0x02)
        {
            usb_debug("btmtk_usb_load_rom_patch : no need to load rom patch\n");
            btmtk_usb_send_hci_reset_cmd(data);
            goto error0;
        }
    }

    buf = os_kmalloc(UPLOAD_PATCH_UNIT,MTK_GFP_ATOMIC);

    if (!buf)
    {
        ret = -ENOMEM;
        goto error1;
    }

    pos = buf;

    if (LOAD_CODE_METHOD == BIN_FILE_METHOD)
    {
        LD_load_code_from_bin(&data->rom_patch, (char*)data->rom_patch_bin_file_name, data->udev, &data->rom_patch_len);
        usb_debug("LOAD_CODE_METHOD : BIN_FILE_METHOD\n");
    }
    else
    {
        data->rom_patch = data->rom_patch_header_image;
        usb_debug("LOAD_CODE_METHOD : HEADER_METHOD\n");
    }

    if (!data->rom_patch)
    {
        if (LOAD_CODE_METHOD == BIN_FILE_METHOD)
        {
            usb_debug
                ("%s:please assign a rom patch(/etc/firmware/%s)or(/lib/firmware/%s)\n",
                 __func__, data->rom_patch_bin_file_name,
                 data->rom_patch_bin_file_name);
        }
        else
        {
            usb_debug("%s:please assign a rom patch\n", __func__);
        }

        ret = -1;
        goto error2;
    }

    tmp_str = data->rom_patch;
    usb_debug("FW Version = %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
           tmp_str[0], tmp_str[1], tmp_str[2], tmp_str[3],
           tmp_str[4], tmp_str[5], tmp_str[6], tmp_str[7],
           tmp_str[8], tmp_str[9], tmp_str[10], tmp_str[11],
           tmp_str[12], tmp_str[13], tmp_str[14], tmp_str[15]);

    usb_debug("build Time = %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
           tmp_str[0], tmp_str[1], tmp_str[2], tmp_str[3],
           tmp_str[4], tmp_str[5], tmp_str[6], tmp_str[7],
           tmp_str[8], tmp_str[9], tmp_str[10], tmp_str[11],
           tmp_str[12], tmp_str[13], tmp_str[14], tmp_str[15]);

    os_memset(rom_patch_version, 0, sizeof(rom_patch_version));
    os_memcpy(rom_patch_version, tmp_str, 15);

    tmp_str = data->rom_patch + 16;
    usb_debug("platform = %c%c%c%c\n", tmp_str[0], tmp_str[1], tmp_str[2], tmp_str[3]);


    tmp_str = data->rom_patch + 20;
    usb_debug("HW/SW version = %c%c%c%c\n", tmp_str[0], tmp_str[1], tmp_str[2], tmp_str[3]);

    tmp_str = data->rom_patch + 24;
    usb_debug("Patch version = %c%c%c%c\n", tmp_str[0], tmp_str[1], tmp_str[2], tmp_str[3]);


    usb_debug("\nloading rom patch...\n");

    cur_len = 0x00;
    patch_len = data->rom_patch_len - PATCH_INFO_SIZE;

    /* loading rom patch */
    while (1)
    {
        s32 sent_len_max = UPLOAD_PATCH_UNIT - PATCH_HEADER_SIZE;
        real_len = 0;
        sent_len =
            (patch_len - cur_len) >= sent_len_max ? sent_len_max : (patch_len - cur_len);

        usb_debug("patch_len = %d\n", patch_len);
        usb_debug("cur_len = %d\n", cur_len);
        usb_debug("sent_len = %d\n", sent_len);

        if (sent_len > 0)
        {
            if (first_block == 1)
            {
                if (sent_len < sent_len_max)
                    phase = PATCH_PHASE3;
                else
                    phase = PATCH_PHASE1;
                first_block = 0;
            }
            else if (sent_len == sent_len_max)
            {
                phase = PATCH_PHASE2;
            }
            else
            {
                phase = PATCH_PHASE3;
            }

            /* prepare HCI header */
            pos[0] = 0x6F;
            pos[1] = 0xFC;
            pos[2] = (sent_len + 5) & 0xFF;
            pos[3] = ((sent_len + 5) >> 8) & 0xFF;

            /* prepare WMT header */
            pos[4] = 0x01;
            pos[5] = 0x01;
            pos[6] = (sent_len + 1) & 0xFF;
            pos[7] = ((sent_len + 1) >> 8) & 0xFF;

            pos[8] = phase;

            os_memcpy(&pos[9], data->rom_patch + PATCH_INFO_SIZE + cur_len, sent_len);

            usb_debug("sent_len + PATCH_HEADER_SIZE = %d, phase = %d\n",
                   sent_len + PATCH_HEADER_SIZE, phase);

            ret = data->hcif->usb_bulk_msg(data->udev, MTKBT_BULK_TX_EP, buf, sent_len + PATCH_HEADER_SIZE, &real_len, 0);

            if (ret)
            {
                usb_debug("upload rom_patch err: %d\n", ret);
                goto error2;
            }

            MTK_MDELAY(1);

            cur_len += sent_len;

        }
        else
        {
            break;
        }
    }

    MTK_MDELAY(20);
    ret = btmtk_usb_get_rom_patch_result(data);
    MTK_MDELAY(20);

    /* Send Checksum request */
    #if CRC_CHECK
    int total_checksum = checksume16(data->rom_patch + PATCH_INFO_SIZE, patch_len);
    btmtk_usb_chk_crc(data, patch_len);
    MTK_MDELAY(20);
    if (total_checksum != btmtk_usb_get_crc(data))
    {
        usb_debug("checksum fail!, local(0x%x) <> fw(0x%x)\n", total_checksum,
               btmtk_usb_get_crc(data));
        ret = -1;
        goto error2;
    }
    else
    {
        usb_debug("%s crc match!\n", __func__);
    }
    #endif
    MTK_MDELAY(20);
    /* send check rom patch result request */
    btmtk_usb_send_check_rom_patch_result_cmd(data);
    MTK_MDELAY(20);
    /* CHIP_RESET */
    ret = btmtk_usb_reset_wmt(data);
    MTK_MDELAY(20);
    /* BT_RESET */
    btmtk_usb_send_hci_reset_cmd(data);
    /* for WoBLE/WoW low power */
    btmtk_usb_send_hci_set_ce_cmd(data);

 error2:
 error1:
 error0:
    btmtk_usb_io_write32(data, SEMAPHORE_03, 0x1);
    usb_debug("btmtk_usb_load_rom_patch end\n");
    return ret;
}

void LD_btmtk_usb_SetWoble(mtkbt_dev_t *dev)
{
    struct LD_btmtk_usb_data *data = BT_INST(dev)->priv_data;

    usb_debug("%s\n", __func__);

    if (!data)
    {
        usb_debug("%s,btmtk data NULL!\n", __func__);
        return;
    }

    btmtk_usb_send_hci_suspend_cmd(data);

    return ;
}

int LD_btmtk_usb_probe(mtkbt_dev_t *dev)
{
    struct LD_btmtk_usb_data *data;
    int  err = 0;
    //int i;
    //mtkbt_dev_t *usb_dev = dev;

    usb_debug("===========================================\n");
    usb_debug("Mediatek Bluetooth USB driver ver %s\n", LD_VERSION);
    usb_debug("===========================================\n");
    memset(driver_version, 0, sizeof(driver_version));
    memcpy(driver_version, LD_VERSION, sizeof(LD_VERSION));
    probe_counter++;
    isbtready = 0;
    is_assert = 0;
    usb_debug("probe_counter = %d\n", probe_counter);

    usb_debug("LD_btmtk_usb_probe begin\n");

    data = os_kmalloc(sizeof(*data),MTK_GFP_ATOMIC);

    if (!data)
    {
        usb_debug("[ERR] kzalloc failed !\n");
        usb_debug("LD_btmtk_usb_probe end Error 2\n");
        return -ENOMEM;
    }

    data->hcif = BT_INST(dev)->hci_if;

    data->cmdreq_type = USB_TYPE_CLASS;

    data->udev = dev;
    meta_udev = data->udev;

    data->meta_tx = 0;

    data->io_buf = os_kmalloc(LD_BT_MAX_EVENT_SIZE,MTK_GFP_ATOMIC);

    btmtk_usb_switch_iobase(data, WLAN);

    /* clayton: according to the chip id, load f/w or rom patch */
    btmtk_usb_cap_init(data);

    if (data->need_load_rom_patch)
    {
        err = btmtk_usb_load_rom_patch(data);
        //btmtk_usb_send_hci_suspend_cmd(data);
        if (err < 0)
        {
            if(data->io_buf) os_kfree(data->io_buf);
            os_kfree(data);
            usb_debug("btmtk_usb_probe end Error 4\n");
            return err;
        }
    }

    if (data->rom_patch_bin_file_name)
        os_kfree(data->rom_patch_bin_file_name);


    isUsbDisconnet = 0;
    BT_INST(dev)->priv_data = data;
    g_Ldata = data;
    isbtready = 1;

    usb_debug("btmtk_usb_probe end\n");
    return 0;
}

void LD_btmtk_usb_disconnect(mtkbt_dev_t *dev)
{
    struct LD_btmtk_usb_data *data = BT_INST(dev)->priv_data;
    //struct hci_dev *hdev;

    usb_debug("%s\n", __func__);

    if (!data)
        return;

    isbtready = 0;
    metaCount = 0;

    if (LOAD_CODE_METHOD == BIN_FILE_METHOD)
    {
        if (data->need_load_rom_patch)
            os_kfree(data->rom_patch);

        if (data->need_load_fw)
            os_kfree(data->fw_image);
    }

    usb_debug("unregister bt irq\n");

    isUsbDisconnet = 1;
    usb_debug("btmtk: stop all URB\n");
    os_kfree(data->io_buf);
    os_kfree(data);
    g_Ldata = NULL;
}