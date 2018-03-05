//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>

#ifndef __DRV_URSA_6M60_C__
#define __DRV_URSA_6M60_C__

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include "ursa_6m60.h"

#define UART_CMD_BUF_SIZE    11
#define IS_URAT_SUCCESS_MSG(x)          (((x) & 0xF0) == 0xA0)
#define IS_URAT_SUCCESS_DATA_MSG(x)     ((((x) & 0xF0) == 0xA0) && (((x) & 0x0F) > 2))
#define IS_URAT_ERROR_MSG(x)            (((x) & 0xF0) == 0xE0)
#define IS_VALID_MSG(x)                 ((IS_URAT_SUCCESS_MSG(x) || IS_URAT_ERROR_MSG(x)) && (GET_URAT_MSG_LEN(x) >= 2))
#define GET_URAT_MSG_LEN(x)             ((x) & 0x0F)
#define URSA_CMD_INTERVAL_TIME   26
#define MFC_CMD_DATA_SUM      7

MS_U32  g_uart_fd = 0;
MS_U32 u32Count = 3;
MS_U8* m_data = NULL;
MS_U8 g_uart_init_status = 0;
MS_U8 uart_rx_buf[512];
static MS_U32 Data_count = 0;
static MS_U32 Data_Len = 0;

void RxHandler(int c)
{
    MS_BOOL isValidMsg = FALSE;
    MS_BOOL bRet = FALSE;
    MS_U32 bLen = 0;

    isValidMsg = IS_VALID_MSG(c);
    if((Data_Len != 0) && (Data_count != 0))
    {
        UBOOT_DEBUG("Date Value: 0x%x\n", c);
        //UBOOT_DEBUG("date IN1: Data_Len:%d, Data_count:%d\n", Data_Len,Data_count);
        *(m_data + (Data_Len - Data_count)) = c;
        Data_count--;
        //UBOOT_DEBUG("date IN2: Data_Len:%d, Data_count:%d\n", Data_Len,Data_count);
    }
    else if(isValidMsg == TRUE)
    {
        bLen = GET_URAT_MSG_LEN(c);
        UBOOT_DEBUG("bLen: %x\n",bLen);
        if(m_data != NULL)
        {
            free(m_data);
        }
        m_data = (MS_U8 *)malloc(bLen * sizeof(MS_U8));
        memset(m_data, 0, bLen * sizeof(MS_U8));
        Data_count = bLen -1;
        Data_Len = bLen -1;
        //*(m_data + (Data_count - Data_count)) = c;
        //Data_count--;
    }
}

MS_BOOL InitUrsaCmdData(void)
{
    int ret = -1;
    if(g_uart_init_status)
    {
        UBOOT_DEBUG("InitUrsaCmdData Init Once!\n");
        return FALSE;
    }

    MDrv_UART_Init(E_UART_PIU_UART1,115200);

    MDrv_UART_SetIOMapBase();
    //Whaley Board use RX2/TX2 Uart.
    //MStar 095D board init at mboot/.../mhal_sys.c   use RX5/TX5
    //6M60 uart Link to Soft E_UART_PIU_UART1
#if 0
    //Whaley Board Need go to this flow
    ret = mdrv_uart_connect(E_UART_PORT2, E_UART_PIU_UART1);
    if (ret < 0)
    {
        UBOOT_ERROR("device_ursa:: E_UART_PORT2, UARTInit control interface Fail\n");
        return FALSE;
    }
#endif
    g_uart_fd = mdrv_uart_open(E_UART_PIU_UART1);
    if (g_uart_fd)
    {
        //mdrv_uart_set_baudrate(g_uart_fd ,115200);
        //mdrv_uart_rx_enable(g_uart_fd); //Open rx receive only when sending cmd.
        memset(uart_rx_buf, 0, sizeof(uart_rx_buf));
        if(mdrv_uart_set_rx_buffer(g_uart_fd, uart_rx_buf,sizeof(uart_rx_buf)) != 0)
        {
                UBOOT_ERROR("mdrv_uart_set_rx_buffer failed\n");
        }
        mdrv_uart_set_rx_callback(g_uart_fd, RxHandler);
        UBOOT_DEBUG("uartcomm init uart fd:%d ok\n", g_uart_fd);
    }
    else
    {
        UBOOT_ERROR("uartcomm init failed !\n");
        return FALSE;
    }
    g_uart_init_status = 1;
    return TRUE;
}

