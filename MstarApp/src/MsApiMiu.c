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
#include <common.h>
#include <MsMath.h>
#include <MsRawIO.h>
#include <stdio.h>
#include <MsTypes.h>
#include <MsOS.h>
#include <drvBDMA.h>
#include <drvMIU.h>
#include <MsDebug.h>
#include <miu/MsDrvMiu.h>
#include <ShareType.h>
#include <bootlogo/MsPoolDB.h>
#include <MsMmap.h>
#include <MsApiMiu.h>
#include <MsEnvironment.h>


MS_U32 u32Lx1Start = 0, u32Lx1Size = 0;
MS_U32 u32Lx2Start = 0, u32Lx2Size = 0;
MS_U32 u32Lx3Start = 0, u32Lx3Size = 0;
MS_U32 u32Block = 0;

int Set_DRAM_Size(void)
{
    MS_U32 u32DramSize1 = 0, u32DramSize2 = 0,u32DramSize3 = 0,u32DramSize4 = 0;
    char *start=0;
    char dram_size_tmp[32];
    char cEnvBuffer[512]= {0};
    char * tok;
    char *s = getenv("KERNEL_PROTECT");
    memset(cEnvBuffer, 0, sizeof(cEnvBuffer));
    if (s != NULL)
    {
        MsDrv_MiuInit();
        strncpy(cEnvBuffer, s,255);
        tok = strtok (cEnvBuffer," ");
        if(isDetectMmap()==0)
        {
            while (tok != NULL)
            {
                if((start = strstr (tok, "DRAM_SIZE1=")) != NULL)
                {
                    memset(dram_size_tmp, 0, sizeof(dram_size_tmp));
                    strncpy(dram_size_tmp, tok+11, strlen(dram_size_tmp)-8);
                    u32DramSize1=(simple_strtoul(dram_size_tmp, NULL, 16));
                    UBOOT_DEBUG("DRAM_SIZE1 0x%lx \n",u32DramSize1);
                    MDrv_MIU_Dram_Size(E_MIU_0, dram_size_to_enum(u32DramSize1));
                }
                else if((start = strstr (tok, "DRAM_SIZE2=")) != NULL)
                {
                    memset(dram_size_tmp,0,sizeof(dram_size_tmp));
                    strncpy(dram_size_tmp, tok+11, strlen(dram_size_tmp)-8);
                    u32DramSize2=(simple_strtoul(dram_size_tmp, NULL, 16));
                    UBOOT_DEBUG("DRAM_SIZE2 0x%lx \n",u32DramSize2);
                    if(u32DramSize2!=0)
                    {
                        MDrv_MIU_Dram_Size(E_MIU_1, dram_size_to_enum(u32DramSize2));
                    }
                }
                else if((start = strstr (tok, "DRAM_SIZE3=")) != NULL)
                {
                    memset(dram_size_tmp,0,sizeof(dram_size_tmp));
                    strncpy(dram_size_tmp, tok+11, strlen(dram_size_tmp)-8);
                    u32DramSize3=(simple_strtoul(dram_size_tmp, NULL, 16));
                    UBOOT_DEBUG("DRAM_SIZE3 0x%lx \n",u32DramSize3);
                    if(u32DramSize3!=0)
                    {
                        MDrv_MIU_Dram_Size(2, dram_size_to_enum(u32DramSize3));// 2:E_MIU_2
                    }
                }
                else if((start = strstr (tok, "DRAM_SIZE4=")) != NULL)
                {
                    memset(dram_size_tmp,0,sizeof(dram_size_tmp));
                    strncpy(dram_size_tmp, tok+11, strlen(dram_size_tmp)-8);
                    u32DramSize4=(simple_strtoul(dram_size_tmp, NULL, 16));
                    UBOOT_DEBUG("DRAM_SIZE4 0x%lx \n",u32DramSize3);
                    if(u32DramSize4!=0)
                    {
                        MDrv_MIU_Dram_Size(3, dram_size_to_enum(u32DramSize4));// 2:E_MIU_3
                    }
                }
                tok = strtok (NULL, " ");
            }
#if ENABLE_NIKON || ENABLE_MILAN || ENABLE_MARLON || ENABLE_KERES || ENABLE_KIRIN || ENABLE_KRIS || ENABLE_KAYLA || ENABLE_K5TN
            if (u32DramSize1 == 0)
            {
                UBOOT_DEBUG("#######################################################################\n");
                if (u32DramSize1 == 0)
                    UBOOT_DEBUG("# [PROTECT WARNING], miu kernel protect is not enabled on first dram  #\n");
                UBOOT_DEBUG("#######################################################################\n");
                return 0;
            }
#else
            if (u32DramSize1 == 0 || u32DramSize2 == 0 || u32DramSize3 == 0)
            {
                UBOOT_DEBUG("#######################################################################\n");
                if (u32DramSize1 == 0)
                    UBOOT_DEBUG("# [PROTECT WARNING], miu kernel protect is not enabled on first dram  #\n");
                if (u32DramSize2 == 0)
                    UBOOT_DEBUG("# [PROTECT WARNING], miu kernel protect is not enabled on second dram #\n");
                #if ENABLE_MONACO
                if (u32DramSize3 == 0)
                    UBOOT_DEBUG("# [PROTECT WARNING], miu kernel protect is not enabled on 3rd dram #\n");
                #endif
                    UBOOT_DEBUG("#######################################################################\n");
                if (u32DramSize1 == 0 || u32DramSize2 == 0)
                {
                    return 0;
                }
            }
#endif
        }//if(isDetectMmap()==0)
    }
    return 0;
}

