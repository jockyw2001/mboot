#define CONFIG_RIU_BASE_ADDRESS     (0x1F000000U)
#define RIU     ((unsigned short volatile *) CONFIG_RIU_BASE_ADDRESS)

#define SEAL_SECURE0_RANGE0                     (0x00123700UL)
#define REG_SECURE_RANGE0_START_ADDR           (SEAL_SECURE0_RANGE0+0x00UL)
#define REG_SECURE_RANGE0_END_ADDR             (SEAL_SECURE0_RANGE0+0x08UL)
#define REG_SECURE0_DETECT_ENABLE               (SEAL_SECURE0_RANGE0+0xECUL)


// startAddr, endAddr:  BUS address
void BOOT_SEAL_SetSecureRange(unsigned int startAddr, unsigned int endAddr)
{
    // covert to PA
    startAddr -= CONFIG_MIU0_BUSADDR;
    endAddr -= CONFIG_MIU0_BUSADDR;

    RIU[REG_SECURE_RANGE0_START_ADDR] = (startAddr & 0xFFFFULL);
    RIU[REG_SECURE_RANGE0_START_ADDR + 2] = ((startAddr>>16) & 0xFFFFULL);
    RIU[REG_SECURE_RANGE0_START_ADDR + 4] = ((startAddr>>32) & 0xFFFFULL);

    RIU[REG_SECURE_RANGE0_END_ADDR] = (endAddr & 0xFFFFULL);
    RIU[REG_SECURE_RANGE0_END_ADDR + 2] = ((endAddr>>16) & 0xFFFFULL);
    RIU[REG_SECURE_RANGE0_END_ADDR + 4] = ((endAddr>>32) & 0xFFFFULL);

    (RIU[REG_SECURE0_DETECT_ENABLE]) |= (1<<0);
}