MS_U32 Send(MS_U8* cValue, const MS_U32 u32Length)
{
    int length = 0;
    if (g_uart_fd  == 0)
    {
        UBOOT_ERROR("Please connect before send command\n");
        return -1;
    }

    length = mdrv_uart_write(g_uart_fd, cValue, u32Length);
    return length;
}

MS_U32 Read(MS_U8* cValue, const MS_U32 u32Length)
{
    int length = 0;
    if (g_uart_fd == 0)
    {
        UBOOT_ERROR("Please connect before read command\n");
        return -1;
    }

    length = mdrv_uart_read(g_uart_fd, cValue, u32Length);
    return length;
}

void combineDDC2UART(MS_U8 *u8UartBuf, MS_U8 *u8DDCBuf)
{
    u8UartBuf[0]= 0xE0;
    u8UartBuf[1]= UART_CMD_BUF_SIZE; //Length 11
    u8UartBuf[2]= 0x40; //separate-tv ,Basic DDC CMD
    u8UartBuf[3] = u8DDCBuf[0];
    u8UartBuf[4] = u8DDCBuf[1];
    u8UartBuf[5] = u8DDCBuf[2];
    u8UartBuf[6] = u8DDCBuf[3];
    u8UartBuf[7] = u8DDCBuf[4];
    u8UartBuf[8] = u8DDCBuf[5];
    u8UartBuf[9] = u8DDCBuf[6];
    u8UartBuf[10] = 0x0;

    calUartCheckSum(u8UartBuf,UART_CMD_BUF_SIZE);
}

void calUartCheckSum(MS_U8 *pBuf, MS_U8 u8BufLen )
{
    MS_U16 u16CheckSum =0;
    MS_U8 bufSize = u8BufLen;
    u8BufLen--;
    while(u8BufLen > 0)
    {
        -- u8BufLen;
        u16CheckSum += pBuf[u8BufLen] & 0xFF;
    }
    u16CheckSum &= 0xFF;
    pBuf[bufSize-1] = 0xFF - u16CheckSum;
}

MS_BOOL checkUartCheckSum(MS_U8 *pBuf, MS_U8 u8BufLen )
{
    MS_U16 u16CheckSum =0;
    MS_U8 tLen = u8BufLen;
    u8BufLen--;

    while(u8BufLen > 0)
    {
        --u8BufLen;
        u16CheckSum += pBuf[u8BufLen] & 0xFF;
    }
    u16CheckSum &= 0x00FF;
    u16CheckSum = (0xFF - u16CheckSum);
    if(pBuf[tLen-1] == u16CheckSum)
    {
        return TRUE;
    }
    UBOOT_ERROR("Uart CheckSum is incorrect, %x!=%x\n",u16CheckSum,pBuf[tLen-1]);
    return FALSE;
}

void CalCheckSum(MS_U8 *pBuf, MS_U8 u8BufLen)
{
    MS_U8 u8CheckSum =0;

    while( u8BufLen > 1 )//calculate pBuf[1]~pBuf[End] do not count pBuf[0],it is checksum
    {
        -- u8BufLen;
        u8CheckSum += pBuf[u8BufLen] & 0x0F;
    }
    u8CheckSum &= 0x0F;
    pBuf[0] &= 0x0F;
    pBuf[0] |= ((u8CheckSum )<<4);
}

