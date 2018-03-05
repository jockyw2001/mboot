#
# (C) Copyright 2004-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk
include $(TOPDIR)/../sboot/.config
CONFIG_MSTAR_CHIP_NAME := $(subst ",,$(CONFIG_MSTAR_CHIP_NAME))

LIB	=  $(obj)libmscore.a

#AOBJS	=

COBJS	=
#MstarCore:src

# SYS ###
CFLAGS += -I./src/hal/$(CONFIG_MSTAR_CHIP_NAME)/sys/
CPPFLAGS += -I./src/hal/$(CONFIG_MSTAR_CHIP_NAME)/sys/
CFLAGS += -I./src/drivers/sd/
CFLAGS += -I./src/hal/$(CONFIG_MSTAR_CHIP_NAME)/sdio/
CPPFLAGS += -I./src/drivers/sd/
CPPFLAGS += -I./src/hal/$(CONFIG_MSTAR_CHIP_NAME)/sdio/
COBJS += ./src/api/MsInit.o
ifeq ($(CONFIG_UBOOT_MINI_LIB),)
COBJS += ./src/api/apiIR.o
COBJS += ./src/api/msAPI_Power.o
endif
COBJS += ./src/api/MsTemplate.o
COBJS += ./src/api/MsSystem.o
COBJS += ./src/api/MsSysUtility.o
ifeq ($(CONFIG_UBOOT_MINI_LIB),)
COBJS += ./src/api/MsCoreRegister.o
COBJS += ./src/api/MsSetupTee.o
endif
COBJS += ./src/api/msAPI_string.o

ifdef CONFIG_POINTS_HANDLER_ENABLE
COBJS += ./src/api/MsApiChkPoints.o
endif

ifeq ($(CONFIG_FRONT_END),y)
CPPFLAGS += -I./cofdm/
CFLAGS += -I./cofdm/
CPPFLAGS += -I./tuner/
CFLAGS += -I./tuner/
COBJS += ./src/api/msAPI_Tuner.o
endif

ifeq ($(CONFIG_KEYPAD),y)
ifeq ($(CONFIG_Customer),y)
CFLAGS += -I$(TOPDIR)/../MstarCustomer/MSTAR/include/keypad/$(CONFIG_CUSTOMER_FOLDNAME)/
CPPFLAGS += -I$(TOPDIR)/../MstarCustomer/MSTAR/include/keypad/$(CONFIG_CUSTOMER_FOLDNAME)/
else
CFLAGS += -I$(TOPDIR)/../sboot/inc/$(CONFIG_MSTAR_CHIP_NAME)/board/$(CONFIG_MSTAR_BD_BOARDNAME)
CPPFLAGS += -I$(TOPDIR)/../sboot/inc/$(CONFIG_MSTAR_CHIP_NAME)/board/$(CONFIG_MSTAR_BD_BOARDNAME)
endif
endif

#ifeq ($(ENABLE_POWER_MUSIC),y)
COBJS += ./src/api/MsApiAudio.o
#endif
ifeq ($(CONFIG_UARTCOMM),y)
COBJS += ./src/api/MsUartComm.o
COBJS += ./cmd/cmd_MsUartComm.o
endif
ifeq ($(CONFIG_CVBS_OUT),y)
COBJS += ./src/api/MsApiVE.o
COBJS += ./cmd/cmd_MsVE.o
endif
ifeq ($(CONFIG_XC_Init),y)
COBJS += ./src/api/MsApiXC.o
COBJS += ./cmd/cmd_MsXC.o
endif
ifeq ($(CONFIG_DOLBY),y)
COBJS += ./src/api/MsApiMspi.o
COBJS += ./cmd/cmd_MsMspi.o
endif
COBJS += ./src/api/MsApiPanel.o
COBJS += ./src/api/MsApiGEGOP.o
COBJS += ./src/api/MsApiJpd.o
COBJS += ./src/api/MsApiPM.o
COBJS += ./src/api/MsApiSpi.o
ifeq ($(CONFIG_UBOOT_MINI_LIB),)
COBJS += ./src/api/MsMemPerformance.o
endif
ifdef CONFIG_CMD_MMC
COBJS += ./src/api/MsMmc.o
endif
ifeq ($(CONFIG_MSTAR_STR_ENABLE),y)
COBJS += ./src/api/MsStr.o
COBJS += ./cmd/cmd_Str.o
endif
ifdef CONFIG_MSTAR_FTL
COBJS += ./src/api/MsFtl.o
endif
ifdef CONFIG_MSTAR_UNFD_FTL
COBJS += ./src/api/MsUnfdFtl.o
endif
ifeq ($(CONFIG_HDMI_TX),y)
COBJS += ./src/api/MsApiHdmi.o
endif
ifeq ($(CONFIG_EEPROM),y)
COBJS += ./src/api/MsEeprom.o
endif
ifeq ($(CONFIG_UART_RX),y)
COBJS += ./src/api/MsApiUart.o
endif
ifeq ($(CONFIG_UBOOT_MINI_LIB),)
COBJS += ./src/api/MsApiUrsa.o
endif

#MstarCore:cmd
ifeq ($(CONFIG_EMAC),y)
COBJS += ./cmd/cmd_emac.o
endif
ifeq ($(CONFIG_GMAC),y)
COBJS += ./cmd/cmd_emac.o
endif
COBJS += ./cmd/cmd_spi.o
ifeq ($(CONFIG_UBOOT_MINI_LIB),)
COBJS += ./cmd/cmd_MsMemPerformance.o
endif
COBJS += ./cmd/cmd_MsSysUtility.o
COBJS += ./cmd/cmd_MsTemplate.o
COBJS += ./cmd/cmd_MsSystem.o
COBJS += ./cmd/cmd_MsPanel.o

ifdef CONFIG_CMD_MMC
COBJS += ./cmd/cmd_MsMmc.o
endif
ifdef CONFIG_MSTAR_FTL
COBJS += ./cmd/cmd_MsFtl.o
endif
ifdef CONFIG_MSTAR_UNFD_FTL
COBJS += ./cmd/cmd_MsUnfdFtl.o
endif
ifdef CONFIG_SD
COBJS += ./cmd/cmd_MsSd.o
COBJS += ./src/api/MsSd.o
endif
ifeq ($(CONFIG_FRONT_END),y)
COBJS += ./cmd/cmd_tuner.o
endif
ifeq ($(CONFIG_OAD_IN_MBOOT),y)
COBJS += ./cmd/cmd_dmx.o
endif
ifeq ($(CONFIG_HDMI_TX),y)
COBJS += ./cmd/cmd_MsHdmi.o
endif
ifeq ($(CONFIG_EEPROM),y)
COBJS += ./cmd/cmd_MsEeprom.o
endif
ifeq ($(CONFIG_UART_RX),y)
COBJS += ./cmd/cmd_MsUart.o
endif
ifeq ($(CONFIG_UBOOT_MINI_LIB),)
COBJS += ./cmd/cmd_MsUrsa.o
COBJS += ./cmd/cmd_pcie.o
endif

SRCS	:= $(AOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(AOBJS) $(COBJS))

#CPPFLAGS += -I..

all:	$(LIB)

$(LIB): $(obj).depend $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
