#include <mhal_sys.h>
#include <MsTypes.h>
#include <MsUboot.h>
#include <command.h>
#include <common.h>
#include <MsSysUtility.h>

#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
#include "../mmc/inc/common/eMMC.h"
#endif
void Mhal_console_init(void)
{

    //bit 12 UART Enable, bit 11 UART RX Enable
    *(volatile MS_U32*)(MS_RIU_MAP+(0x0E12<<1)) &= ~0x1800;    //don't enable uart (decided by mboot env)

    // Switch UART0 to PIU UART 0
    // Clear UART_SEL0
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA6<<1)) &= ~0x000F;
#if (EARLY_DISABLE_UART)
#else
    // bit 11 UART RX Enable
    *(volatile MS_U32*)(MS_RIU_MAP+(0x0E12<<1)) |= 0x0800;
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA6<<1)) = 0x0654;
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA8<<1)) = 0x0000;
#endif

#if (CONFIG_Second_UART_Mode_1 || CONFIG_Second_UART_Mode_2 || CONFIG_Second_UART_Mode_3)
    // <-------
	//config PIU_UART1 CLK
	*(volatile MS_U32*)(MS_RIU_MAP+(0x100B28<<1)) &= 0xFFE0; // 100B bank 14 reg_ckg_uart1
	*(volatile MS_U32*)(MS_RIU_MAP+(0x100B28<<1)) |= 0x0008; //set fuart clk 144M
    //config PIU_UART1
    *(volatile MS_U32*)(MS_RIU_MAP+(0x11060C<<1)) |= 0x0080; //Divisor Latch Access enable
    *(volatile MS_U32*)(MS_RIU_MAP+(0x110600<<1)) = 0x004E;  // Divisor Latch LSB = 0x4E
    *(volatile MS_U32*)(MS_RIU_MAP+(0x110604<<1)) = 0x0000;  //Divisor Latch MSB =0
    *(volatile MS_U32*)(MS_RIU_MAP+(0x11060C<<1)) &= 0xFF7F; //Divisor Latch Access disable

    *(volatile MS_U32*)(MS_RIU_MAP+(0x110608<<1)) = 0x0000;
    *(volatile MS_U32*)(MS_RIU_MAP+(0x110608<<1)) = 0x0007; //enable fifo & clear fifo

    *(volatile MS_U32*)(MS_RIU_MAP+(0x11060C<<1)) = 0x0000;
    *(volatile MS_U32*)(MS_RIU_MAP+(0x11060C<<1)) = 0x0003; //8-bit Data / 1-bit Stop / No Parity Bit

    *(volatile MS_U32*)(MS_RIU_MAP+(0x110610<<1)) = 0x0000; //Set DTS/RTS to 1

    *(volatile MS_U32*)(MS_RIU_MAP+(0x110604<<1)) = 0x0000; //Disable Interrupt
    // config UART source
	*(volatile MS_U32*)(MS_RIU_MAP+(0x101EA6<<1)) = 0xFF14;
#endif

#if (CONFIG_Fourth_UART_Mode_1 || CONFIG_Fourth_UART_Mode_2 || CONFIG_Fourth_UART_Mode_3)
		// <-------
		//config PIU_FUART CLK
		*(volatile MS_U32*)(MS_RIU_MAP+(0x100B2E<<1)) &= 0xFFF0; // 100B bank 17 reg_ckg_Fuart
		*(volatile MS_U32*)(MS_RIU_MAP+(0x100B2E<<1)) |= 0x0008; //set fuart clk 108M
		//config PIU_FUART
		*(volatile MS_U32*)(MS_RIU_MAP+(0x11068C<<1)) |= 0x0080; //Divisor Latch Access enable
		*(volatile MS_U32*)(MS_RIU_MAP+(0x110680<<1)) = 0x004E;  // Divisor Latch LSB = 0x4E
		*(volatile MS_U32*)(MS_RIU_MAP+(0x110684<<1)) = 0x0000;  //Divisor Latch MSB =0
		*(volatile MS_U32*)(MS_RIU_MAP+(0x11068C<<1)) &= 0xFF7F; //Divisor Latch Access disable

		*(volatile MS_U32*)(MS_RIU_MAP+(0x110688<<1)) = 0x0000;
		*(volatile MS_U32*)(MS_RIU_MAP+(0x110688<<1)) = 0x0007; //enable fifo & clear fifo

		*(volatile MS_U32*)(MS_RIU_MAP+(0x11068C<<1)) = 0x0000;
		*(volatile MS_U32*)(MS_RIU_MAP+(0x11068C<<1)) = 0x0003; //8-bit Data / 1-bit Stop / No Parity Bit

		*(volatile MS_U32*)(MS_RIU_MAP+(0x110690<<1)) = 0x0000; //Set DTS/RTS to 1

		*(volatile MS_U32*)(MS_RIU_MAP+(0x110684<<1)) = 0x0000; //Disable Interrupt
		// config UART source
		*(volatile MS_U32*)(MS_RIU_MAP+(0x101EA6<<1)) = 0x7014;
#endif

    //enable reg_uart_rx_enable;
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA2<<1)) |= (1<<10);

}

void Mhal_Sys_WholeChipReset( void )
{

    WriteByte( 0xE5C, 0x79 ); //PM_SLEEP (Bank 0x000E) for T13, A1, A6, A7, Amethyst, Eagle

    while(1);

}