int MsApi_kernelProtect(void)
{
    MS_U64 u64LxStart=0,u64Size=0;
    MS_U8 u8BlockMiu[E_MIU_ID_MAX]="\0";
    MS_U8 u8MiuIndex = 0;
    MS_U8 LX_ID=0;

    if ( getenv("KERNEL_PROTECT") != NULL)
    {
        for(LX_ID=E_LX;LX_ID<E_MAX;LX_ID++)
        {
            if(Get_LX_MemInfo(LX_ID,&u64LxStart,&u64Size)==TRUE)
            {
                u8MiuIndex=MsApi_Miu_GetID(u64LxStart);
                UBOOT_DEBUG("Lx%d start:[0x%llx] Size[0x%llx] in MIU(%d)\n",(LX_ID+1),u64LxStart,u64Size,u8MiuIndex);
                if(u64Size>0)
                {
                    MsDrv_kernelProtect(u8BlockMiu[u8MiuIndex],u64LxStart,(MS_U64)((MS_U64)u64LxStart + (MS_U64)u64Size));
                    u8BlockMiu[u8MiuIndex]++;
                }
            }
        }

    }
    return 0;
}


/*
setenv DynMmapID E_LX_MEM2 E_LX_MEM2 E_LX_MEM2 E_LX_MEM2 E_LX_MEM2 E_LX_MEM2 E_LX_MEM2
setenv MIUPRO0 0 0 E_LX_MEM2 0x00000000 0x00200000 0 43 57 82 08 11 12 13 15 24 26 28 29 0 0 0 0
setenv MIUPRO1 0 1 E_LX_MEM2 0x00300000 0x00500000 0 03 06 07 08 11 12 13 15 24 26 28 29 0 0 0 0
setenv MIUPRO2 0 2 E_LX_MEM2 0x00600000 0x00700000 0 03 06 07 08 11 12 13 15 24 26 28 29 0 0 0 0
setenv MIUPRO3 0 3 E_LX_MEM2 0x00600000 0x00700000 0 03 06 07 08 11 12 13 15 24 26 28 29 0 0 0 0
setenv MIUPRO4 1 0 E_LX_MEM2 0x00600000 0x00700000 0 03 06 07 08 11 12 13 15 24 26 28 29 0 0 0 0
setenv MIUPRO5 1 1 E_LX_MEM2 0x00600000 0x00700000 0 03 06 07 08 11 12 13 15 24 26 28 29 0 0 0 0
setenv MIUPRO6 1 2 E_LX_MEM2 0x00600000 0x00700000 0 03 06 07 08 11 12 13 15 24 26 28 29 0 0 0 0
setenv MIUPRO7 1 3 E_LX_MEM2 0x00600000 0x00700000 0 03 06 07 08 11 12 13 15 24 26 28 29 0 0 0 0


*/
int MsApi_MiuProtect(void)
{
    char cTempBuffer[BUFFER_SIZE]= {0};
    char cEnvBuffer[BUFFER_SIZE]= {0};
    char * p_str = NULL;
    char * tok=NULL;
    int index=0;
    int client_index=0;
    U32 temp_start=0;
    U32 temp_length=0;
    MS_BOOL checkIndex=TRUE;
    UBOOT_TRACE("IN\n");
    do
    {
        memset(cTempBuffer, 0, BUFFER_SIZE);
        memset(cEnvBuffer, 0, BUFFER_SIZE);
        p_str = NULL;
        tok = NULL;
        snprintf(cTempBuffer, BUFFER_SIZE, "MIUPRO%d", index);
        UBOOT_DEBUG(" getenv[%s] \n",cTempBuffer);
        p_str = getenv (cTempBuffer);
        if(p_str != NULL)
        {
            strncpy(cEnvBuffer, p_str,BUFFER_SIZE-1);
            stMIU_PortectInfo portectInfo;
            memset(&portectInfo, 0, sizeof(stMIU_PortectInfo));
            tok = strtok (cEnvBuffer," ");
            portectInfo.miu_id=(MIU_ID)((int)simple_strtol(tok, NULL, 16));
            tok = strtok (NULL," ");
            portectInfo.u8Group=(int)simple_strtol(tok, NULL, 16);
            tok = strtok (NULL," ");
            portectInfo.c_Mmap_Id=tok;
            UBOOT_DEBUG(" MMAP ID =%s",portectInfo.c_Mmap_Id);
            checkIndex=TRUE;

            if( NULL!= strstr (tok, "E_") )
            {

                tok = strtok (NULL," ");//start not used
                tok = strtok (NULL," ");//end not used
                if(get_addr_from_mmap(portectInfo.c_Mmap_Id, &temp_start)==0)
                {
                    portectInfo.u32Start=temp_start;
                }
                else
                {
                    UBOOT_DEBUG(" index[%d]%s get mmap fail %s !!!\n",index,cTempBuffer,portectInfo.c_Mmap_Id);
                    checkIndex=FALSE;

                }
                if(get_length_from_mmap(portectInfo.c_Mmap_Id, &temp_length)==0)
                {
                    portectInfo.u32End=temp_start+temp_length;
                }
                else
                {
                    UBOOT_DEBUG(" index[%d]%s get mmap fail %s !!!\n",index,cTempBuffer,portectInfo.c_Mmap_Id);
                    checkIndex=FALSE;
                }
            }
            else
            {

                tok = strtok (NULL," ");
                portectInfo.u32Start=simple_strtol(tok, NULL, 16);
                tok = strtok (NULL," ");
                portectInfo.u32End=simple_strtol(tok, NULL, 16);
                if(portectInfo.u32End==0)
                {
                    UBOOT_DEBUG(" index[%d] %s end of address is 0  !!!\n",index,cTempBuffer);

                    if(portectInfo.u32Start==0)
                    {
                        index=index+1;
                        continue;
                    }
                    else
                        checkIndex=FALSE;
                }
            }
            if(checkIndex==FALSE)
            {
                UBOOT_ERROR(" %s parse Fail  !!!\n",cTempBuffer);
                index=index+1;
                continue;
            }
            tok = strtok (NULL," ");// do nothing for enable/disable flag
            for(client_index=0;client_index<16;client_index++)
            {
                tok = strtok (NULL," ");
                if(tok==NULL)
                {
                    UBOOT_DEBUG(" index[%d] Client ID end of index [%d] !!!\n",index,client_index);
                    break;
                }
                portectInfo.clientID[client_index]=(MS_U8)simple_strtol(tok, NULL, 10);
            }
            if(dbgLevel>=EN_DEBUG_LEVEL_DEBUG)
            {
                printf(" Block[%d]start[%lx]end[%lx]\n",portectInfo.u8Group,portectInfo.u32Start,portectInfo.u32End);
                printf("portect IDs \n");
                for(client_index=0;client_index<16;client_index++)
                {
                    printf("%d[0x%x] ",client_index,portectInfo.clientID[client_index]);
                    if(client_index==7)
                        printf("\n");
                }
                printf("\n");
            }

            MsDrv_MIU_Protect(portectInfo.u8Group,portectInfo.clientID,portectInfo.u32Start,portectInfo.u32End,true);

        }
        else
        {
            UBOOT_DEBUG("No Portect Infor !!!\n");
            return -1;
        }
        index=index+1;
    }
    while (p_str!=NULL);


    UBOOT_TRACE("OK\n");
    return 0;

}

