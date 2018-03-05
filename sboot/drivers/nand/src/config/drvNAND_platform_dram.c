#include "../../inc/common/drvNAND.h"

//=============================================================
#if (defined(NAND_DRV_TV_SBOOT) && NAND_DRV_TV_SBOOT)

U32 nand_clock_setting(U32 u32ClkParam)
{
	NAND_DRIVER *pNandDrv = (NAND_DRIVER*)(CONFIG_SRAM_BASE_ADDRESS + CONFIG_SRAM_FCIE_OFFSET);

	u32ClkParam = FCIE3_SW_DEFAULT_CLK;

	if((REG(NC_DDR_CTRL) & BIT_DDR_MASM) || pNandDrv->u8_CellType == 1)
		return UNFD_ST_SUCCESS;

    #if defined(CONFIG_MSTAR_KRATOS) || defined(CONFIG_MSTAR_KIWI)

    /*div4 enable*/
    REG_SET_BITS_UINT16(REG_CLK_NFIE, BIT_CLK_SELECT);

    /*set FCIE 4x clock*/
    REG_CLR_BITS_UINT16(REG_CLK_NFIE, BIT1|BIT0);
    REG_CLR_BITS_UINT16(REG_CLK_NFIE, NFIE_CLK_MASK);
    REG_SET_BITS_UINT16(REG_CLK_NFIE, u32ClkParam << 0);
    
    #elif defined(CONFIG_MSTAR_MULAN)

    REG_SET_BITS_UINT16(REG_CLK_NFIE_DIV4_EN, BIT_CLK_DIV4_ENABLE);
    
    REG_CLR_BITS_UINT16(REG_CLK_NFIE, BIT9|BIT8);
    REG_CLR_BITS_UINT16(REG_CLK_NFIE, NFIE_CLK_MASK);
    REG_SET_BITS_UINT16(REG_CLK_NFIE, u32ClkParam << 0);
    
    #else
	REG_CLR_BITS_UINT16(reg_ckg_fcie, BIT6-1); // enable FCIE clk, set to lowest clk

	REG_SET_BITS_UINT16(reg_ckg_fcie, u32ClkParam);

    #endif  // (ENABLE_NIKE) || defined(CONFIG_MSTAR_MADISON) || defined(CONFIG_MSTAR_MIAMI) || defined(CONFIG_MSTAR_MONACO) || defined(ENABLE_NADAL) || (ENABLE_NUGGET)  || defined(CONFIG_MSTAR_CLIPPERS) || defined(CONFIG_MSTAR_MUJI) || defined(CONFIG_MSTAR_MONET)

	#if defined(NC_SEL_FCIE3) && NC_SEL_FCIE3
	REG_WRITE_UINT16(NC_LATCH_DATA, BIT_RE_DATA_LATCH_SEL_1T);
	#elif defined(NC_SEL_FCIE5) && NC_SEL_FCIE5
	REG_WRITE_UINT16(NC_LATCH_DATA, BIT_NC_LATCH_DATA_2_0_T|BIT_NC_LATCH_STS_2_0_T);
	#endif

    return UNFD_ST_SUCCESS;
}

#endif
