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

    // UART_SEL0 --> PIU UART0

    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA6<<1)) = 0x0654;
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA8<<1)) = 0x0000;

#endif
    // <-------

    // UART mode

    //enable reg_uart_rx_enable;
    *(volatile MS_U32*)(MS_RIU_MAP+(0x101EA2<<1)) |= (1<<10);

   
}

void Mhal_Sys_WholeChipReset( void )
{

    WriteByte( 0xE5C, 0x79 ); //PM_SLEEP (Bank 0x000E) for T13, A1, A6, A7, Amethyst, Eagle
    while(1);

}


