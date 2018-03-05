#ifndef _USB_FUNCTION_H_
#define _USB_FUNCTION_H_

//#include "type.h"

#define M_FTR_TESTJ                     0x0100
#define M_FTR_TESTK                     0x0200
#define M_FTR_TESTSE0                   0x0300
#define M_FTR_TESTPKT                   0x0400

/* MSD BOT Functional Characteristics */
#define MSDFN_BOT_RESET                 0xFF
#define MSDFN_BOT_GET_MAXLUN            0xFE

#define DEV_DESC_EP0_SIZE_OFFSET    8   /* where to find max ep0 in dscrptor*/
#define SETLSB(x,y) (x) = (((x) & 0xff00) | ((y) & 0xff))
#define SETMSB(x,y) (x) = (((x) & 0xff) | (((y) & 0xff) << 8))
#define GETLSB(x)   ((x) & 0xff)
#define GETMSB(x)   GETLSB((x)>>8)

/*
 * USB Set Features
 */
#define	ENDPOINT_HALT			        0x00
#define	DEVICE_REMOTE_WAKEUP		    0x01
#define	TEST_MODE			            0x02

#define USB_SUCCESS                         0x00
#define USB_FAILURE                         -1

typedef	U8	pipe_t;

void USB_Parse_DRC_Int_Peripheral(drcintitem_t *dP,USB_VAR *gUSBStruct);
S32 USB_Parse_Received_Setup(USB_VAR *gUSBStruct);
#endif
