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
#define DRV_SWIR_C
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include <common.h>
#include <MsTypes.h>
#include <ShareType.h>
#include <drvIR.h>
#include <MsIRQ.h>
#include <MsOS.h>
#include <MsUboot.h>
#include "drvSWIR.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Config
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static U32 u32GetKeyTime = 0;
static U8 last_pnflag = 0xFF;
static U8 u8Protocol = 0xFF;
static U32 u32Headcode = 0xFFFFFFFF;
static U16 u16Keycode = 0xFFFF;
static U8 u8Repeat = 0xFF;
static U8 u8GetKeyFlag = 0;
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

#define IR_INFO(msg...)  printf(msg)

#define DEBUG_IR 1

#if DEBUG_IR
static U8 count_prev =0;  //for record fifo max data size
#endif

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static unsigned long _MDrv_IR_GetSystemTime(void)
{
    return  MsOS_GetSystemTime();
}

static void Reset_Current_Time(void)
{
    u32GetKeyTime = _MDrv_IR_GetSystemTime();
}

static U32 Get_Off_Time(void)
{
    return (_MDrv_IR_GetSystemTime() - u32GetKeyTime);
}
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
//=============Standard NEC Timming Spec  =============//
#define NEC_NBITS              32
#define NEC_HEADER_PULSE       9000//us
#define NEC_HEADER_SPACE       4500
#define NEC_REPEAT_SPACE       2250
#define NEC_BIT_PULSE          560
#define NEC_BIT_0_SPACE        560
#define NEC_BIT_1_SPACE        1680
#define NEC_REPEAT_TIME        120  //ms

static U8 u8Status_nec = STATE_INACTIVE;
static U8 u8BitCount_nec = 0;
static U32 u32DataBits_nec = 0;
static U8 u8Done_nec = 0;
static U8 u8RepeatStart_nec = 0;
static U8 u8Repeat_nec = 0;
static U8 u8RepeatNum_nec = 0;
static U32 u32PrevData_nec = 0;
static U8 MDrv_IR_Decode_NEC(U16 shotcount, U8 flag)
{
    switch(u8Status_nec)
    {
        case STATE_INACTIVE:
            if(flag)
                break;
            u8BitCount_nec = 0;
            u32DataBits_nec = 0;
            u8Done_nec = 0;
            u8Repeat_nec = 0;
            if((shotcount> 8000)&&(shotcount<10000))
            {
                u8Status_nec = STATE_HEADER_SPACE;
                return 0;
            }
            break;
        case STATE_HEADER_SPACE:
            if(!flag)
            {
                break;
            }
            if((shotcount>3500)&&(shotcount<5500))
            {
                u8RepeatStart_nec = 0;
                u8RepeatNum_nec = 0;
                u8Status_nec = STATE_BIT_PULSE;
                return 0;
            }
            else if((shotcount>1900)&&(shotcount<2650))
            {//repeat
                if(u8RepeatStart_nec)
                {
                    if(Get_Off_Time() < NEC_REPEAT_TIME)
                    {
                        Reset_Current_Time();
                        u8Repeat_nec = 1;
                        u8Status_nec = STATE_INACTIVE;
                        return 0;
                    }
                }
            }
            IR_INFO("[Mboot_IR] STATE_HEADER_SPACE error shot= %d\n",shotcount);
            break;
        case STATE_BIT_PULSE:
            if (flag)
            {
                break;
            }
            if((shotcount<400)||(shotcount>700))
            {
                IR_INFO("[Mboot_IR] STATE_BIT_PULSE error shot= %d\ = %d\n",shotcount);
                break;
            }
            u8Status_nec = STATE_BIT_SPACE;
            return 0;
        case STATE_BIT_SPACE:
            if (!flag)
            {
                break;
            }
            u32DataBits_nec >>= 1;
            if ((shotcount>1300)&&(shotcount<2000))
                u32DataBits_nec |= 0x80000000;
            else if ((shotcount<400)||(shotcount>700))
            {
                IR_INFO("[Mboot_IR] STATE_BIT_SPACE error shot = %d\n",shotcount);
                break;
            }
            u8BitCount_nec++;

            if (u8BitCount_nec == NEC_NBITS)
            {
                u8Done_nec = 1;
            }
            else
                u8Status_nec = STATE_BIT_PULSE;

            return 0;
        default:
            break;

    }
     u8Status_nec = STATE_INACTIVE;
     return 0;
}

