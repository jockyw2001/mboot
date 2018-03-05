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

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <command.h>
#include <common.h>
#include <linux/ctype.h>
#include <MsTypes.h>
#include <MsUartComm.h>
#include <malloc.h>
#include <stdlib.h>
#include "drvUART.h"
#include <MsIRQ.h>

#ifdef CONFIG_UARTCOMM

#define BUFFER_MAX 50
#define DEFAULT_BAUDRATE 115200

static MS_U32 uart_comm=0;

//use cmd
/*static void uartcomm_cmduse(void)
{
#ifdef CONFIG_UARTCOMM
    run_command("uartcomm init 1 1 115200",0);
    run_command("uartcomm write -s hello uartcomm",0);
    char buffer[60]={0};
    memset(buffer,51,sizeof(buffer));
    char cmd[50]="uartcomm write -x";
    sprintf(cmd,"%s %x %d",cmd,buffer,sizeof(buffer));
    printf("cmd :%s\n",cmd);
    run_command(cmd,0);
#endif
}*/

//add customer flow to manage buffer data.
static void uartcomm_cus(MS_U8 *buffer)
{
   /* if(buffer!= NULL)
    {
        printf("%s",buffer);
    }*/
    return ;
}

static void uartcomm_showusage(void)
{
    printf("uartcomm usage:\n");
    printf("uartcomm init <port> <pad> <baudrate>: init uart port,<port>--1,2,3,4 <pad> 0,1,2,3\n");
    printf("     eg:uartcomm init 1 1 115200 ==init uart_port UART1 with GPIO PAD=PAD_GPIO7,Baudrate=115200\n");
    printf("     for monaco:UART1_PAD:0=not enabled,1=PAD_GPIO7/PAD_GPIO8,2=PAD_GPIO20/PAD_GPIO19,3=PAD_GPIO4/PAD_GPIO3\n");
    printf("uartcomm write <-s> <cmd data> : uart write data <step 1--uartcomm init success,step2--uart write data>\n");
    printf("     -s: write string to uart,default cmd_size <= 30\n");
    printf("uartcomm read  <size>     : uart read data <tep 1--uartcomm init success,step2--uart read data>\n");
}

