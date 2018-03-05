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
#ifndef _Mstar_Boot_Header_
#define _Mstar_Boot_Header_
#include <CusBoot.h>
#include <ShareType.h>

#if defined (CONFIG_SECURITY_BOOT)
#define BOOT_COMMAND "authenticate recovery 0x25000000; bootm 0x25000000"
#else
  #if (ENABLE_MODULE_MMC == 1)
  #define BOOT_COMMAND "mmc read.p 0x45000000 recovery 0x00800000; bootm   0x45000000"
  #else
  #define BOOT_COMMAND "nand read.e 0x45000000 recovery 0x00800000; bootm 0x45000000"
  #endif
#endif

typedef enum
{
    EN_BOOT_MODE_UNKNOWN,
    EN_BOOT_MODE_NORMAL,
    EN_BOOT_MODE_RECOVERY,
    EN_BOOT_MODE_FORCEUGRADE,
    EN_BOOT_MODE_USB_UPGRADE,
    EN_BOOT_MODE_NET_UPGRADE,
    EN_BOOT_MODE_OAD_UPGRADE,
#if (ENABLE_MODULE_ENV_UPGRADE_FROM_BANK == 1)
    EN_BOOT_MODE_ENV_UPGRADE,
#endif
    EN_BOOT_MODE_USB_RECOVRY_UPGRADE,
    EN_BOOT_MODE_OTA_UPGRADE,
    EN_BOOT_MODE_RECOVRY_WIPE_DATA,
    EN_BOOT_MODE_RECOVRY_WIPE_CACHE,
    EN_BOOT_MODE_UART_DEBUG,
#if CONFIG_RESCUE_ENV
    EN_BOOT_MODE_BRICK_TERMINATOR,
#endif
#if (ENABLE_MODULE_SYSTEM_RESTORE == 1)
    EN_BOOT_MODE_SYSTEM_RESTORE,
#endif
#if defined (CONFIG_SEC_SYSTEM)
    EN_BOOT_MODE_SECOND_SYSTEM,
#endif
#if defined (CONFIG_AN_FASTBOOT_ENABLE)
   EN_BOOT_MODE_FASTBOOT,
#endif

}EN_BOOT_MODE;

#if (CONFIG_DYNAMIC_IR)
///----------------------------------------------------------------------//
//--------------------------IR Bootcheck -----------------------------------//

#define IR_MAX 5

#define IR_KEY_MAX 16

typedef enum
{
    IR_BOOTCASE_INVAILUE=0x00,
    IR_BOOTCASE_RECOVERY,
    IR_BOOTCASE_PGRADEUSBOTA,
    IR_BOOTCASE_FORCEUPGRADE,
    IR_BOOTCASE_UPGRADEBOOTLOADER,
    IR_BOOTCASE_RECOVERYWIPEDATA,
    IR_BOOTCASE_RECOVERYWIPECACHE,
    IR_BOOTCASE_SECOND_SYSTEM,
    IR_BOOTCASE_FASTBOOT,
    IR_BOOTCASE_BRICK_TERMINATOR_RECOVERY,
    IR_BOOTCASE_BOOTCASE_MAX
} eIRBootCase;

typedef struct {
    U8 u8Protocol;
    U32 u32Headcode;
    U16 u16Key[IR_KEY_MAX];
}IRBootCheckInfo_t;

typedef IRBootCheckInfo_t st_IRBootCheck_t[IR_MAX];
///----------------------------------------------------------------------//
//--------------------------IR wakeup info  -----------------------------------//

typedef struct {
    U8 u8Protocol;
    U32 u32Headcode;
    U16 u16Key;
}IRWakeup_t;

typedef IRWakeup_t st_IRWakeup_t[IR_MAX];

#endif

#if defined (CONFIG_DUAL_SYSTEM)
#define DUAL_SYSTEM_PARTITION_SUFFIX "1"
#define DUAL_MBOOT_PARTITION_SUFFIX "BAK"

unsigned char read_bootflag(void);
int do_bootflag_read (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_bootflag_set(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_sboot_verify_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_copy_mpool(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_mboot_verify_check(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
int do_bootcheck (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int isBootToRecovery(void);

#if ENABLE_STB_ECOS_BOOT
int do_read_boot_info (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#if defined(CONFIG_SEC_SYSTEM)
int do_Boot2secKernel (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#ifdef CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE
int do_smc (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
#if defined (CONFIG_ARMv8_ARM_TRUSTED_FIRMWARE)|| defined(CONFIG_ARMv7_OPTEE)
int do_read_arm_fw (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
int do_read_optee (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif

#endif