static BOOLEAN MDrv_IR_GetKeyCode_NEC(U8 *u8IRProtocol,U32 *u32IRHeadcode,U16 *u16IRKeycode,U8 *u8Repeat)
{
    U8 u8Addr = 0;
    U8 u8AddrInv =0;
    U8 u8Cmd =0;
    U8 u8CmdInv =0;
    if(u8Repeat_nec)
    {
        u8CmdInv  = (u32PrevData_nec >> 24) & 0xff;
        u8Cmd     = (u32PrevData_nec >> 16) & 0xff;
        u8AddrInv = (u32PrevData_nec >>  8) & 0xff;
        u8Addr    = (u32PrevData_nec >>  0) & 0xff;
        *u8IRProtocol = E_IR_TYPE_NEC;
        *u32IRHeadcode = u8Addr <<8 |u8AddrInv;
        *u16IRKeycode = u8Cmd;
        *u8Repeat = 1;
        u8Status_nec = STATE_INACTIVE;
        u8Repeat_nec = 0;
        Reset_Current_Time();
        return TRUE;
    }
    else if(u8Done_nec)
    {
        u8Done_nec = 0;
        u8CmdInv  = (u32DataBits_nec >> 24) & 0xff;
        u8Cmd     = (u32DataBits_nec >> 16) & 0xff;
        u8AddrInv = (u32DataBits_nec >>  8) & 0xff;
        u8Addr    = (u32DataBits_nec >>  0) & 0xff;
        *u8IRProtocol = E_IR_TYPE_NEC;
        *u32IRHeadcode = u8Addr <<8 |u8AddrInv;
        *u16IRKeycode = u8Cmd;
        *u8Repeat = 0;
        u32PrevData_nec = u32DataBits_nec;
        u8RepeatStart_nec = 1;
        u8Status_nec = STATE_INACTIVE;
        Reset_Current_Time();
        return TRUE;
    }
    return FALSE;
}

//=============Standard RC5 Timming Spec  =============//

#define RC5_NBITS         14
#define RC5_BIT_MIN       889  //us
#define RC5_BIT_MAX       1778 //us

static U8 u8Status_rc5 = STATE_INACTIVE;
static U8 u8BitCount_rc5 = 0;
static U32 u32DataBits_rc5 = 0;
static U8 u8Done_rc5 = 0;
static U8 u8PrevFlag_rc5 = 0;
static U32 databits_to_data(U32 data_bits,U8 bits,U8 reverse)
{
    U32 ret_data=0;
    U8 i = 0;
    U8 number = bits>>1;
    for(i=1;i<number+1;i++)
    {
        ret_data <<= 1;
        if(reverse)
        {
            if(((data_bits>>(bits-(i<<1)))&0x03)==1)
                ret_data |=0x01;
        }
        else
        {
            if(((data_bits>>(bits-(i<<1)))&0x03)==2)
                ret_data |=0x01;

        }
    }
    if(bits%2)
    {
        ret_data <<= 1;
        if(reverse)
        {
            if((data_bits&0x01) == 0)
                ret_data |=0x01;
        }
        else
        {
            if(data_bits&0x01)
                ret_data |=0x01;
        }
    }
    return ret_data;
}

static int MDrv_IR_Decode_RC5(U16 shotcount, U8 flag)
{
    switch (u8Status_rc5)
    {

    case STATE_INACTIVE:
        if (flag)
            break;
        //match RC5 start bit1  1
        if (shotcount>700&&shotcount<1100)
        {
            u8BitCount_rc5 = 0;
            u32DataBits_rc5 = 0;
            u8Status_rc5 = STATE_BIT_DATA;
            return 0;
        }
        //match RC5 start bit1 with bit2  0_High
        else if(shotcount>1400&&shotcount<2150)
        {
            u8BitCount_rc5 = 1;
            u32DataBits_rc5 |= 1;
            u8Status_rc5 = STATE_BIT_DATA;
        }
        else
            break;

    case STATE_BIT_DATA:
        if (shotcount>700&&shotcount<1100)
        {
            u8BitCount_rc5 ++;
            u32DataBits_rc5 <<= 1;
            u8Status_rc5 = STATE_BIT_DATA;
            if(!flag)
            {
                u32DataBits_rc5 |= 1;
            }
        }
        else if(shotcount>1400&&shotcount<2150)
        {
            u8BitCount_rc5 += 2;
            u32DataBits_rc5 <<= 2;
            u8Status_rc5 = STATE_BIT_DATA;
            if(!flag)
            {
                u32DataBits_rc5 |= 3;
            }
        }
        else
            break;
        if(u8BitCount_rc5 >= (RC5_NBITS*2 -3))
        {
            u8Done_rc5 = 1;
        }
        return 0;
    default:
        break;
    }

    u8Status_rc5 = STATE_INACTIVE;
    return 0;
}
static BOOLEAN MDrv_IR_GetKeyCode_RC5(U8 *u8IRProtocol,U32 *u32IRHeadcode,U16 *u16IRKeycode,U8 *u8Repeat)
{
    U32 u32Data;
    if(u8Done_rc5)
    {
        u8Done_rc5 = 0;
        u8Status_rc5 = STATE_INACTIVE;

        u32Data = databits_to_data(u32DataBits_rc5,u8BitCount_rc5,0);

        *u8IRProtocol = E_IR_TYPE_RC5;
        *u16IRKeycode    = (u32Data & 0x0003F) >> 0;
        *u32IRHeadcode   = (u32Data & 0x007C0) >> 6;
        if(u8PrevFlag_rc5 == ((u32Data & 0x00800) ? 1 : 0))
        {
            *u8Repeat = 1;
        }
        else
        {
            *u8Repeat = 0;
        }
        u8PrevFlag_rc5   = (u32Data & 0x00800) ? 1 : 0;
        *u16IRKeycode   += (u32Data & 0x01000) ? 0 : 0x40;
        return TRUE;
    }
    return FALSE;
}