#if (!CONFIG_UBOOT_MINI_LIB)
int do_MsApi_kernelProtect( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    return MsApi_kernelProtect();

}

int do_MsApi_MiuProtect( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{

    return MsApi_MiuProtect();

}
#endif
void MsApi_RunTimePmProtect(unsigned int u32StartAddr, unsigned int u32EndAddr)
{
    u32StartAddr=u32StartAddr;
    u32EndAddr=u32EndAddr;
#if (CONFIG_ENABLE_RT_PM_PROTECT)
    MsDrv_PMProtect(3,u32StartAddr,u32EndAddr); //64K run time PM
#endif
}

#if (!CONFIG_UBOOT_MINI_LIB)
int do_MsApi_RunTimePmProtect( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    unsigned int StarAddr=simple_strtoul(argv[1], NULL, 16);
    unsigned int EndAddr=simple_strtoul(argv[2], NULL, 16);
    MsApi_RunTimePmProtect(StarAddr, EndAddr);
    return 0;
}
#endif

extern unsigned int u32UbootStart;
extern unsigned int u32UbootEnd;
#if 0 // by jh

// return 1 for overlap case, 0 for non-overlap case
static MS_U32 isOverlapUboot(MS_U32 u32Start, MS_U32 u32Size)
{
    if ((u32UbootStart <= u32Start) && (u32Start <= u32UbootEnd))
        return 1;
    if ((u32UbootStart <= (u32Start+u32Size)) && ((u32Start+u32Size) <= u32UbootEnd))
        return 1;
    return 0;
}

static MS_U32 return_or_block(MS_U32 u32RetVal)
{
    if (u32Block)
        while(1);

    return u32RetVal;
}


#define TEST_OFFSET (128*1024*1024) // set 128M offset
#define TEST_OFFSET_MASK (TEST_OFFSET-1)
#define MIU1_OFFSET (0xA0000000)
#define TEST_RANGE (0x80000000) // set 2G test range for each miu
static MS_U32 write_test_to_lxmem(MS_U32 u32Start, MS_U32 u32Size)
{
    MS_U32 i;
    MS_U32 u32DramValue[u32Size/sizeof(MS_U32)];
    MS_U32* pu32Start = (MS_U32*)u32Start;
    MS_U32 failed = 0;
    MS_U32 u32DramSize = 0, u32MiuOffset = 0;
    MS_U32 u32BdmaStart = 0;
    BDMA_CpyType eBdmaCpyType;

    if (!u32Size)
    {
        printf("[PROTECT ERROR], lxmem size = 0\n");
        return return_or_block(-1);
    }

    // 1. decide dram parameters
    u32DramSize = (u32Start < MIU1_OFFSET) ? u32DramSize1 : u32DramSize2;
    u32MiuOffset = (u32Start < MIU1_OFFSET) ? 0 : MIU1_OFFSET;
    eBdmaCpyType = (u32Start < MIU1_OFFSET) ? E_BDMA_FLASH2SDRAM : E_BDMA_FLASH2SDRAM1;
    if (!u32DramSize)
    {
        printf("[PROTECT ERROR], dram size = 0\n");
        return return_or_block(-1);
    }

    // 2. backup parital values in protected region
    for (i = 0; i < sizeof(u32DramValue)/sizeof(MS_U32); i++)
        u32DramValue[i] = *(pu32Start + i);

    // 3. bdma attack
    u32BdmaStart = (u32Start & TEST_OFFSET_MASK) + u32MiuOffset;
    for (i = 0; i < (TEST_RANGE/TEST_OFFSET); i++)
    {
        u32BdmaStart += TEST_OFFSET;
        if (isOverlapUboot(u32BdmaStart, u32Size))
            continue;
        // bdma source is not important, just fill it with reasonable address
        MDrv_BDMA_CopyHnd(0x20000, u32BdmaStart, u32Size, eBdmaCpyType, 0);
    }
    flush_cache(u32Start, u32Size); // necessary, or the value would be kept in cache

    // 4. check whether the region is protected or not
    for (i = 0; i < sizeof(u32DramValue)/sizeof(MS_U32); i++)
    {
        if (u32DramValue[i] != *(pu32Start + i))
        {
            failed = 1;
            break;
        }
    }

    if (failed)
    {
        printf("[PROTECT ERROR], fail at address 0x%p \n", pu32Start + i);
        return return_or_block(-1);
    }
    else
    {
        printf("[PROTECT INFO], pass at address 0x%X, size 0x%X\n", (unsigned int)u32Start, (unsigned int)u32Size);
        return 0;
    }
}

static MS_U32 isDramExist(MS_U32 u32LxStart)
{
    if (u32LxStart < MIU1_OFFSET)
        return u32DramSize1 ? 1 : 0;
    else
        return u32DramSize2 ? 1 : 0;
}

#define TEST_SIZE (256) // it's enough to test 256 bytes on lx mem head & tail
#endif
void MsApi_kernelProtectBist(void)
{
    #if 0 //jh
    MIU_PortectInfo info;

    if (isDramExist(u32Lx1Start) && u32Lx1Size >= TEST_SIZE)
    {
        write_test_to_lxmem(u32Lx1Start, TEST_SIZE);
        write_test_to_lxmem(u32Lx1Start + u32Lx1Size - TEST_SIZE, TEST_SIZE);
    }
    if (isDramExist(u32Lx2Start) && u32Lx2Size >= TEST_SIZE)
    {
        write_test_to_lxmem(u32Lx2Start, TEST_SIZE);
        write_test_to_lxmem(u32Lx2Start + u32Lx2Size - TEST_SIZE, TEST_SIZE);
    }
    if (isDramExist(u32Lx3Start) && u32Lx3Size >= TEST_SIZE)
    {
        write_test_to_lxmem(u32Lx3Start, TEST_SIZE);
        write_test_to_lxmem(u32Lx3Start + u32Lx3Size - TEST_SIZE, TEST_SIZE);
    }

    MDrv_MIU_GetProtectInfo(0, &info);
    MDrv_MIU_GetProtectInfo(1, &info);
    #endif
}

#if (!CONFIG_UBOOT_MINI_LIB)
int do_MsApi_kernelProtectBist(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    MsApi_kernelProtectBist();
    return 0;
}

void MsApi_NuttxProtect(MS_U32 u32StartAddr, MS_U32 u32EndAddr)
{
    UBOOT_DEBUG("IN\n");

    UBOOT_DEBUG("u32StartAddr=0x%x\n",(unsigned int)u32StartAddr);
    UBOOT_DEBUG("u32EndAddr=0x%x\n",(unsigned int)u32EndAddr);
    MsDrv_NuttxProtect(3,u32StartAddr,u32EndAddr);

    UBOOT_DEBUG("OK\n");
}

int do_MsApi_NuttxProtect(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
        U32 u32Start=0;
        U32 u32End=0;
        if(argc<3) {
        cmd_usage(cmdtp);
        return -1;
    }
    u32Start=simple_strtoul(argv[1], NULL, 16);
    u32End=simple_strtoul(argv[2], NULL, 16);
    UBOOT_DEBUG("u32Start=0x%x\n",u32Start);
        UBOOT_DEBUG("u32End=0x%x\n",u32End);
    MsApi_NuttxProtect(u32Start,u32End); //"protect address" and "protect lenth" must align to 64K
    return 0;
}
#endif

extern U8 MDrv_MIU_SelectMIU_UBoot(U8 bMIU1, U16 *au16SelMiu, U8 u8GpNum);
extern U8 MDrv_MIU_SetGroupPriority_UBoot(U8 bMIU1, U8 *au8GpPriority, U8 u8GpNum);

static void set_miu_group_priority(U8 u8MiuIdx, const char* cStr)
{
    if (cStr)
    {
        int i = 0;
        int j = 0;
        int length = 0;
        U8 au8GpPriority[MIU_MAX_GP_PRIORITY_NUM] = {0};

        for (;;)
        {
            if (cStr[i] != ':')
            {
                au8GpPriority[j++] = cStr[i] - 0x30;
            }
            else
            {
                ++length; // length is ':' number and priority number need add one more.
            }

            if ('\0' == cStr[i])
            {
                ++length;
                break;
            }

            ++i;
        }
        MDrv_MIU_SetGroupPriority_UBoot(u8MiuIdx, au8GpPriority, length);
    }
}

static void select_miu(U8 u8MiuIdx, const char* cStr)
{
    if (cStr)
    {
        int i = 0;
        int j = 0;
        int length = 0;

        for (;;)
        {
            if (':' == cStr[i])
            {
                ++length;
            }

            if ('\0' == cStr[i])
            {
                ++length; // length is ':' number and priority number need add one more.
                break;
            }

            ++i;
        }

        i = 0;
        j = 0;
        U16 au16SelMiu[MIU_MAX_GP_NUM] = {0};

        for (;;)
        {
            au16SelMiu[j++] = ascii_to_hex(&cStr[i], 4);
            i += 5;

            if (length == j)
            {
                break;
            }
        }

        MDrv_MIU_SelectMIU_UBoot (u8MiuIdx, au16SelMiu, length);
    }
}

static void set_miu(void)
{
    set_miu_group_priority(0, getenv("MIU0_GROUP_PRIORITY"));
    set_miu_group_priority(1, getenv("MIU1_GROUP_PRIORITY"));

#if 0
    set_miu_group_priority(2, getenv("MIU2_GROUP_PRIORITY"));
#endif
    select_miu(0, getenv("MIU0_GROUP_SELMIU"));
    select_miu(1, getenv("MIU1_GROUP_SELMIU"));

#if 0
    select_miu(2, getenv("MIU2_GROUP_SELMIU"));
#endif
}

int do_set_miu(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    set_miu();
    Set_DRAM_Size();
    return 0;
}