static char * nstrncpy(char * dest,const char *src,size_t index,size_t count)
{
	char *tmp = dest;
    while(index -- && *src++ != '\0')
        /* nothing */;
	while (count-- && (*dest++ = *src++) != '\0')
		/* nothing */;

	return tmp;
}
int do_uartcomm (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
        MS_U8 UartPortNum = 0;
        MS_U8 UartPadNum = 0;
        MS_U32 Baudrate = 0;
        UART_DEVICE_TYPE UartDevice = E_UART_INVALID;
        UART_PORT_TYPE UartPort = E_UART_PORT_INVALID;
        UART_PAD_TYPE UartPad = E_UART_PAD_1_0;
        MS_U8 buffer[BUFFER_MAX]={0};
        MS_U32 addr = 0;
        MS_U8* pdata =NULL;
        MS_U32 size = 0;
        MS_U8 i = 0;

        if(!strcmp(argv[1], "init"))
        {
            if(argc != 5)
            {
                uartcomm_showusage();
                return -1;
            }
            UartPortNum = simple_strtoul(argv[2], NULL, 10);
            UartPadNum = simple_strtoul(argv[3], NULL, 10);
            Baudrate = simple_strtoul(argv[4], NULL, 10);

            switch(UartPortNum)
            {
                case 1: UartPort = E_UART_PORT1;
                        UartPad = UartPadNum + E_UART_PAD_1_0;
                        UartDevice = E_UART_PIU_UART1;
                        break;
                case 2: UartPort = E_UART_PORT2;
                        UartPad = UartPadNum + E_UART_PAD_2_0;
                        UartDevice = E_UART_PIU_UART2;
                        break;
                case 3: UartPort = E_UART_PORT3;
                        UartPad = UartPadNum + E_UART_PAD_3_0;
                        UartDevice = E_UART_PIU_UART1;//uart source choose PIU_UART1 while PIU_UART1 not used
                        break;
                case 4: UartPort = E_UART_PORT4;
                        UartPad = UartPadNum + E_UART_PAD_4_0;
                        UartDevice = E_UART_PIU_UART2;//uart source choose PIU_UART2 while PIU_UART2 not used
                        break;
                default: UartPort = E_UART_PORT_INVALID;
                        printf("UART PORT select error\n");
                        break;
            }

            MDrv_UART_Init(E_UART_PIU_UART1,DEFAULT_BAUDRATE);
            if(UartDevice == E_UART_INVALID || UartPort ==E_UART_PORT_INVALID)
            {
                printf("uartcomm init failed ! uartsource or pad not avaliable.\n");
                return -1;
            }
            mdrv_uart_connect_mux(UartDevice,UartPort,UartPad);
            uart_comm = mdrv_uart_open(UartDevice);

            if (uart_comm)
            {
                mdrv_uart_set_baudrate(uart_comm,Baudrate);
                printf("uartcomm init port %d ok\n",UartPort);
            }
            else
            {
                printf("uartcomm init failed !\n");
                return -1;
            }
            return 0;
        }
        else if(!strcmp(argv[1], "read"))
        {
            if(argc != 3)
            {
                uartcomm_showusage();
                return -1;
            }
            size = simple_strtoul(argv[2], NULL, 10);
            if(uart_comm)
            {
                while(size)
                {
                    memset(buffer,0,BUFFER_MAX);

                    if(size > BUFFER_MAX)
                    {
                        mdrv_uart_read(uart_comm,buffer,BUFFER_MAX);
                        size = size-BUFFER_MAX;
                    }
                    else
                    {
                        mdrv_uart_read(uart_comm,buffer,size);
                        size =0;
                    }

                    uartcomm_cus(buffer);
                }
            }
            else
            {
                printf("please uartcomm init seccessed first\n");
                return -1;
            }
            return 0;
        }
        else if(!strcmp(argv[1],"write"))
        {
            if(argc > CONFIG_SYS_MAXARGS)//uboot cmd args limited
            {
                printf("args is too long!\n");
                return -1;
            }
            if(uart_comm)
            {
                MS_U8 n = 0;
                memset(buffer,0,BUFFER_MAX);
                if(!strcmp(argv[2], "-s"))//string
                {
                    i = 3;
                    while(argv[i])
                    {
                        size = strlen(argv[i]);

                        while(size > BUFFER_MAX)
                        {
                            nstrncpy(buffer,argv[i],n*BUFFER_MAX,BUFFER_MAX);
                            mdrv_uart_write(uart_comm,buffer,BUFFER_MAX);
                            memset(buffer,0,BUFFER_MAX);
                            size = size - BUFFER_MAX;
                            n++;
                        }

                        nstrncpy(buffer,argv[i],n*BUFFER_MAX,size);
                        mdrv_uart_write(uart_comm,buffer,size+1);
                        memset(buffer,0,BUFFER_MAX);
                        i++;
                    }
                }
                else if(!strcmp(argv[2], "-a"))//address
                {
                    if(argc != 5)
                    {
                        uartcomm_showusage();
                        return -1;
                    }

                    addr = simple_strtoul(argv[3], NULL, 16);
                    size = simple_strtoul(argv[4], NULL, 10);
                    pdata = (MS_U8*)addr;

                    while(size>BUFFER_MAX)
                    {
                        memcpy(buffer,pdata+(n*BUFFER_MAX),BUFFER_MAX);
                        mdrv_uart_write(uart_comm,buffer,size);
                        size = size - BUFFER_MAX;
                        n++;
                    }

                    memcpy(buffer,pdata+(n*BUFFER_MAX),size);
                    mdrv_uart_write(uart_comm,buffer,size);
                }
                else
                {
                    while(argv[i])
                    {
                        buffer[i]= simple_strtoul(argv[i], NULL, 10);
                        i++;
                        size++;
                    }

                    mdrv_uart_write(uart_comm,buffer,size);
                }
            }
            else
            {
                printf("please uartcomm init seccessed first\n");
                return -1;
            }
            return 0;
        }
        else if(!strcmp(argv[1],"test"))
        {
            MDrv_UART_Init(E_UART_PIU_UART1,DEFAULT_BAUDRATE);

            uart_comm = mdrv_uart_open(E_UART_PIU_UART1);

            if (uart_comm)
            {
                mdrv_uart_set_baudrate(uart_comm,DEFAULT_BAUDRATE);
            }
            else
            {
                printf("uartcomm open failed !\n");
                return -1;
            }
            mdrv_uart_write(uart_comm,"hello",5);
            mdrv_uart_read(uart_comm,buffer,10);
            printf("uart receive data :%s",buffer);
        }
        else
        {
            uartcomm_showusage();
        }
        return 0;
}
#endif