//=============Standard RC6 Timming Spec  =============//
static U8 status = STATE_INACTIVE;
static U8 rc6_bits=0;
static U32 rc6_databits=0;
static U8 rc6_data[4] ={0};
static U8 data_count =0;
static U8 data_flag = 0;
static U8 rc6_mode=0;
static U8 done_flag =0;
static U8 u8PrevFlag_rc6 = 0;
static U8 rc6_databits_to_data(U32 data_bits,U8 bits)
{
    U8 ret_data=0;
    U8 i = 0;
    U8 number = bits>>1;
    for(i=1;i<number+1;i++)
    {
        ret_data <<= 1;
        if(((data_bits>>(bits-(i<<1)))&0x03)==2)
            ret_data |=0x01;
    }
    if(bits%2)
    {
        ret_data <<= 1;
        if(data_bits&0x01)
            ret_data |=0x01;
    }
    return ret_data;
}

static void MDrv_IR_Decode_RC6(U16 shotcount, U8 flag)
{
    switch(status)
    {
        case STATE_INACTIVE:
            if(flag)
                break;
            if (shotcount>2100&&shotcount<3100)
            {
                status = STATE_HEADER_SPACE;
                return;
            }
            else
                break;
        case STATE_HEADER_SPACE:
            if(!flag)
                break;
            if (shotcount>710&&shotcount<1050)
            {
                rc6_bits=0;
                rc6_databits=0;
                memset(rc6_data,0,sizeof(rc6_data));
                data_count =0;
                data_flag = 0;
                rc6_mode=0;
                done_flag =0;
                status = STATE_RC6_DATA;
                return;
            }
            else
            {
                //printf("[Lwc Debug Msg] error 1\n",0);
                break;
            }
        case STATE_RC6_DATA:
            if (shotcount>350 && shotcount<550)
            {
                rc6_bits++;
                rc6_databits <<= 1;
                if(!flag)
                    rc6_databits |=0x01;
            }
            else if (shotcount>710 && shotcount<1050)
            {
                rc6_bits += 2;
                rc6_databits <<= 2;
                if(!flag)
                    rc6_databits |=0x03;
            }
            else// error bits
            {
                //printf("[Lwc Debug Msg] error 2,PulseShot=%d\n",shotcount);
                break;
            }
            if(!data_flag)
            {
                if(rc6_bits>=7)//mode bits over
                {
                    rc6_mode = rc6_databits_to_data(rc6_databits,rc6_bits);
                    status = STATE_TRAILER;
                }
            }
            else
            {
                //data handle
                if((rc6_mode&0x07) == 0) //MODE_0  command + keycode
                {
                    if(data_count == 1)
                    {
                        if(rc6_bits>=15)
                        {
                            rc6_data[data_count] = rc6_databits_to_data(rc6_databits,rc6_bits);
                            done_flag =1;
                            status = STATE_INACTIVE;
                            return;
                        }
                    }
                }
                else if((rc6_mode&0x07) == 6) //MODE_6A 2bytes command +2bytes keycode [key value=last byte]
                {
                    if(data_count == 3)
                    {
                        if(rc6_bits>=15)
                        {
                            rc6_data[data_count] = rc6_databits_to_data(rc6_databits,rc6_bits);
                            done_flag =1;
                            status = STATE_INACTIVE;
                            return;
                        }
                    }
                }
                else
                {
                    IR_INFO("[RC6] u8RC6Mode =%x Decoder Not Support\n",(rc6_mode&0x07));
                    break;
                }
                if(rc6_bits>=16)
                {
                    rc6_data[data_count] = rc6_databits_to_data((rc6_databits>>(rc6_bits-16)),16);
                    rc6_bits = rc6_bits-16;
                    rc6_databits &= ~((~0)<<rc6_bits);
                    data_count++;
                }
            }
            return ;
       case STATE_TRAILER:
            if (shotcount>350&&shotcount<500)
            {
                rc6_bits += 1;
                rc6_databits <<= 1;
                if(!flag)
                    rc6_databits |=0x01;
            }
            else if (shotcount>710&&shotcount<1050)
            {
                rc6_bits += 2;
                rc6_databits <<= 2;
                if(!flag)
                    rc6_databits |=0x03;
            }
            else if (shotcount>1100&&shotcount<1500)
            {
                rc6_bits += 3;
                rc6_databits <<= 3;
                if(!flag)
                    rc6_databits |=0x07;
            }
            else
            {
                //printf("[Lwc Debug Msg] error 3\n",0);
                break;
            }
            if(rc6_bits>=12)
            {
                data_flag =1;
                rc6_bits= rc6_bits-12;
                rc6_databits &= ~((~0)<<rc6_bits);
                status = STATE_RC6_DATA;
            }
            return;
       default :
            //printf("[Lwc Debug Msg] error 4\n",0);
            break;
     }
     status = STATE_INACTIVE;
     return ;
}

