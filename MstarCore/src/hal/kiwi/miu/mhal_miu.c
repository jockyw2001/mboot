
#include <mhal_miu.h>
#include <MsTypes.h>
#include <drvMIU.h>

static void _MIU_Write2Bytes(U32 u32RegOffset, U16 u16Val)
{
    ((volatile U16*)(MIUREG_BASE))[u32RegOffset] = u16Val;
}

static const U16 client_gp[9] = {REG_MIUSEL_GP0, REG_MIUSEL_GP1, REG_MIUSEL_GP2, REG_MIUSEL_GP3, REG_MIUSEL_GP4, REG_MIUSEL_GP5, REG_MIUSEL_GP6, REG_MIUSEL_GP7, 0};

U8 HAL_MIU_SelectMIU_UBoot(U8 bMIU1, U16 *au16SelMiu, U8 u8GpNum)
{
    U8 idx;
    U32 u32RegAddr = 0;
    U32 u32RegArbbAddr = 0;

    if(bMIU1==0)
    {
        u32RegAddr =MIU0_REG_BASE;
        u32RegArbbAddr = MIU0_ARBB_REG_BASE;
    }
    else if(bMIU1==1)
    {
        u32RegAddr =MIU1_REG_BASE;
        u32RegArbbAddr = MIU1_ARBB_REG_BASE;
    }
    else if(bMIU1==2)
    {
        u32RegAddr =MIU2_REG_BASE;
        u32RegArbbAddr = MIU2_ARBB_REG_BASE;
    }

	for (idx = 0; idx < u8GpNum; idx++)
	{
        _MIU_Write2Bytes( ( u32RegAddr+client_gp[idx]), au16SelMiu[idx]);
    }

    return TRUE;
}



static MS_U8 u8MIUProtectkernel_ID[16] = {
    MIU_CLIENT_MIPS_RW, //MIPS W
    MIU_CLIENT_NAND_RW, 
    MIU_CLIENT_USB_UHC0_RW, //USB0
    MIU_CLIENT_USB_UHC1_RW, //USB1
    MIU_CLIENT_USB_UHC2_RW, //USB2
    MIU_CLIENT_G3D_RW, //G3D
    MIU_CLIENT_USB3_RW, //USB3.0
    MIU_CLIENT_SDIO_RW, //SD IO
    MIU_CLIENT_SATA_RW, //SATA
    MIU_CLIENT_USB_UHC3_RW, //USB3
};


MS_U8* HAL_MIU_GetKernelProtectClients(void)
{
    return u8MIUProtectkernel_ID;
}

MS_U64 HAL_MIU_PA2BA(MS_U64 u64PhyAddr)
{
    MS_U64 u64BusAddr = 0x0;
    if(u64PhyAddr>=CONFIG_SYS_MIU_INTERVAL)
    {
        u64BusAddr= (u64PhyAddr - CONFIG_SYS_MIU_INTERVAL + CONFIG_SYS_MIU1_BUS);
    }
    else //miu0
    {
        if(u64PhyAddr>=0x10000000)//<256
        {
            u64BusAddr= (0x40000000+u64PhyAddr);
        }
        else
        {
            u64BusAddr= u64PhyAddr;
        }
    }
    return u64BusAddr;
}

MS_U64 HAL_MIU_BA2PA(MS_U64 u64BusAddr)
{
    if ((0x00000000 <= u64BusAddr) && (0x10000000 > u64BusAddr))
    {
        return (u64BusAddr);
    }
    if ((0x40000000 <= u64BusAddr) && (0x50000000 > u64BusAddr))
    {
        return (u64BusAddr & 0x0fffffff);
    }
    if ((0x50000000 <= u64BusAddr) && (0x60000000 > u64BusAddr))
    {
        return (u64BusAddr & 0x1fffffff);
    }
    if ((0x60000000 <= u64BusAddr) && (0x70000000 > u64BusAddr))
    {
        return ((u64BusAddr & 0x0fffffff)+CONFIG_SYS_MIU_INTERVAL);
    }
    if ((0x70000000 <= u64BusAddr) && (0x80000000 > u64BusAddr))
    {
        return ((u64BusAddr & 0x1fffffff)+CONFIG_SYS_MIU_INTERVAL);
    }
    if ((0xE0000000 <= u64BusAddr) && (0xFFFFFFFF >= u64BusAddr)) //BA 0xE --> PA 0xC
    {
        return ((u64BusAddr & 0x1fffffff)+CONFIG_SYS_MIU2_INTERVAL);
    }
    return 0;
}