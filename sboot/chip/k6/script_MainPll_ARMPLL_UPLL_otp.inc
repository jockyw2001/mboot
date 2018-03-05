// K6 DMIPS = (CPU_FREQ * 2.3) * 2
CPU_FREQ_700MHZ = 0x4EFE88


// Turn on xtal
WREG (REG_ADDR_BASE + (0x000e7a <<1 )) , 0x0000

// MPLL Power on
WREG (REG_ADDR_BASE + (0x110b02 <<1 )) , 0x0000
WREG_B (REG_ADDR_BASE + (0x110b08 <<1 )) , 0x11
WREG (REG_ADDR_BASE + (0x110b00 <<1 )) , 0x0013


//
//CPU freq setting
//

// MIPSPLL disable
WREG_B (REG_ADDR_BASE + ((0x110a22 <<1) + 1)) , 0x01








WREG (REG_ADDR_BASE + (0x110ac0 <<1 )) , (CPU_FREQ_700MHZ & 0xffff)
WREG (REG_ADDR_BASE + (0x110ac2 <<1 )) , (CPU_FREQ_700MHZ >> 16)

WREG (REG_ADDR_BASE + (0x110ac4 <<1 )) , 0x0001

// bit0: MIPSPLL disable
// bit3: enable ARM test clock for clock detector (OTP CPU_CLK_MAX will use this)
WREG_B (REG_ADDR_BASE + ((0x110a22 <<1) + 1)) , 0x08