static BOOLEAN MDrv_IR_GetKeyCode_RC6(U8 *u8IRProtocol,U32 *u32IRHeadcode,U16 *u16IRKeycode,U8 *u8Repeat)
{
    if(done_flag)
    {
        done_flag = 0;
        *u8IRProtocol = E_IR_TYPE_RC6;
        if((rc6_mode&0x07) == 0) //MODE_0  command + keycode
        {
            *u32IRHeadcode = rc6_data[0];
            *u16IRKeycode = rc6_data[1];
            *u8Repeat = 0;
        }
        else if((rc6_mode&0x07) == 6)
        {
            *u32IRHeadcode = rc6_data[0]<<8 |rc6_data[1];
            *u16IRKeycode = rc6_data[3];
            if(u8PrevFlag_rc6 == (rc6_data[2]&0x80))
            {
                *u8Repeat = 1;
            }
            else
            {
                *u8Repeat = 0;
            }
            u8PrevFlag_rc6 = rc6_data[2]&0x80;
        }
        else
        {
            IR_INFO("[RC6] u8RC6Mode =%x Decoder Not Support\n",(rc6_mode&0x07));
        }

        status = STATE_INACTIVE;
        return TRUE;
    }
    return FALSE;
}

//=============Standard RCA Timming Spec  =============//
#define RCA_NBITS		24
#define RCA_UNIT		500     //us
#define RCA_HEADER_PULSE  4000  //(8  * RCA_UNIT)
#define RCA_HEADER_SPACE  4000  //(8  * RCA_UNIT)
#define RCA_BIT_PULSE	  500	//(1  * RCA_UNIT)
#define RCA_BIT_0_SPACE	  1000  //(2  * RCA_UNIT)
#define RCA_BIT_1_SPACE	  2000  //(4  * RCA_UNIT)

static U8 u8Status_rca = STATE_INACTIVE;
static U8 u8BitCount_rca = 0;
static U32 u32DataBits_rca = 0;
static U8 u8Done_rca = 0;

static U8 bitrev_n(U8 value,U8 n)
{
    U8 i = 0;
    U8 ret = 0;
    for(i = 0;i<n;i++)
    {
        ret = ret<<1;
        if((value>>i)&0x01)
        {
            ret |= 1;
        }
    }
    return ret;
}
static int MDrv_IR_Decode_RCA(U16 shotcount, U8 flag)
{
    switch (u8Status_rca)
    {

    case STATE_INACTIVE:
        if (flag)
            break;

        if (shotcount>3200&&shotcount<4800)
        {
            u8BitCount_rca = 0;
            u32DataBits_rca = 0;
            u8Status_rca = STATE_HEADER_SPACE;
            return 0;
        }
        else
            break;

    case STATE_HEADER_SPACE:
        if (!flag)
            break;

        if (shotcount>3200&&shotcount<4800)
        {
            u8Status_rca = STATE_BIT_PULSE;
            return 0;
        }
        break;

    case STATE_BIT_PULSE:
        if (flag)
            break;

        if (shotcount>380&&shotcount<620)
        {
            u8Status_rca = STATE_BIT_SPACE;
            return 0;
        }
        break;

    case STATE_BIT_SPACE:
        if (!flag)
            break;
        u32DataBits_rca >>= 1;
        if (shotcount>800&&shotcount<1200) // log 1
            u32DataBits_rca |= 0x80000000;
        else if (!(shotcount>1600&&shotcount<2400)) //log 0
            break;
        u8BitCount_rca++;

        if (u8BitCount_rca == RCA_NBITS)
        {
            u8Done_rca = 1;
        }
        else
            u8Status_rca = STATE_BIT_PULSE;

        return 0;
    default:
        break;
    }

    u8Status_rca = STATE_INACTIVE;
    return 0;
}
static BOOLEAN MDrv_IR_GetKeyCode_RCA(U8 *u8IRProtocol,U32 *u32IRHeadcode,U16 *u16IRKeycode,U8 *u8Repeat)
{
    U8 u8Addr = 0;
    U8 u8AddrInv =0;
    U8 u8Cmd =0;
    U8 u8CmdInv =0;
    if(u8Done_rca) //preamble(addr) 4bit +command 8bit +preamble(addr) \ 4bit + command \ 8bit
    {
        u32DataBits_rca = u32DataBits_rca >>8;
        u8Done_rca = 0;
        u8Addr = bitrev_n(u32DataBits_rca&0x0F,4);
        u8Cmd = bitrev_n((u32DataBits_rca>>4)&0xFF,8);
        u8AddrInv= bitrev_n((u32DataBits_rca>>12)&0x0F,4);
        u8CmdInv= bitrev_n((u32DataBits_rca>>16)&0xFF,8);
        *u8IRProtocol = E_IR_TYPE_RCA;
        *u32IRHeadcode = (u8Addr<<8 | u8AddrInv);
        *u16IRKeycode = u8Cmd;
        *u8Repeat = 0;
        IR_INFO("[RCA] u8Addr = %x u8AddrInv = %x u8Cmd = %x u8CmdInv = %x\n",u8Addr,u8AddrInv,u8Cmd,u8CmdInv);
        u8Status_rca = STATE_INACTIVE;
        return TRUE;
    }
    return FALSE;
}

