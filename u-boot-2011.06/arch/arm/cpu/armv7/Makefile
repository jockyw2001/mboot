#
# (C) Copyright 2000-2003
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
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

LIB	= $(obj)lib$(CPU).o

START	:= start.o
ifdef CONFIG_MSTAR_STR_MINISIZE
START += start_str.o
endif
COBJS	:= cpu.o
COBJS  += syslib.o
SOBJS  := cache.o

ifneq ($(CONFIG_ARMV7_NONSEC)$(CONFIG_ARMV7_VIRT),)
	SOBJS    += nonsec_virt.o
	COBJS    += virt-v7.o
endif

SRCS	:= $(START:.o=.S) $(COBJS:.o=.c) $(SOBJS:.o=.S)
OBJS	:= $(addprefix $(obj),$(COBJS))
OBJS	+= $(addprefix $(obj),$(SOBJS))
START	:= $(addprefix $(obj),$(START))

all:	$(obj).depend PREPARE_PRANA2 $(START) $(LIB)

PREPARE_PRANA2:
	cp $(TOPDIR)/lib/mstar/libprana_$(CONFIG_MSTAR_CHIP_NAME).a $(TOPDIR)/lib/mstar/templibprana_$(CONFIG_MSTAR_CHIP_NAME).a
	@echo "Extracting vectors.o..."
	ar x $(TOPDIR)/lib/mstar/templibprana_$(CONFIG_MSTAR_CHIP_NAME).a vectors.o
	ar d $(TOPDIR)/lib/mstar/templibprana_$(CONFIG_MSTAR_CHIP_NAME).a vectors.o
	@echo "Removing head.o..."
	ar d $(TOPDIR)/lib/mstar/templibprana_$(CONFIG_MSTAR_CHIP_NAME).a head.o
	rm $(TOPDIR)/lib/mstar/templibprana_$(CONFIG_MSTAR_CHIP_NAME).a
$(LIB):	$(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