MS_BOOL Cmd_DataWrite(MS_U8 *u8Buf)
{
        MS_U8 u8DataBuf[UART_CMD_BUF_SIZE] = {0};
        int bLen= 0;
        MS_U32 timeout1 = 0;
        MS_U32 timeout2 = 0;
        MS_BOOL ret = FALSE;

        combineDDC2UART(u8DataBuf,u8Buf);
        UBOOT_DEBUG(">>CMD:%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\n",u8DataBuf[0], u8DataBuf[1],u8DataBuf[2],u8DataBuf[3],u8DataBuf[4],u8DataBuf[5],u8DataBuf[6],
    u8DataBuf[7] ,u8DataBuf[8],u8DataBuf[9],u8DataBuf[10]);
		mdrv_uart_rx_enable(g_uart_fd);//Enable RX Call_back When Send CMD
        do
       {
            bLen = Send(u8DataBuf,UART_CMD_BUF_SIZE);

            if(bLen == UART_CMD_BUF_SIZE)
            {
                ret = TRUE;
                timeout1 = 0;
                UBOOT_DEBUG("Send success,receiving....\n");

                //don't need readMsgAndWaitCount, using callback To Read MSG
                do
                {
                     //Waiting for RX Call back Get Datalen
                     MsOS_DelayTask(1);
                     //UBOOT_DEBUG("Send success,receiving Fail retry: %d\n", timeout1);
                     //UBOOT_DEBUG("date IN: Data_Len:%d, Data_count:%d\n", Data_Len,Data_count);
                }
                while((Data_Len == 0 || Data_count != 0) && (timeout1 ++ <u32Count));
                if(Data_Len != 0)
                {
                    UBOOT_DEBUG("Send success,received  Date len:%d \n", Data_Len);
                    Data_Len = 0;
                }
                else
                {
                    UBOOT_ERROR("Send success,received  Failed\n");
                    ret = FALSE;
                }
            }
            else
            {
                UBOOT_ERROR("Send fail, please check!!!\n");
                ret = FALSE;
            }
            if(ret == FALSE)
            {
                UBOOT_ERROR("retry %d....\n",timeout2+1);
            }
        }
        while((ret != TRUE) && (timeout2++<u32Count));
        mdrv_uart_rx_disable(g_uart_fd);//Disable RX Call_back When Get MSG Done
        return ret;
}

MS_BOOL Cmd_DataRead(MS_U8 *u8Buf, MS_U16 u16Size)
{
    MsOS_DelayTask(50);
    MS_BOOL bRet = FALSE;

    if (NULL != m_data)
    {
        if (u16Size == 0)
            memcpy(u8Buf, m_data, 1);
        else
            memcpy(u8Buf, m_data, u16Size);
        free(m_data);
        m_data = NULL;
        bRet = TRUE;
        UBOOT_DEBUG("Cmd_DataRead Success\n");
    }
    else
    {
         UBOOT_ERROR("Cmd_DataRead Failed\n");
    }
    return bRet;
}

MS_U32 GetUrsaVersionInfo(void)
{
    MS_U8 major = 0xFF;
    MS_U8 minor = 0xF;
    MS_U8 u8Buff[MFC_CMD_DATA_SUM] = {0x03, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00};
     MS_BOOL bRet =  FALSE;

    u8Buff[2] = 0;
    CalCheckSum(u8Buff, MFC_CMD_DATA_SUM);
    bRet = Cmd_DataWrite(u8Buff);
    MsOS_DelayTask(URSA_CMD_INTERVAL_TIME);
    bRet = Cmd_DataRead(&major, 0);

     u8Buff[2] = 1;
    CalCheckSum(u8Buff, MFC_CMD_DATA_SUM);
    bRet = Cmd_DataWrite(u8Buff);
    MsOS_DelayTask(URSA_CMD_INTERVAL_TIME);
    bRet = Cmd_DataRead(&minor, 0);

    printf("6M60 GetVersion major:%x minor:%x bRet=%d\n", major, minor, bRet);

    return ( MS_U16)(((MS_U16)major << 8 ) | minor);
}

int do_GetUrsaVersionInfo (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    InitUrsaCmdData();
    GetUrsaVersionInfo();
}

int do_Change_Uart_connect_port2 (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MS_BOOL ret = FALSE;
    MDrv_UART_SetIOMapBase();
    //Whaley Board use RX2/TX2 Uart.
    //MStar 095D board init at mboot/.../mhal_sys.c   use RX5/TX5
    //6M60 uart Link to Soft E_UART_PIU_UART1
    ret = mdrv_uart_connect(E_UART_PORT2, E_UART_PIU_UART1);
    if (ret < 0)
    {
        UBOOT_ERROR("device_ursa:: E_UART_PORT2, UARTInit control interface Fail\n");
        return FALSE;
    }
}

U_BOOT_CMD(getversion, CONFIG_SYS_MAXARGS, 1, do_GetUrsaVersionInfo,
    "6M60 Get Version\n",    NULL);

U_BOOT_CMD(whaley_6m60_uart, CONFIG_SYS_MAXARGS, 1, do_Change_Uart_connect_port2,
    "change 6M60 Connect to E_UART_PORT2\n",    NULL);

#endif //__DRV_URSA_6M60_C__