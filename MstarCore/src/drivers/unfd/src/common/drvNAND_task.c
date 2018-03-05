#include "drvNAND.h"
#include "drvNAND_utl.h"

#if defined(__VER_UNFD_FTL__)&&__VER_UNFD_FTL__


struct unfd_task_queue {
	unsigned int		flags;
	void			*data;
} unfd_tq;

void drvNAND_BGThread_Ex(void)
{
	static int all_flag=0;
    int cur_flag;

    cur_flag = unfd_tq.flags & ~all_flag;
    all_flag |= unfd_tq.flags;
    
	#if UNFD_FTL_WL
	if(cur_flag & NAND_MSG_WEAR_LEVELING)
	{
		nand_Wear_Leveling();
		all_flag &= ~NAND_MSG_WEAR_LEVELING;
	}

	if(cur_flag & NAND_MSG_WEAR_LEVELING1)
	{
		nand_Wear_Leveling1();
        all_flag &= ~NAND_MSG_WEAR_LEVELING1;
	}
	#endif
	if(cur_flag & NAND_MSG_FORCE_WRITE_BACK)
	{
		NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
		U8  u8_i;

		#if UNFD_FTL_WL
        for (u8_i=0 ; u8_i<(pNandDrv->u8_Zone1SubZoneCnt+1); u8_i++)
		{
			if(pNandDrv->au32_ZoneTotalECnt[u8_i] > WL_SAVE_EC_TIMES)
				nand_SaveEraseCounter(u8_i);
		}
		#endif

        for(u8_i=0; u8_i<MAX_WBQ_CNT; u8_i++)
		{
			nand_FlushWBQ(u8_i);
		}

        all_flag &= ~NAND_MSG_FORCE_WRITE_BACK;
	}
	
}


void drvNAND_BGThread(int flag)
{
    //=========================
    // [if kernel]: down(&PfModeSem);
    //   need a semaphore to lock here
    // [or MBoot]:
    #if defined(NAND_DRV_TV_MBOOT) && NAND_DRV_TV_MBOOT
    if(unfd_tq.flags)
        return;
    #endif
    //=========================
    
	unfd_tq.flags = flag;
    drvNAND_BGThread_Ex();

/*
	if(flag & NAND_MSG_WEAR_LEVELING)
	{
		nand_Wear_Leveling();
	}

	if(flag & NAND_MSG_WEAR_LEVELING1)
	{
		nand_Wear_Leveling1();
	}

	if(flag & NAND_MSG_FORCE_WRITE_BACK)
	{
		NAND_DRIVER *pNandDrv = drvNAND_get_DrvContext_address();
		U8  u8_i;

        for (u8_i=0 ; u8_i<(pNandDrv->u8_Zone1SubZoneCnt+1); u8_i++)
		{
			if(pNandDrv->au32_ZoneTotalECnt[u8_i] > WL_SAVE_EC_TIMES)
				nand_SaveEraseCounter(u8_i);
		}

        for(u8_i=0; u8_i<MAX_WBQ_CNT; u8_i++)
		{
			nand_FlushWBQ(u8_i);
		}

	}
*/

    //=========================
    #if defined(NAND_DRV_TV_MBOOT) && NAND_DRV_TV_MBOOT
    unfd_tq.flags = 0;
    #endif
    //up(&PfModeSem);
    //=========================
}


int drvNAND_BGThread_Init(void)
{
	return 0;
}
#endif