//================== Standard PANASONIC Timming Spec ===================//
#define PANASONIC_HEADER_PULSE    3488//us
#define PANASONIC_HEADER_SPACE    1744
#define PANASONIC_BIT_PULSE       436
#define PANASONIC_BIT_0_SPACE     436
#define PANASONIC_BIT_1_SPACE     1308
#define PANASONIC_REPEAT_TIME     150  //ms

static U8 u8Status_panasonic = STATE_INACTIVE;
static U8 u8BitCount_panasonic = 0;
static U8 u8DataBits_panasonic[6] = {0};
static U8 u8DataIdx_panasonic = 0;
static U8 u8Done_panasonic = 0;
static U8 u8RepeatNum_panasonic = 0;
static U8 u8PrevKey_panasonic = 0;
static U8 MDrv_IR_Decode_Panasonic(U16 shotcount, U8 flag)
{
    switch(u8Status_panasonic)
    {
        case STATE_INACTIVE:
            if(flag)
                break;
            u8DataIdx_panasonic = 0;
            u8BitCount_panasonic = 0;
            u8Done_panasonic = 0;
            memset(u8DataBits_panasonic,0,sizeof(u8DataBits_panasonic));
            if((shotcount> 2750)&&(shotcount<4200))
            {
                u8Status_panasonic = STATE_HEADER_SPACE;
                return 0;
            }
            else
                break;
        case STATE_HEADER_SPACE:
            if(!flag)
                break;
            if((shotcount>1350)&&(shotcount<2100))
            {
                u8Status_panasonic = STATE_BIT_PULSE;
                return 0;
            }
            break;
        case STATE_BIT_PULSE:
            if (flag)
                break;
            if((shotcount<300)||(shotcount>550))
                break;
            u8Status_panasonic = STATE_BIT_SPACE;
            return 0;
        case STATE_BIT_SPACE:
            if (!flag)
                break;
            u8DataBits_panasonic[u8DataIdx_panasonic] >>= 1;
            if ((shotcount>1000)&&(shotcount<1550))
                u8DataBits_panasonic[u8DataIdx_panasonic]|= 0x80;
            else if ((shotcount<300)||(shotcount>550))
                break;
            u8BitCount_panasonic++;
            //repeat flag select
            if(u8BitCount_panasonic == 8)
            {
                u8BitCount_panasonic = 0;
                u8DataIdx_panasonic++;
                if(u8DataIdx_panasonic ==6)
                {
                    u8Done_panasonic = 1;
                    return 0;
                }
                u8DataBits_panasonic[u8DataIdx_panasonic] = 0;
            }
            u8Status_panasonic = STATE_BIT_PULSE;
            return 0;
        default:
            break;
    }
     u8Status_panasonic = STATE_INACTIVE;
     return 0;
}
static BOOLEAN MDrv_IR_GetKeyCode_Panasonic(U8 *u8IRProtocol,U32 *u32IRHeadcode,U16 *u16IRKeycode,U8 *u8Repeat)
{
    U8 headcode0, headcode1, headcode2,command0,command1, verify;
    if (u8Done_panasonic)
    {
        u8Done_panasonic = 0;
        u8Status_panasonic = STATE_INACTIVE;
        headcode0 = u8DataBits_panasonic[0];
        headcode1 = u8DataBits_panasonic[1];
        headcode2 = u8DataBits_panasonic[2];
        command0 = u8DataBits_panasonic[3];
        command1 = u8DataBits_panasonic[4];
        verify = u8DataBits_panasonic[5];
        *u32IRHeadcode = headcode2<<16 |headcode1<<8 |headcode0;
        *u16IRKeycode = command1;
        *u8IRProtocol = E_IR_TYPE_PANASONIC;
        if((Get_Off_Time() < PANASONIC_REPEAT_TIME) &&(u8PrevKey_panasonic ==command1))
        {
            if(u8RepeatNum_panasonic < 2)
            {
                u8RepeatNum_panasonic++;
                Reset_Current_Time();
                return FALSE;
            }
            *u8Repeat = 1;
            Reset_Current_Time();
            return TRUE;
        }
        *u8Repeat = 0;
        u8PrevKey_panasonic = command1;
        u8RepeatNum_panasonic = 0;
        Reset_Current_Time();
        return TRUE;;
    }
    return FALSE;
}

