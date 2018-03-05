#ifndef _OAD_SCAN_H_
#define _OAD_SCAN_H_ 1

typedef struct _chaninfo
{
    MS_U32   m_frequency;
    MS_U16   m_chanNum;
    MS_U16   m_bandwith;
    MS_U8    m_countryType;
    MS_U16   m_u16OADPid;
}CHANINFO_T;


int OAD_SCAN_GetNextChanInfo(CHANINFO_T *pChanInfo);

int OAD_SCAN_ChanTryLock(CHANINFO_T *pChanInfo);


int OAD_SCAN_ShowPrompt(void);

int OAD_SCAN_ShowProgress(MS_U8 precent);

int MApp_BL_OADGetRFChanInfo(CHANINFO_T *pRFChan);

int MApp_BL_FindOADChan(MS_U16 *pOADPid);

int MApp_BL_OADGetRFChanInfo(CHANINFO_T *pRFChan);




#endif
