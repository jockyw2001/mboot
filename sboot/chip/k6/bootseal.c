#define CONFIG_RIU_BASE_ADDRESS     (0x1F000000U)
#define RIU     ((unsigned short volatile *) CONFIG_RIU_BASE_ADDRESS)

#define SEAL_TZPC_NONPM_MIU0                     (0x00122700UL)
#define SEAL_SECURE0_RANGE0                     (0x00123700UL)
#define REG_SECURE_RANGE0_START_ADDR           (SEAL_SECURE0_RANGE0+0x00UL)
#define REG_SECURE_RANGE0_END_ADDR             (SEAL_SECURE0_RANGE0+0x04UL)
#define REG_SECURE0_DETECT_ENABLE               (SEAL_SECURE0_RANGE0+0xECUL)

#define REG_TZPC_MIU0_ID0                      (SEAL_TZPC_NONPM_MIU0+0x38)
#define REG_TZPC_MIU0_CTL                      (SEAL_TZPC_NONPM_MIU0+0x24)
#define REG_TZPC_MIU_TOP_EN                    (1<<0)
#define REG_TZPC_MIU0_BASE_ADDR_LOW            (SEAL_TZPC_NONPM_MIU0+0x2E)
#define REG_TZPC_MIU0_BASE_ADDR_HIGH           (SEAL_TZPC_NONPM_MIU0+0x30)
#define REG_TZPC_MIU_ID_ENABLE                 (1<<12)

#define SEAL_NONPM_TBL_IP_NUM (384)

static int _gSealLocalDelayVar;
#define __delay() {int i; for (i=0; i<100000; i+=2) {i=i-1; _gSealLocalDelayVar+=i;}}

static unsigned long long DefaultNonSecClient[2] =
{
    0x0000000000000301, 0x0000000000000000
};

static unsigned long long DefaultSecClient[2] =
{
    0x0000FFFFFFFF0300, 0x0000000000000000
};

// startAddr, endAddr:  BUS address
void BOOT_SEAL_SetSecureRange(unsigned int startAddr, unsigned int endAddr)
{
    int i, j, offset;

    // Set up the configuration of miu ns table
    // Only Seucre ARM can modify it - 9'd377
    RIU[REG_TZPC_MIU0_ID0] = 377;
    (RIU[REG_TZPC_MIU0_CTL]) |= REG_TZPC_MIU_TOP_EN;

    // set up base address of miu secure range permission table
    RIU[REG_TZPC_MIU0_BASE_ADDR_LOW] = 0x0000;
    RIU[REG_TZPC_MIU0_BASE_ADDR_HIGH] = 0x0000;

    // - CryptoDMA (Secure CryptoDMA - 9'd383)
    // - Secure ARM (9'd377)
    // - NFIE
    // Only above AID could be configured to access secure area
    (RIU[REG_TZPC_MIU0_ID0]) |= REG_TZPC_MIU_ID_ENABLE;
    __delay();

    for (i=0, offset=0; i<SEAL_NONPM_TBL_IP_NUM; ++i, offset+=0x20)
    {
        if (i == 93 //(E_SEAL_NFIE0_PROT_NONPM)
            || i == 94 //(E_SEAL_NFIE1_PROT_NONPM)
            || i == 95 //(E_SEAL_NFIE2_PROT_NONPM)
            || i == 377 //(E_SEAL_ARM2MIU_SECURE_PROT_NONPM)
            || i == 383 ) //(E_SEAL_CRYPTODMA_GEN_SECURE_PROT_NONPM)
        {
            *(volatile unsigned long long*)(CONFIG_MIU0_BUSADDR + offset) = DefaultSecClient[0];
            *(volatile unsigned long long*)(CONFIG_MIU0_BUSADDR + offset + 8) = DefaultSecClient[1];
        }
        else
        {
            *(volatile unsigned long long*)(CONFIG_MIU0_BUSADDR + offset) = DefaultNonSecClient[0];
            *(volatile unsigned long long*)(CONFIG_MIU0_BUSADDR + offset + 8) = DefaultNonSecClient[1];
        }
    }
    __delay();

    (RIU[REG_TZPC_MIU0_ID0]) &= (~REG_TZPC_MIU_ID_ENABLE);
    __delay();

    // covert to PA
    startAddr -= CONFIG_MIU0_BUSADDR;
    endAddr -= CONFIG_MIU0_BUSADDR;

    // addr is 4KB shifted
    startAddr = startAddr >> 12;
    endAddr = endAddr >> 12;

    RIU[REG_SECURE_RANGE0_START_ADDR] = (startAddr & 0xFFFFULL);
    RIU[REG_SECURE_RANGE0_START_ADDR + 2] = ((startAddr>>16) & 0x000FULL);

    RIU[REG_SECURE_RANGE0_END_ADDR] = (endAddr & 0xFFFFULL);
    RIU[REG_SECURE_RANGE0_END_ADDR + 2] = ((endAddr>>16) & 0x000FULL);

    (RIU[REG_SECURE_RANGE0_END_ADDR + 2]) |= (0x8000);

    (RIU[REG_SECURE0_DETECT_ENABLE]) |= (1<<0);
}