//================== Standard Toshiba Timming Spec ===================//
#define TOSHIBA_NBITS           32
#define TOSHIBA_HEADER_PULSE    4500//us
#define TOSHIBA_HEADER_SPACE    4500
#define TOSHIBA_BIT_PULSE       560
#define TOSHIBA_BIT_0_SPACE     560
#define TOSHIBA_BIT_1_SPACE     1680
#define TOSHIBA_REPEAT_TIME     120  //ms

static U8 u8Status_toshiba = STATE_INACTIVE;
static U8 u8BitCount_toshiba = 0;
static U32 u32DataBits_toshiba = 0;
static U8 u8Done_toshiba = 0;
static U8 u8Repeat_toshiba = 0;
static U8 u8RepeatNum_toshiba = 0;
static U32 u32PrevData_toshiba = 0;
static U8 MDrv_IR_Decode_Toshiba(U16 shotcount, U8 flag)
{
    U32 time_temp;
    switch(u8Status_toshiba)
    {
        case STATE_INACTIVE:
            if(flag)
                break;
            u8BitCount_toshiba = 0;
            u32DataBits_toshiba = 0;
            u8Done_toshiba = 0;
            u8Repeat_toshiba = 0;
            if((shotcount> 3500)&&(shotcount<5400))
            {
                u8Status_toshiba = STATE_HEADER_SPACE;
                return 0;
            }
            else
                break;
        case STATE_HEADER_SPACE:
            if(!flag)
                break;
            if((shotcount>3500)&&(shotcount<5400))
            {
                u8Status_toshiba = STATE_BIT_PULSE;
                return 0;
            }
            break;
        case STATE_BIT_PULSE:
            if (flag)
                break;
            if((shotcount<400)||(shotcount>700))
                break;
            u8Status_toshiba = STATE_BIT_SPACE;
            return 0;
        case STATE_BIT_SPACE:
            if (!flag)
                break;
            u32DataBits_toshiba >>= 1;
            if ((shotcount>1300)&&(shotcount<2000))
                u32DataBits_toshiba |= 0x80000000;
            else if ((shotcount<400)||(shotcount>700))
                break;
            u8BitCount_toshiba++;
            //repeat flag select
            if(u8BitCount_toshiba == 1)
            {
                time_temp = Get_Off_Time();
                //if current time < timeout  set repeat flag
                if(Get_Off_Time() < TOSHIBA_REPEAT_TIME)
                {
                    u8Repeat_toshiba = 1;
                    u8Status_toshiba = STATE_INACTIVE;
                    Reset_Current_Time();
                    return 0;
                }
            }
            if (u8BitCount_toshiba == TOSHIBA_NBITS)
            {
                u8Done_toshiba = 1;
            }
            else
                u8Status_toshiba = STATE_BIT_PULSE;
            return 0;
        default:
            break;

    }
     u8Status_toshiba = STATE_INACTIVE;
     return 0;
}
static BOOLEAN MDrv_IR_GetKeyCode_Toshiba(U8 *u8IRProtocol,U32 *u32IRHeadcode,U16 *u16IRKeycode,U8 *u8Repeat)
{
    U8 u8Addr = 0;
    U8 u8AddrInv =0;
    U8 u8Cmd =0;
    U8 u8CmdInv =0;
    if (u8Repeat_toshiba)
    {
        u8Repeat_toshiba = 0;
        u8CmdInv  = (u32PrevData_toshiba >> 24) & 0xff;
        u8Cmd     = (u32PrevData_toshiba >> 16) & 0xff;
        u8AddrInv = (u32PrevData_toshiba >>  8) & 0xff;
        u8Addr    = (u32PrevData_toshiba >>  0) & 0xff;
        *u8IRProtocol = E_IR_TYPE_TOSHIBA;
        *u32IRHeadcode = u8Addr <<8 |u8AddrInv;
        *u16IRKeycode = u8Cmd;
        *u8Repeat = 1;
        Reset_Current_Time();
        return TRUE;
    }
    else if (u8Done_toshiba)
    {
        u8Done_toshiba = 0;
        u8CmdInv  = (u32DataBits_toshiba >> 24) & 0xff;
        u8Cmd     = (u32DataBits_toshiba >> 16) & 0xff;
        u8AddrInv = (u32DataBits_toshiba >>  8) & 0xff;
        u8Addr    = (u32DataBits_toshiba >>  0) & 0xff;
        u32PrevData_toshiba = u32DataBits_toshiba;

        *u8IRProtocol = E_IR_TYPE_TOSHIBA;
        *u32IRHeadcode = u8Addr <<8 |u8AddrInv;
        *u16IRKeycode = u8Cmd;
        *u8Repeat = 0;
        u8RepeatNum_toshiba = 0;
        u8Status_toshiba = STATE_INACTIVE;
        Reset_Current_Time();
        return TRUE;
    }
    return FALSE;
}
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
static U8 drvIR_SW_ReadByte( U16 u16RegIndex )
{
    return (U8)*((unsigned char volatile *) RIU_MAP + ((u16RegIndex << 1)-(u16RegIndex & 1)));
}

