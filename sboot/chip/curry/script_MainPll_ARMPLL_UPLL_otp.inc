// Turn on xtal
WREG (REG_ADDR_BASE + (0x000e7a <<1 )) , 0x0000

// MPLL Power on
WREG (REG_ADDR_BASE + (0x110b02 <<1 )) , 0x0000
WREG_B (REG_ADDR_BASE + (0x110b08 <<1 )) , 0x11
WREG (REG_ADDR_BASE + (0x110b00 <<1 )) , 0x0013
