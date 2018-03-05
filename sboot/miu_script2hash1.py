import sys
import os
import re
from os.path import join
from subprocess import Popen, PIPE
import hashlib
import struct
import array
from struct import pack, unpack

MIU_HEADER_OUT = 'test.h'

def main(argv):
    miu_script_target = argv[1]
    miu_base_addr = argv[2]
    miu_script_source = miu_script_target.replace('.h','.txt')
    file = open(miu_script_source)
    cmd_base = '0x10'
    cmd_base_8bits = '0xFF'
    AC_only = 0
    file_out = '//<MStar Software>\n\
//******************************************************************************\n\
// MStar Software\n\
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.\n\
// All software, firmware and related documentation herein ("MStar Software") are\n\
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by\n\
// law, including, but not limited to, copyright law and international treaties.\n\
// Any use, modification, reproduction, retransmission, or republication of all\n\
// or part of MStar Software is expressly prohibited, unless prior written\n\
// permission has been granted by MStar.\n\
//\n\
// By accessing, browsing and/or using MStar Software, you acknowledge that you\n\
// have read, understood, and agree, to be bound by below terms ("Terms") and to\n\
// comply with all applicable laws and regulations:\n\
//\n\
// 1. MStar shall retain any and all right, ownership and interest to MStar\n\
//    Software and any modification/derivatives thereof.\n\
//    No right, ownership, or interest to MStar Software and any\n\
//    modification/derivatives thereof is transferred to you under Terms.\n\
//\n\
// 2. You understand that MStar Software might include, incorporate or be\n\
//    supplied together with third party`s software and the use of MStar\n\
//    Software may require additional licenses from third parties.\n\
//    Therefore, you hereby agree it is your sole responsibility to separately\n\
//    obtain any and all third party right and license necessary for your use of\n\
//    such third party`s software.\n\
//\n\
// 3. MStar Software and any modification/derivatives thereof shall be deemed as\n\
//    MStar`s confidential information and you agree to keep MStar`s\n\
//    confidential information in strictest confidence and not disclose to any\n\
//    third party.\n\
//\n\
// 4. MStar Software is provided on an "AS IS" basis without warranties of any\n\
//    kind. Any warranties are hereby expressly disclaimed by MStar, including\n\
//    without limitation, any warranties of merchantability, non-infringement of\n\
//    intellectual property rights, fitness for a particular purpose, error free\n\
//    and in conformity with any international standard.  You agree to waive any\n\
//    claim against MStar for any loss, damage, cost or expense that you may\n\
//    incur related to your use of MStar Software.\n\
//    In no event shall MStar be liable for any direct, indirect, incidental or\n\
//    consequential damages, including without limitation, lost of profit or\n\
//    revenues, lost or damage of data, and unauthorized system use.\n\
//    You agree that this Section 4 shall still apply without being affected\n\
//    even if MStar Software has been modified by MStar in accordance with your\n\
//    request or instruction for your use, except otherwise agreed by both\n\
//    parties in writing.\n\
//\n\
// 5. If requested, MStar may from time to time provide technical supports or\n\
//    services in relation with MStar Software to you for your use of\n\
//    MStar Software in conjunction with your or your customer`s product\n\
//    ("Services").\n\
//    You understand and agree that, except otherwise agreed by both parties in\n\
//    writing, Services are provided on an "AS IS" basis and the warranty\n\
//    disclaimer set forth in Section 4 above shall apply.\n\
//\n\
// 6. Nothing contained herein shall be construed as by implication, estoppels\n\
//    or otherwise:\n\
//    (a) conferring any license or right to use MStar name, trademark, service\n\
//        mark, symbol or any other identification;\n\
//    (b) obligating MStar or any of its affiliates to furnish any person,\n\
//        including without limitation, you and your customers, any assistance\n\
//        of any kind whatsoever, or any information; or\n\
//    (c) conferring any license or right under any intellectual property right.\n\
//\n\
// 7. These terms shall be governed by and construed in accordance with the laws\n\
//    of Taiwan, R.O.C., excluding its conflict of law rules.\n\
//    Any and all dispute arising out hereof or related hereto shall be finally\n\
//    settled by arbitration referred to the Chinese Arbitration Association,\n\
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration\n\
//    Rules of the Association by three (3) arbitrators appointed in accordance\n\
//    with the said Rules.\n\
//    The place of arbitration shall be in Taipei, Taiwan and the language shall\n\
//    be English.\n\
//    The arbitration award shall be final and binding to both parties.\n\
//\n\
//******************************************************************************\n\
//<MStar Software>\n'
    file_out += '#define _RV32_1(addr, value)    {addr, (((value) & 0xff) << 8) | 0xff}\n'
    file_out += '#define _RV32_1_MASK(addr, mask, value)    {addr, (((value) & 0xff) << 8) | mask}\n'
    file_out += '#define _RV32_2(a,b)    {a<<1,b}\n\
{CHIPCFG_CMD_BASE, '
    file_out += miu_base_addr
    file_out += "20},\n"

    while True :
            file_string = file.readline()
            if file_string =='': break
            #print(file_string)
            str_array = file_string.split()
            #print(str_array)

            if(len(str_array) > 1):
                if str_array[0] == 'wait' :
                    if (len(str_array) > 2) and (str_array[2] == '//DC:') :
                        if (len(str_array) > 3) and (str_array[3] == 'wait'):
                            if AC_only == 0:
                                file_out += "{CHIPCFG_STR_IFAC,"
                                file_out += "0x0"
                                file_out += "},\n"
							
                            file_out += "{CHIPCFG_CMD_WAIT,"
                            file_out += str_array[1]
                            file_out += "},\n"

                            file_out += "{CHIPCFG_STR_IFAC_END,"
                            file_out += "0x0"
                            file_out += "},\n"
							
                            file_out += "{CHIPCFG_STR_IFDC,"
                            file_out += "0x0"
                            file_out += "},\n"
							
                            file_out += "{CHIPCFG_CMD_WAIT,"
                            file_out += str_array[4]
                            file_out += "},\n"

                            file_out += "{CHIPCFG_STR_IFDC_END,"
                            file_out += "0x0"
                            file_out += "},\n"

                            AC_only = 0
							
                        else:
                            if AC_only == 0:
                                file_out += "{CHIPCFG_STR_IFAC,"
                                file_out += "0x0"
                                file_out += "},\n"
								
                            AC_only = 1	
							
                            file_out += "{CHIPCFG_CMD_WAIT,"
                            file_out += str_array[1]
                            file_out += "},\n"
														
                    else:
                        if AC_only == 1:
                            file_out += "{CHIPCFG_STR_IFAC_END,"
                            file_out += "0x0"
                            file_out += "},\n"
						
                        AC_only = 0	
                        
                        file_out += "{CHIPCFG_CMD_WAIT,"
                        file_out += str_array[1]
                        file_out += "},\n"
												
                elif str_array[0] == '//DC:' and str_array[1] == 'wait' :	
                        file_out += "{CHIPCFG_STR_IFDC,"
                        file_out += "0x0"
                        file_out += "},\n"
							
                        file_out += "{CHIPCFG_CMD_WAIT,"
                        file_out += str_array[2]
                        file_out += "},\n"

                        file_out += "{CHIPCFG_STR_IFDC_END,"
                        file_out += "0x0"
                        file_out += "},\n"
					
            if(len(str_array) > 2):
                if str_array[0] == 'wriu' and str_array[1][0:2]=='0x':
                    if (len(str_array) > 3) and (str_array[3] == '//DC:'):
                        if (len(str_array) > 4) and (str_array[4] == 'wriu'):						
                            if str_array[1][0:4]=='0x11' and cmd_base_8bits!='0x11':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "22},\n"
                                cmd_base_8bits = '0x11'
                            if str_array[1][0:4]=='0x10' and cmd_base_8bits!='0x10':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "20},\n"
                                cmd_base_8bits = '0x10'
                            if str_array[1][0:4]=='0x16' and cmd_base_8bits!='0x16':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "2c},\n"
                                cmd_base_8bits = '0x16'
                            if str_array[1][0:4]=='0x14' and cmd_base_8bits!='0x14':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr 	
                                file_out += "28},\n"
                                cmd_base_8bits = '0x14'
                            
                            if AC_only == 0:
                                file_out += "{CHIPCFG_STR_IFAC,"
                                file_out += "0x0"
                                file_out += "},\n"		
								
                            file_out += "_RV32_1("
                            if str_array[1][7] == '0' or str_array[1][7] == '2' or str_array[1][7] == '4' or str_array[1][7] == '6' or str_array[1][7] == '8' or str_array[1][7] == 'A' or str_array[1][7] == 'C' or str_array[1][7] == 'E' or str_array[1][7] == 'a' or str_array[1][7] == 'c' or str_array[1][7] == 'e':
                                file_out += "(0x"
                                file_out += str_array[1][4:8]
                                file_out += " <<1), "
            
                            if str_array[1][7] == '1' :
                                file_out += "((0x"
                                file_out += str_array[1][4:7]
                                file_out +=  "0 <<1) + 1), "
            
                            if str_array[1][7] == '3' :
                                file_out += "((0x"
                                file_out += str_array[1][4:7]
                                file_out +=  "2 <<1) + 1), "
            
                            if str_array[1][7] == '5' :
                                file_out += "((0x"
                                file_out += str_array[1][4:7]
                                file_out +=  "4 <<1) + 1), "
            
                            if str_array[1][7] == '7' :
                                file_out += "((0x"
                                file_out += str_array[1][4:7]
                                file_out +=  "6 <<1) + 1), "
            
                            if str_array[1][7] == '9' :
                                file_out += "((0x"
                                file_out += str_array[1][4:7]
                                file_out +=  "8 <<1) + 1), "
            
                            if str_array[1][7] == 'B' :
                                file_out += "((0x"
                                file_out += str_array[1][4:7]
                                file_out +=  "A <<1) + 1), "
            
                            if str_array[1][7] == 'D' :
                                file_out += "((0x"
                                file_out += str_array[1][4:7]
                                file_out +=  "C <<1) + 1), "
            
                            if str_array[1][7] == 'F' :
                                file_out += "((0x"
                                file_out += str_array[1][4:7]
                                file_out +=  "E <<1) + 1), "
								
                            file_out += str_array[2][0:4]
                            file_out += "),\n"
                            cmd_base = '0xFF'
							
                            file_out += "{CHIPCFG_STR_IFAC_END,"
                            file_out += "0x0"
                            file_out += "},\n"
							
                            if str_array[5][0:4]=='0x11' and cmd_base_8bits!='0x11':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "22},\n"
                                cmd_base_8bits = '0x11'
                            if str_array[5][0:4]=='0x10' and cmd_base_8bits!='0x10':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "20},\n"
                                cmd_base_8bits = '0x10'
                            if str_array[5][0:4]=='0x16' and cmd_base_8bits!='0x16':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "2c},\n"
                                cmd_base_8bits = '0x16'
                            if str_array[5][0:4]=='0x14' and cmd_base_8bits!='0x14':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "28},\n"
                                cmd_base_8bits = '0x14'

                            file_out += "{CHIPCFG_STR_IFDC,"
                            file_out += "0x0"
                            file_out += "},\n"		
								
                            file_out += "_RV32_1("
                            if str_array[5][7] == '0' or str_array[5][7] == '2' or str_array[5][7] == '4' or str_array[5][7] == '6' or str_array[5][7] == '8' or str_array[5][7] == 'A' or str_array[5][7] == 'C' or str_array[5][7] == 'E' or str_array[5][7] == 'a' or str_array[5][7] == 'c' or str_array[5][7] == 'e' :
                                file_out += "(0x"
                                file_out += str_array[5][4:8]
                                file_out += " <<1), "
            
                            if str_array[5][7] == '1' :
                                file_out += "((0x"
                                file_out += str_array[5][4:7]
                                file_out +=  "0 <<1) + 1), "
            
                            if str_array[5][7] == '3' :
                                file_out += "((0x"
                                file_out += str_array[5][4:7]
                                file_out +=  "2 <<1) + 1), "
            
                            if str_array[5][7] == '5' :
                                file_out += "((0x"
                                file_out += str_array[5][4:7]
                                file_out +=  "4 <<1) + 1), "
            
                            if str_array[5][7] == '7' :
                                file_out += "((0x"
                                file_out += str_array[5][4:7]
                                file_out +=  "6 <<1) + 1), "
            
                            if str_array[5][7] == '9' :
                                file_out += "((0x"
                                file_out += str_array[5][4:7]
                                file_out +=  "8 <<1) + 1), "
            
                            if str_array[5][7] == 'B' :
                                file_out += "((0x"
                                file_out += str_array[5][4:7]
                                file_out +=  "A <<1) + 1), "
            
                            if str_array[5][7] == 'D' :
                                file_out += "((0x"
                                file_out += str_array[5][4:7]
                                file_out +=  "C <<1) + 1), "
            
                            if str_array[5][7] == 'F' :
                                file_out += "((0x"
                                file_out += str_array[5][4:7]
                                file_out +=  "E <<1) + 1), "
            
                            file_out += str_array[6][0:4]
                            file_out += "),\n"
                            cmd_base = '0xFF'
							
                            file_out += "{CHIPCFG_STR_IFDC_END,"
                            file_out += "0x0"
                            file_out += "},\n"

                            AC_only = 0
							
                        else:
                            if str_array[1][0:4]=='0x11' and cmd_base_8bits!='0x11':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "22},\n"
                                cmd_base_8bits = '0x11'
                            if str_array[1][0:4]=='0x10' and cmd_base_8bits!='0x10':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "20},\n"
                                cmd_base_8bits = '0x10'
                            if str_array[1][0:4]=='0x16' and cmd_base_8bits!='0x16':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "2c},\n"
                                cmd_base_8bits = '0x16'
                            if str_array[1][0:4]=='0x14' and cmd_base_8bits!='0x14':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "28},\n"
                                cmd_base_8bits = '0x14'
                            
                            if AC_only == 0:
                                file_out += "{CHIPCFG_STR_IFAC,"
                                file_out += "0x0"
                                file_out += "},\n"		

                            AC_only = 1
							
                            file_out += "_RV32_1("
                            if str_array[1][7] == '0' or str_array[1][7] == '2' or str_array[1][7] == '4' or str_array[1][7] == '6' or str_array[1][7] == '8' or str_array[1][7] == 'A' or str_array[1][7] == 'C' or str_array[1][7] == 'E' or str_array[1][7] == 'a' or str_array[1][7] == 'c' or str_array[1][7] == 'e' :
                                file_out += "(0x"
                                file_out += str_array[1][4:8]
                                file_out += " <<1), "
            
                            if str_array[1][7] == '1' :
                                file_out += "((0x"
                                file_out += str_array[1][4:7]
                                file_out +=  "0 <<1) + 1), "
            
                            if str_array[1][7] == '3' :
                                file_out += "((0x"
                                file_out += str_array[1][4:7]
                                file_out +=  "2 <<1) + 1), "
            
                            if str_array[1][7] == '5' :
                                file_out += "((0x"
                                file_out += str_array[1][4:7]
                                file_out +=  "4 <<1) + 1), "
            
                            if str_array[1][7] == '7' :
                                file_out += "((0x"
                                file_out += str_array[1][4:7]
                                file_out +=  "6 <<1) + 1), "
            
                            if str_array[1][7] == '9' :
                                file_out += "((0x"
                                file_out += str_array[1][4:7]
                                file_out +=  "8 <<1) + 1), "
            
                            if str_array[1][7] == 'B' :
                                file_out += "((0x"
                                file_out += str_array[1][4:7]
                                file_out +=  "A <<1) + 1), "
            
                            if str_array[1][7] == 'D' :
                                file_out += "((0x"
                                file_out += str_array[1][4:7]
                                file_out +=  "C <<1) + 1), "
            
                            if str_array[1][7] == 'F' :
                                file_out += "((0x"
                                file_out += str_array[1][4:7]
                                file_out +=  "E <<1) + 1), "
            
                            file_out += str_array[2][0:4]
                            file_out += "),\n"
                            cmd_base = '0xFF'
							
                    else:
                        if AC_only == 1:
                            file_out += "{CHIPCFG_STR_IFAC_END,"
                            file_out += "0x0"
                            file_out += "},\n"
							
                        AC_only = 0
							
                        if str_array[1][0:4]=='0x11' and cmd_base_8bits!='0x11':
                            file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                            file_out += miu_base_addr
                            file_out += "22},\n"
                            cmd_base_8bits = '0x11'
                        if str_array[1][0:4]=='0x10' and cmd_base_8bits!='0x10':
                            file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                            file_out += miu_base_addr
                            file_out += "20},\n"
                            cmd_base_8bits = '0x10'
                        if str_array[1][0:4]=='0x16' and cmd_base_8bits!='0x16':
                            file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                            file_out += miu_base_addr
                            file_out += "2c},\n"
                            cmd_base_8bits = '0x16'
                        if str_array[1][0:4]=='0x14' and cmd_base_8bits!='0x14':
                            file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                            file_out += miu_base_addr
                            file_out += "28},\n"
                            cmd_base_8bits = '0x14'
							
                        file_out += "_RV32_1("
                        if str_array[1][7] == '0' or str_array[1][7] == '2' or str_array[1][7] == '4' or str_array[1][7] == '6' or str_array[1][7] == '8' or str_array[1][7] == 'A' or str_array[1][7] == 'C' or str_array[1][7] == 'E' or str_array[1][7] == 'a' or str_array[1][7] == 'c' or str_array[1][7] == 'e' :
                            file_out += "(0x"
                            file_out += str_array[1][4:8]
                            file_out += " <<1), "
        
                        if str_array[1][7] == '1' :
                            file_out += "((0x"
                            file_out += str_array[1][4:7]
                            file_out +=  "0 <<1) + 1), "
        
                        if str_array[1][7] == '3' :
                            file_out += "((0x"
                            file_out += str_array[1][4:7]
                            file_out +=  "2 <<1) + 1), "
        
                        if str_array[1][7] == '5' :
                            file_out += "((0x"
                            file_out += str_array[1][4:7]
                            file_out +=  "4 <<1) + 1), "
        
                        if str_array[1][7] == '7' :
                            file_out += "((0x"
                            file_out += str_array[1][4:7]
                            file_out +=  "6 <<1) + 1), "
        
                        if str_array[1][7] == '9' :
                            file_out += "((0x"
                            file_out += str_array[1][4:7]
                            file_out +=  "8 <<1) + 1), "
        
                        if str_array[1][7] == 'B' :
                            file_out += "((0x"
                            file_out += str_array[1][4:7]
                            file_out +=  "A <<1) + 1), "
        
                        if str_array[1][7] == 'D' :
                            file_out += "((0x"
                            file_out += str_array[1][4:7]
                            file_out +=  "C <<1) + 1), "
        
                        if str_array[1][7] == 'F' :
                            file_out += "((0x"
                            file_out += str_array[1][4:7]
                            file_out +=  "E <<1) + 1), "
        
                        file_out += str_array[2][0:4]
                        file_out += "),\n"
                        cmd_base = '0xFF'
						
                elif str_array[0] == '//DC:' and str_array[1] == 'wriu' and str_array[2][0:2]=='0x' :
                    if str_array[2][0:4]=='0x11' and cmd_base_8bits!='0x11':
                        file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                        file_out += miu_base_addr
                        file_out += "22},\n"
                        cmd_base_8bits = '0x11'
                    if str_array[2][0:4]=='0x10' and cmd_base_8bits!='0x10':
                        file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                        file_out += miu_base_addr
                        file_out += "20},\n"
                        cmd_base_8bits = '0x10'
                    if str_array[2][0:4]=='0x16' and cmd_base_8bits!='0x16':
                        file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                        file_out += miu_base_addr
                        file_out += "2c},\n"
                        cmd_base_8bits = '0x16'
                    if str_array[2][0:4]=='0x14' and cmd_base_8bits!='0x14':
                        file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                        file_out += miu_base_addr
                        file_out += "28},\n"
                        cmd_base_8bits = '0x14'

                    file_out += "{CHIPCFG_STR_IFDC,"
                    file_out += "0x0"
                    file_out += "},\n"		
						
                    file_out += "_RV32_1("
                    if str_array[2][7] == '0' or str_array[2][7] == '2' or str_array[2][7] == '4' or str_array[2][7] == '6' or str_array[2][7] == '8' or str_array[2][7] == 'A' or str_array[2][7] == 'C' or str_array[2][7] == 'E' or str_array[2][7] == 'a' or str_array[2][7] == 'c' or str_array[2][7] == 'e' :
                        file_out += "(0x"
                        file_out += str_array[2][4:8]
                        file_out += " <<1), "
    
                    if str_array[2][7] == '1' :
                        file_out += "((0x"
                        file_out += str_array[2][4:7]
                        file_out +=  "0 <<1) + 1), "
    
                    if str_array[2][7] == '3' :
                        file_out += "((0x"
                        file_out += str_array[2][4:7]
                        file_out +=  "2 <<1) + 1), "
    
                    if str_array[2][7] == '5' :
                        file_out += "((0x"
                        file_out += str_array[2][4:7]
                        file_out +=  "4 <<1) + 1), "
    
                    if str_array[2][7] == '7' :
                        file_out += "((0x"
                        file_out += str_array[2][4:7]
                        file_out +=  "6 <<1) + 1), "
    
                    if str_array[2][7] == '9' :
                        file_out += "((0x"
                        file_out += str_array[2][4:7]
                        file_out +=  "8 <<1) + 1), "
    
                    if str_array[2][7] == 'B' :
                        file_out += "((0x"
                        file_out += str_array[2][4:7]
                        file_out +=  "A <<1) + 1), "
    
                    if str_array[2][7] == 'D' :
                        file_out += "((0x"
                        file_out += str_array[2][4:7]
                        file_out +=  "C <<1) + 1), "
    
                    if str_array[2][7] == 'F' :
                        file_out += "((0x"
                        file_out += str_array[2][4:7]
                        file_out +=  "E <<1) + 1), "

                    file_out += str_array[3][0:4]
                    file_out += "),\n"
                    cmd_base = '0xFF'
					
                    file_out += "{CHIPCFG_STR_IFDC_END,"
                    file_out += "0x0"
                    file_out += "},\n"
						
            if(len(str_array) > 3):					
                if str_array[0] == 'wriu' and str_array[1] == '-w' :
                    if (len(str_array) > 4) and (str_array[4] == '//DC:') :
                        if (len(str_array) > 5) and (str_array[5] == 'wriu') and (str_array[6] == '-w'):
    						if str_array[2][0:4]=='0x11' and cmd_base!='0x11' :
    							file_out += "{CHIPCFG_CMD_BASE, "
    							file_out += miu_base_addr
    							file_out += "22},\n"
    							cmd_base = '0x11'
    						if str_array[2][0:4]=='0x10' and cmd_base!='0x10' :
    							file_out += "{CHIPCFG_CMD_BASE, "
    							file_out += miu_base_addr
    							file_out += "20},\n"
    							cmd_base = '0x10'
    						if str_array[2][0:4]=='0x16' and cmd_base!='0x16' :
    							file_out += "{CHIPCFG_CMD_BASE, "
    							file_out += miu_base_addr
    							file_out += "2c},\n"
    							cmd_base = '0x16'
    						if str_array[2][0:4]=='0x14' and cmd_base!='0x14' :
    							file_out += "{CHIPCFG_CMD_BASE, "
    							file_out += miu_base_addr
    							file_out += "28},\n"
    							cmd_base = '0x14'
								
    						if AC_only == 0:
    							file_out += "{CHIPCFG_STR_IFAC,"
    							file_out += "0x0"
    							file_out += "},\n"
								
    						file_out += "_RV32_2(0x"
    						file_out += str_array[2][4:8]
    						file_out += ","
    						file_out += str_array[3][0:6]
    						file_out += "),\n"
                    		
    						file_out += "{CHIPCFG_STR_IFAC_END,"
    						file_out += "0x0"
    						file_out += "},\n"
    						    						
    						if str_array[7][0:4]=='0x11' and cmd_base!='0x11' :
    							file_out += "{CHIPCFG_CMD_BASE, "
    							file_out += miu_base_addr
    							file_out += "22},\n"
    							cmd_base = '0x11'
    						if str_array[7][0:4]=='0x10' and cmd_base!='0x10' :
    							file_out += "{CHIPCFG_CMD_BASE, "
    							file_out += miu_base_addr
    							file_out += "20},\n"
    							cmd_base = '0x10'
    						if str_array[7][0:4]=='0x16' and cmd_base!='0x16' :
    							file_out += "{CHIPCFG_CMD_BASE, "
    							file_out += miu_base_addr
    							file_out += "2c},\n"
    							cmd_base = '0x16'
    						if str_array[7][0:4]=='0x14' and cmd_base!='0x14' :
    							file_out += "{CHIPCFG_CMD_BASE, "
    							file_out += miu_base_addr
    							file_out += "28},\n"
    							cmd_base = '0x14'

    						file_out += "{CHIPCFG_STR_IFDC,"
    						file_out += "0x0"
    						file_out += "},\n"
								
    						file_out += "_RV32_2(0x"
    						file_out += str_array[7][4:8]
    						file_out += ","
    						file_out += str_array[8][0:6]
    						file_out += "),\n"
    						
    						file_out += "{CHIPCFG_STR_IFDC_END,"
    						file_out += "0x0"
    						file_out += "},\n"
    						cmd_base_8bits = '0xFF'
							
    						AC_only = 0

                        else:
							
    						if str_array[2][0:4]=='0x11' and cmd_base!='0x11' :
    							file_out += "{CHIPCFG_CMD_BASE, "
    							file_out += miu_base_addr
    							file_out += "22},\n"
    							cmd_base = '0x11'
    						if str_array[2][0:4]=='0x10' and cmd_base!='0x10' :
    							file_out += "{CHIPCFG_CMD_BASE, "
    							file_out += miu_base_addr
    							file_out += "20},\n"
    							cmd_base = '0x10'
    						if str_array[2][0:4]=='0x16' and cmd_base!='0x16' :
    							file_out += "{CHIPCFG_CMD_BASE, "
    							file_out += miu_base_addr
    							file_out += "2c},\n"
    							cmd_base = '0x16'
    						if str_array[2][0:4]=='0x14' and cmd_base!='0x14' :
    							file_out += "{CHIPCFG_CMD_BASE, "
    							file_out += miu_base_addr
    							file_out += "28},\n"
    							cmd_base = '0x14'

    						if AC_only == 0:						
    							file_out += "{CHIPCFG_STR_IFAC,"
    							file_out += "0x0"
    							file_out += "},\n"
							
							AC_only = 1	
							
    						file_out += "_RV32_2(0x"
    						file_out += str_array[2][4:8]
    						file_out += ","
    						file_out += str_array[3][0:6]
    						file_out += "),\n"
                    							
                    else:
                        if AC_only == 1:
    						file_out += "{CHIPCFG_STR_IFAC_END,"
    						file_out += "0x0"
    						file_out += "},\n"

						AC_only = 0
							
                        if str_array[2][0:4]=='0x11' and cmd_base!='0x11':
    						file_out += "{CHIPCFG_CMD_BASE, "
    						file_out += miu_base_addr
    						file_out += "22},\n"
    						cmd_base = '0x11'
                        if str_array[2][0:4]=='0x10' and cmd_base!='0x10':
    						file_out += "{CHIPCFG_CMD_BASE, "
    						file_out += miu_base_addr
    						file_out += "20},\n"
    						cmd_base = '0x10'
                        if str_array[2][0:4]=='0x16' and cmd_base!='0x16':
    						file_out += "{CHIPCFG_CMD_BASE, "
    						file_out += miu_base_addr
    						file_out += "2c},\n"
    						cmd_base = '0x16'
                        if str_array[2][0:4]=='0x14' and cmd_base!='0x14':
    						file_out += "{CHIPCFG_CMD_BASE, "
    						file_out += miu_base_addr
    						file_out += "28},\n"
    						cmd_base = '0x14'
    
                        file_out += "_RV32_2(0x"
                        file_out += str_array[2][4:8]
                        file_out += ","
                        file_out += str_array[3][0:6]
                        file_out += "),\n"
                        cmd_base_8bits = '0xFF'
    					
                elif str_array[0] == '//DC:' and str_array[1] == 'wriu' and str_array[2] == '-w' :   				    				
                    if str_array[3][0:4]=='0x11' and cmd_base!='0x11' :
                        file_out += "{CHIPCFG_CMD_BASE, "
                        file_out += miu_base_addr
                        file_out += "22},\n"
                        cmd_base = '0x11'
                    if str_array[3][0:4]=='0x10' and cmd_base!='0x10' :
                        file_out += "{CHIPCFG_CMD_BASE, "
                        file_out += miu_base_addr
                        file_out += "20},\n"
                        cmd_base = '0x10'
                    if str_array[3][0:4]=='0x16' and cmd_base!='0x16' :
                        file_out += "{CHIPCFG_CMD_BASE, "
                        file_out += miu_base_addr
                        file_out += "2c},\n"
                        cmd_base = '0x16'
                    if str_array[3][0:4]=='0x14' and cmd_base!='0x14' :
                        file_out += "{CHIPCFG_CMD_BASE, "
                        file_out += miu_base_addr
                        file_out += "28},\n"
                        cmd_base = '0x14'

                    file_out += "{CHIPCFG_STR_IFDC,"
                    file_out += "0x0"
                    file_out += "},\n"
						
                    file_out += "_RV32_2(0x"
                    file_out += str_array[3][4:8]
                    file_out += ","
                    file_out += str_array[4][0:6]
                    file_out += "),\n"
    				
                    file_out += "{CHIPCFG_STR_IFDC_END,"
                    file_out += "0x0"
                    file_out += "},\n"
					
                elif str_array[0] == 'wriu' and str_array[1] == '-b' :
                    if (len(str_array) > 5) and (str_array[5] == '//DC:') :
                        if (len(str_array) > 6) and (str_array[6] == 'wriu') and (str_array[7] == '-b'):
                            if str_array[2][0:4]=='0x11' and cmd_base_8bits!='0x11':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "22},\n"
                                cmd_base_8bits = '0x11'
                            if str_array[2][0:4]=='0x10' and cmd_base_8bits!='0x10':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "20},\n"
                                cmd_base_8bits = '0x10'
                            if str_array[2][0:4]=='0x16' and cmd_base_8bits!='0x16':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "2c},\n"
                                cmd_base_8bits = '0x16'
                            if str_array[2][0:4]=='0x14' and cmd_base_8bits!='0x14':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "28},\n"
                                cmd_base_8bits = '0x14'
								
                            if AC_only == 0:
                                file_out += "{CHIPCFG_STR_IFAC,"
                                file_out += "0x0"
                                file_out += "},\n"
								
                            file_out += "_RV32_1_MASK("
                            if str_array[2][7] == '0' or str_array[2][7] == '2' or str_array[2][7] == '4' or str_array[2][7] == '6' or str_array[2][7] == '8' or str_array[2][7] == 'A' or str_array[2][7] == 'C' or str_array[2][7] == 'E' or str_array[2][7] == 'a' or str_array[2][7] == 'c' or str_array[2][7] == 'e' :
                                file_out += "(0x"
                                file_out += str_array[2][4:8]
                                file_out += " <<1), "
    
                            if str_array[2][7] == '1' :
                                file_out += "((0x"
                                file_out += str_array[2][4:7]
                                file_out +=  "0 <<1) + 1), "
    
                            if str_array[2][7] == '3' :
                                file_out += "((0x"
                                file_out += str_array[2][4:7]
                                file_out +=  "2 <<1) + 1), "
    
                            if str_array[2][7] == '5' :
                                file_out += "((0x"
                                file_out += str_array[2][4:7]
                                file_out +=  "4 <<1) + 1), "
    
                            if str_array[2][7] == '7' :
                                file_out += "((0x"
                                file_out += str_array[2][4:7]
                                file_out +=  "6 <<1) + 1), "
    
                            if str_array[2][7] == '9' :
                                file_out += "((0x"
                                file_out += str_array[2][4:7]
                                file_out +=  "8 <<1) + 1), "
    
                            if str_array[2][7] == 'B' :
                                file_out += "((0x"
                                file_out += str_array[2][4:7]
                                file_out +=  "A <<1) + 1), "
    
                            if str_array[2][7] == 'D' :
                                file_out += "((0x"
                                file_out += str_array[2][4:7]
                                file_out +=  "C <<1) + 1), "
    
                            if str_array[2][7] == 'F' :
                                file_out += "((0x"
                                file_out += str_array[2][4:7]
                                file_out +=  "E <<1) + 1), "
    
                            file_out += str_array[3][0:4]
                            file_out += ","
                            file_out += str_array[4][0:4]
                            file_out += "),\n"
                            cmd_base = '0xFF'
							
                            file_out += "{CHIPCFG_STR_IFAC_END,"
                            file_out += "0x0"
                            file_out += "},\n"
							
                            if str_array[8][0:4]=='0x11' and cmd_base_8bits!='0x11':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "22},\n"
                                cmd_base_8bits = '0x11'
                            if str_array[8][0:4]=='0x10' and cmd_base_8bits!='0x10':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "20},\n"
                                cmd_base_8bits = '0x10'
                            if str_array[8][0:4]=='0x16' and cmd_base_8bits!='0x16':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "2c},\n"
                                cmd_base_8bits = '0x16'
                            if str_array[8][0:4]=='0x14' and cmd_base_8bits!='0x14':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "28},\n"
                                cmd_base_8bits = '0x14'

                            file_out += "{CHIPCFG_STR_IFDC,"
                            file_out += "0x0"
                            file_out += "},\n"
								
                            file_out += "_RV32_1_MASK("
                            if str_array[8][7] == '0' or str_array[8][7] == '2' or str_array[8][7] == '4' or str_array[8][7] == '6' or str_array[8][7] == '8' or str_array[8][7] == 'A' or str_array[8][7] == 'C' or str_array[8][7] == 'E' or str_array[8][7] == 'a' or str_array[8][7] == 'c' or str_array[8][7] == 'e' :
                                file_out += "(0x"
                                file_out += str_array[8][4:8]
                                file_out += " <<1), "
    
                            if str_array[8][7] == '1' :
                                file_out += "((0x"
                                file_out += str_array[8][4:7]
                                file_out +=  "0 <<1) + 1), "
    
                            if str_array[8][7] == '3' :
                                file_out += "((0x"
                                file_out += str_array[8][4:7]
                                file_out +=  "2 <<1) + 1), "
    
                            if str_array[8][7] == '5' :
                                file_out += "((0x"
                                file_out += str_array[8][4:7]
                                file_out +=  "4 <<1) + 1), "
    
                            if str_array[8][7] == '7' :
                                file_out += "((0x"
                                file_out += str_array[8][4:7]
                                file_out +=  "6 <<1) + 1), "
    
                            if str_array[8][7] == '9' :
                                file_out += "((0x"
                                file_out += str_array[8][4:7]
                                file_out +=  "8 <<1) + 1), "
    
                            if str_array[8][7] == 'B' :
                                file_out += "((0x"
                                file_out += str_array[8][4:7]
                                file_out +=  "A <<1) + 1), "
    
                            if str_array[8][7] == 'D' :
                                file_out += "((0x"
                                file_out += str_array[8][4:7]
                                file_out +=  "C <<1) + 1), "
    
                            if str_array[8][7] == 'F' :
                                file_out += "((0x"
                                file_out += str_array[8][4:7]
                                file_out +=  "E <<1) + 1), "
    
                            file_out += str_array[9][0:4]
                            file_out += ","
                            file_out += str_array[10][0:4]
                            file_out += "),\n"
                            cmd_base = '0xFF'
							
                            file_out += "{CHIPCFG_STR_IFDC_END,"
                            file_out += "0x0"
                            file_out += "},\n"
							
                            AC_only = 0
							
                        else:
                            if str_array[2][0:4]=='0x11' and cmd_base_8bits!='0x11':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "22},\n"
                                cmd_base_8bits = '0x11'
                            if str_array[2][0:4]=='0x10' and cmd_base_8bits!='0x10':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "20},\n"
                                cmd_base_8bits = '0x10'
                            if str_array[2][0:4]=='0x16' and cmd_base_8bits!='0x16':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "2c},\n"
                                cmd_base_8bits = '0x16'
                            if str_array[2][0:4]=='0x14' and cmd_base_8bits!='0x14':
                                file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                                file_out += miu_base_addr
                                file_out += "28},\n"
                                cmd_base_8bits = '0x14'
								
                            if AC_only == 0:
                                file_out += "{CHIPCFG_STR_IFAC,"
                                file_out += "0x0"
                                file_out += "},\n"
								
                            AC_only = 1
							
                            file_out += "_RV32_1_MASK("
                            if str_array[2][7] == '0' or str_array[2][7] == '2' or str_array[2][7] == '4' or str_array[2][7] == '6' or str_array[2][7] == '8' or str_array[2][7] == 'A' or str_array[2][7] == 'C' or str_array[2][7] == 'E' or str_array[2][7] == 'a' or str_array[2][7] == 'c' or str_array[2][7] == 'e' :
                                file_out += "(0x"
                                file_out += str_array[2][4:8]
                                file_out += " <<1), "
    
                            if str_array[2][7] == '1' :
                                file_out += "((0x"
                                file_out += str_array[2][4:7]
                                file_out +=  "0 <<1) + 1), "
    
                            if str_array[2][7] == '3' :
                                file_out += "((0x"
                                file_out += str_array[2][4:7]
                                file_out +=  "2 <<1) + 1), "
    
                            if str_array[2][7] == '5' :
                                file_out += "((0x"
                                file_out += str_array[2][4:7]
                                file_out +=  "4 <<1) + 1), "
    
                            if str_array[2][7] == '7' :
                                file_out += "((0x"
                                file_out += str_array[2][4:7]
                                file_out +=  "6 <<1) + 1), "
    
                            if str_array[2][7] == '9' :
                                file_out += "((0x"
                                file_out += str_array[2][4:7]
                                file_out +=  "8 <<1) + 1), "
    
                            if str_array[2][7] == 'B' :
                                file_out += "((0x"
                                file_out += str_array[2][4:7]
                                file_out +=  "A <<1) + 1), "
    
                            if str_array[2][7] == 'D' :
                                file_out += "((0x"
                                file_out += str_array[2][4:7]
                                file_out +=  "C <<1) + 1), "
    
                            if str_array[2][7] == 'F' :
                                file_out += "((0x"
                                file_out += str_array[2][4:7]
                                file_out +=  "E <<1) + 1), "

                            file_out += str_array[3][0:4]
                            file_out += ","
                            file_out += str_array[4][0:4]
                            file_out += "),\n"
                            cmd_base = '0xFF'
							
                    else:
                        if AC_only == 1:
                            file_out += "{CHIPCFG_STR_IFAC_END,"
                            file_out += "0x0"
                            file_out += "},\n"
							
                        AC_only = 0
						
                        if str_array[2][0:4]=='0x11' and cmd_base_8bits!='0x11':
                            file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                            file_out += miu_base_addr
                            file_out += "22},\n"
                            cmd_base_8bits = '0x11'
                        if str_array[2][0:4]=='0x10' and cmd_base_8bits!='0x10':
                            file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                            file_out += miu_base_addr
                            file_out += "20},\n"
                            cmd_base_8bits = '0x10'
                        if str_array[2][0:4]=='0x16' and cmd_base_8bits!='0x16':
                            file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                            file_out += miu_base_addr
                            file_out += "2c},\n"
                            cmd_base_8bits = '0x16'
                        if str_array[2][0:4]=='0x14' and cmd_base_8bits!='0x14':
                            file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                            file_out += miu_base_addr
                            file_out += "28},\n"
                            cmd_base_8bits = '0x14'

                        file_out += "_RV32_1_MASK("
                        if str_array[2][7] == '0' or str_array[2][7] == '2' or str_array[2][7] == '4' or str_array[2][7] == '6' or str_array[2][7] == '8' or str_array[2][7] == 'A' or str_array[2][7] == 'C' or str_array[2][7] == 'E' or str_array[2][7] == 'a' or str_array[2][7] == 'c' or str_array[2][7] == 'e':
                            file_out += "(0x"
                            file_out += str_array[2][4:8]
                            file_out += " <<1), "
    
                        if str_array[2][7] == '1' :
                            file_out += "((0x"
                            file_out += str_array[2][4:7]
                            file_out +=  "0 <<1) + 1), "
    
                        if str_array[2][7] == '3' :
                            file_out += "((0x"
                            file_out += str_array[2][4:7]
                            file_out +=  "2 <<1) + 1), "
    
                        if str_array[2][7] == '5' :
                            file_out += "((0x"
                            file_out += str_array[2][4:7]
                            file_out +=  "4 <<1) + 1), "
    
                        if str_array[2][7] == '7' :
                            file_out += "((0x"
                            file_out += str_array[2][4:7]
                            file_out +=  "6 <<1) + 1), "
    
                        if str_array[2][7] == '9' :
                            file_out += "((0x"
                            file_out += str_array[2][4:7]
                            file_out +=  "8 <<1) + 1), "
    
                        if str_array[2][7] == 'B' :
                            file_out += "((0x"
                            file_out += str_array[2][4:7]
                            file_out +=  "A <<1) + 1), "
    
                        if str_array[2][7] == 'D' :
                            file_out += "((0x"
                            file_out += str_array[2][4:7]
                            file_out +=  "C <<1) + 1), "
    
                        if str_array[2][7] == 'F' :
                            file_out += "((0x"
                            file_out += str_array[2][4:7]
                            file_out +=  "E <<1) + 1), "
    
                        file_out += str_array[3][0:4]
                        file_out += ","
                        file_out += str_array[4][0:4]
                        file_out += "),\n"
                        cmd_base = '0xFF'

                elif str_array[0] == '//DC:' and str_array[1] == 'wriu' and str_array[2] == '-b' :			
                    if str_array[3][0:4]=='0x11' and cmd_base_8bits!='0x11':
                        file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                        file_out += miu_base_addr
                        file_out += "22},\n"
                        cmd_base_8bits = '0x11'
                    if str_array[3][0:4]=='0x10' and cmd_base_8bits!='0x10':
                        file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                        file_out += miu_base_addr
                        file_out += "20},\n"
                        cmd_base_8bits = '0x10'
                    if str_array[3][0:4]=='0x16' and cmd_base_8bits!='0x16':
                        file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                        file_out += miu_base_addr
                        file_out += "2c},\n"
                        cmd_base_8bits = '0x16'
                    if str_array[3][0:4]=='0x14' and cmd_base_8bits!='0x14':
                        file_out += "{CHIPCFG_CMD_BASE_8BITS, "
                        file_out += miu_base_addr
                        file_out += "28},\n"
                        cmd_base_8bits = '0x14'

                    file_out += "{CHIPCFG_STR_IFDC,"
                    file_out += "0x0"
                    file_out += "},\n"
				    	
                    file_out += "_RV32_1_MASK("
                    if str_array[3][7] == '0' or str_array[3][7] == '2' or str_array[3][7] == '4' or str_array[3][7] == '6' or str_array[3][7] == '8' or str_array[3][7] == 'A' or str_array[3][7] == 'C' or str_array[3][7] == 'E' or str_array[3][7] == 'a' or str_array[3][7] == 'c' or str_array[3][7] == 'e' :
                        file_out += "(0x"
                        file_out += str_array[3][4:8]
                        file_out += " <<1), "
    
                    if str_array[3][7] == '1' :
                        file_out += "((0x"
                        file_out += str_array[3][4:7]
                        file_out +=  "0 <<1) + 1), "
    
                    if str_array[3][7] == '3' :
                        file_out += "((0x"
                        file_out += str_array[3][4:7]
                        file_out +=  "2 <<1) + 1), "
    
                    if str_array[3][7] == '5' :
                        file_out += "((0x"
                        file_out += str_array[3][4:7]
                        file_out +=  "4 <<1) + 1), "
    
                    if str_array[3][7] == '7' :
                        file_out += "((0x"
                        file_out += str_array[3][4:7]
                        file_out +=  "6 <<1) + 1), "
    
                    if str_array[3][7] == '9' :
                        file_out += "((0x"
                        file_out += str_array[3][4:7]
                        file_out +=  "8 <<1) + 1), "
    
                    if str_array[3][7] == 'B' :
                        file_out += "((0x"
                        file_out += str_array[3][4:7]
                        file_out +=  "A <<1) + 1), "
    
                    if str_array[3][7] == 'D' :
                        file_out += "((0x"
                        file_out += str_array[3][4:7]
                        file_out +=  "C <<1) + 1), "
    
                    if str_array[3][7] == 'F' :
                        file_out += "((0x"
                        file_out += str_array[3][4:7]
                        file_out +=  "E <<1) + 1), "
    
                    file_out += str_array[4][0:4]
                    file_out += ","
                    file_out += str_array[5][0:4]
                    file_out += "),\n"
                    cmd_base = '0xFF'
				    
                    file_out += "{CHIPCFG_STR_IFDC_END,"
                    file_out += "0x0"
                    file_out += "},\n"
					
    if AC_only == 1:
        file_out += "{CHIPCFG_STR_IFAC_END,"
        file_out += "0x0"
        file_out += "},\n"
		
    file.close()
    open(miu_script_target, 'wb').write(file_out)
    print('parsing miu script done\n')

if __name__ == '__main__':	
    main(sys.argv)