//*************************************************************************
//Function name:    drvIR_SW_WriteByte
//Passing parameter: U16 u16RegIndex:   register address
//                   U8  u8Value:       value
//Return parameter:     none
//Description: write 2 register values
//*************************************************************************
static void drvIR_SW_WriteByte(U16 u16RegIndex, U8 u8Value)
{
    *((unsigned char volatile *) RIU_MAP + ((u16RegIndex << 1)-(u16RegIndex & 1))) = u8Value;
}

//*************************************************************************
//Function name:    drvIR_SW_Write2Byte
//Passing parameter: U16 u16RegIndex:   register address
//                   U16 u16Value:      value
//Return parameter:     none
//Description: write 2 register values
//*************************************************************************
static void drvIR_SW_Write2Byte( U16 u16RegIndex, U16 u16Value )
{
    *((unsigned short volatile *) RIU_MAP + u16RegIndex) = u16Value;
}

static void drvIR_SW_Clear_FIFO(void)
{
    U8 i;
    for ( i = 0; i < IR_FIFO_MAX; i++ ) // fifi sie max 16
    {
        U8 garbage;
        if ( drvIR_SW_ReadByte(REG_IR_FIFO_EMPTY) & _IR_FIFO_EMPTY )
        {
            break;
        }
        garbage = drvIR_SW_ReadByte(REG_IR_KEY_DATA);
        drvIR_SW_WriteByte(REG_IR_FIFO_READ_PULSE, drvIR_SW_ReadByte(REG_IR_FIFO_READ_PULSE) | 0x01);
        garbage = drvIR_SW_ReadByte(REG_IR_FIFO_EMPTY);
    }
}
static BOOLEAN drvIR_SW_GetKeyCode(U8 *u8IRProtocol,U32 *u32IRHeadcode,U16 *u16IRKeycode,U8 *u8RepeatFlag)
{
    BOOLEAN ret = FALSE;
    ret = MDrv_IR_GetKeyCode_NEC(u8IRProtocol,u32IRHeadcode,u16IRKeycode,u8RepeatFlag);
    if(ret ==TRUE)
    {
        IR_INFO("[NEC] u16IRKeycode = %x ",*u16IRKeycode);
        IR_INFO("u8RepeatFlag = %d\n",*u8RepeatFlag);
        return TRUE;
    }
    ret = MDrv_IR_GetKeyCode_RC5(u8IRProtocol,u32IRHeadcode,u16IRKeycode,u8RepeatFlag);
    if(ret ==TRUE)
    {
        IR_INFO("[RC5] u16IRKeycode = %x ",*u16IRKeycode);
        IR_INFO("u8RepeatFlag = %d\n",*u8RepeatFlag);
        return TRUE;
    }
    ret = MDrv_IR_GetKeyCode_RC6(u8IRProtocol,u32IRHeadcode,u16IRKeycode,u8RepeatFlag);
    if(ret ==TRUE)
    {
        IR_INFO("[RC6] u16IRKeycode = %x ",*u16IRKeycode);
        IR_INFO("u8RepeatFlag = %d\n",*u8RepeatFlag);
        return TRUE;
    }
    ret = MDrv_IR_GetKeyCode_RCA(u8IRProtocol,u32IRHeadcode,u16IRKeycode,u8RepeatFlag);
    if(ret ==TRUE)
    {
        IR_INFO("[RCA] u16IRKeycode = %x ",*u16IRKeycode);
        IR_INFO("u8RepeatFlag = %d\n",*u8RepeatFlag);
        return TRUE;
    }
    ret = MDrv_IR_GetKeyCode_Panasonic(u8IRProtocol,u32IRHeadcode,u16IRKeycode,u8RepeatFlag);
    if(ret ==TRUE)
    {
        IR_INFO("[Panasonic] u16IRKeycode = %x ",*u16IRKeycode);
        IR_INFO("u8RepeatFlag = %d\n",*u8RepeatFlag);
        return TRUE;
    }
    ret = MDrv_IR_GetKeyCode_Toshiba(u8IRProtocol,u32IRHeadcode,u16IRKeycode,u8RepeatFlag);
    if(ret ==TRUE)
    {
        IR_INFO("[Toshiba] u16IRKeycode = %x ",*u16IRKeycode);
        IR_INFO("u8RepeatFlag = %d\n",*u8RepeatFlag);
        return TRUE;
    }
    return FALSE;
}

static void drvIR_SW_ParseKey(MHAL_SavedRegisters *pHalReg, U32 u32Data)
{
    U8 IRProtocol = 0;
    U32 IRHeadcode = 0;
    U16 IRKeycode = 0;
    U8 RepeatFlag = 0;
    U16 shotcount[IR_FIFO_MAX];
    U8 pnflag[IR_FIFO_MAX];
    U8 count = 0;
    U8 i;
    do{
        shotcount[count] = drvIR_SW_ReadByte(REG_IR_SHOT_CNT0)|(drvIR_SW_ReadByte(REG_IR_SHOT_CNT1)<<8);
        pnflag[count] = (drvIR_SW_ReadByte(REG_IR_SHOT_CNT2)&_IR_SHOT_P)?1:0;
        if(last_pnflag == pnflag[count]) //
        {
            return ;
        }
        last_pnflag = pnflag[count];
        drvIR_SW_WriteByte(0x3DB0, drvIR_SW_ReadByte(REG_IR_FIFO_READ_PULSE) | _IR_FIFO_RD_PULSE); //Enable reg_ir_sw_fifo_en
        count ++;
        if(count>=IR_FIFO_MAX)
        {
            break;
        }
    }while((drvIR_SW_ReadByte(REG_IR_FIFO_EMPTY)&_IR_FIFO_EMPTY)!= _IR_FIFO_EMPTY);
#if DEBUG_IR
    if(count_prev < count)
        count_prev = count;
    drvIR_SW_WriteByte(REG_IR_CCODE2, count_prev); //dummy register record fifo max datasize
#endif
    for(i= 0;i<count;i++)
    {
        MDrv_IR_Decode_NEC(shotcount[i],pnflag[i]);
        MDrv_IR_Decode_RC5(shotcount[i],pnflag[i]);
        MDrv_IR_Decode_RC6(shotcount[i],pnflag[i]);
        MDrv_IR_Decode_RCA(shotcount[i],pnflag[i]);
        MDrv_IR_Decode_Panasonic(shotcount[i],pnflag[i]);
        MDrv_IR_Decode_Toshiba(shotcount[i],pnflag[i]);
    }
    if(drvIR_SW_GetKeyCode(&IRProtocol,&IRHeadcode,&IRKeycode,&RepeatFlag))
    {
        u8GetKeyFlag = 1;
        u8Protocol = IRProtocol;
        u32Headcode = IRHeadcode;
        u16Keycode = IRKeycode;
        u8Repeat = RepeatFlag;
    }
    MsOS_EnableInterrupt(E_INT_FIQ_IR);
    return;
}

BOOLEAN drv_IR_SW_GetKeyCode(U8 *u8IRProtocol,U32 *u32IRHeadcode,U16 *u16IRKeycode,U8 *u8RepeatFlag)
{
    if(u8GetKeyFlag)
    {
        u8GetKeyFlag = 0;
        *u8IRProtocol = u8Protocol;
        *u32IRHeadcode = u32Headcode;
        *u16IRKeycode = u16Keycode ;
        *u8RepeatFlag = u8Repeat;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
//------------------------------------------------------------------------------
/// @brief sw decoder mode HW register Initialize. [Do not changes]
/// @return null: NULL.
/// @return null: NULL.
//------------------------------------------------------------------------------
void drv_IR_SW_Initialize(void)
{
    //------------------- sw cponfig ------------------//
    IR_INFO("[Mboot] ===== sw IR Config Register ===== \n");
    //1. set ctrl enable IR
    drvIR_SW_Write2Byte(REG_IR_CTRL0, 0x01BF);
    //2.set SW decode mode & Glitch Remove (for prevent Abnormal waves)  reg_ir_glhrm_num = 4£¬reg_ir_glhrm_en = 1
    drvIR_SW_Write2Byte(REG_IR_GLHRM_NUM_L, 0x1804);
    //3.set PN shot interrupt
    drvIR_SW_WriteByte(REG_IR_FIFO_CTRL, drvIR_SW_ReadByte(REG_IR_FIFO_CTRL)|0x30);
    //4.IR SW FIFO enable
    drvIR_SW_WriteByte(REG_IR_FIFO_CTRL, drvIR_SW_ReadByte(REG_IR_FIFO_CTRL) | (0x1 <<6)); //Enable reg_ir_sw_fifo_en
    drvIR_SW_WriteByte(REG_IR_CKDIV_NUM,0xCF);//set DIV
    drvIR_SW_WriteByte(REG_IR_SEPR_BIT, drvIR_SW_ReadByte(REG_IR_SEPR_BIT) | (0x1 <<6)); //Disable :ir_shot_cnt to have sample rate 1MHz
    drvIR_SW_Clear_FIFO();

    IR_INFO("[Mboot] ===== Attach IR Interrupt  ======\n");
    MsOS_AttachInterrupt(E_INT_FIQ_IR, (InterruptCb)drvIR_SW_ParseKey);
    MsOS_EnableInterrupt(E_INT_FIQ_IR);
    return ;
}
#undef DRV_SWIR_C
