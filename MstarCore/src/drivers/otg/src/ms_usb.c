#include "../inc/ms_msd_global.h"
#include "../inc/ms_cpu.h"
#include "../inc/ms_drc.h"

S8 USB_Send_Stall(U8 ep,USB_VAR *gUSBStruct)
{
    U32 csr, reg, bit;
    U32 timeout = 0;

    if (gUSBStruct->otgUSB_EP[ep].BltEP == 0)
    {
        reg = 0x102*2;//M_REG_CSR0;
          bit = M_CSR0_P_SENDSTALL;
    }
    else
    {
        //USB_DRC_Index_Select(gUSBStruct->otgUSB_EP[ep].BltEP);

        if (gUSBStruct->otgUSB_EP[ep].DRCDir)   /* RX */
        {
            reg = 0x126*2;//M_REG_RXCSR1;
            bit = M_RXCSR1_P_SENDSTALL;
        }
        else             /* TX */
        {
            reg = 0x112*2;//M_REG_TXCSR1;
            bit = M_TXCSR1_P_SENDSTALL;
        }
    }

    csr = USB_REG_READ8(reg);
    USB_REG_WRITE8(reg, (csr | bit));
    if (gUSBStruct->otgUSB_EP[ep].DRCDir)   /* RX */
    {
//        printf("R:%x \n",USB_REG_READ8(reg));
        while((USB_REG_READ8(0x126*2/*M_REG_RXCSR1*/)&M_RXCSR1_P_SENDSTALL)&&(gUSBStruct->u8USBDeviceMode==1))
        {
            /* wait 1 second timeout */
            if(timeout > 100000)
                break;
            timeout++;
            //hal_delay_us(10);
        };
    }
    else             /* TX */
    {
//        printf("T:%x \n",USB_REG_READ8(reg));
		while (!OTG_Interrupt_Polling(0x20, gUSBStruct)) ;//polling ep0 interrupt
        while((USB_REG_READ8(0x112*2/*M_REG_TXCSR1*/)&M_TXCSR1_P_SENDSTALL)&&(gUSBStruct->u8USBDeviceMode==1))
        {
            /* wait 1 second timeout */
            if(timeout > 100000)
                break;
            timeout++;
            //hal_delay_us(10);
        };
    }
    return 1;
}

void USB_SWOP_Setup(struct devrequest *sP)
{
    SWOP(sP->wValue);
    SWOP(sP->wIndex);
    SWOP(sP->wLength);
}

U8 otgIsUSBConfiged(USB_VAR *gUSBStruct)
{
      if (gUSBStruct->otgUSBState==USB_CONFIGURED)
          return 1;
      else
          return 0;
}

void USB_Change_USB_State(U8 toUSB,USB_VAR *gUSBStruct)
{
    switch  (toUSB)
    {
        case USB_POWERED:   /* HOST:CONNECT DRC INT;PER:exit active mode*/
            gUSBStruct->otgSuspended = 0;
            break;
        case USB_DEFAULT:   /* HOST:Reset complete;PER: RESET DRC INT */
            gUSBStruct->otgHNPEnabled = 0;
            gUSBStruct->otgHNPSupport = 0;
            gUSBStruct->otgRemoteWakeup = 0;
            gUSBStruct->otgSuspended = 0;
            gUSBStruct->u8USBDeviceMode = 0;	// 20110310
            break;
        case USB_ADDRESS:
            gUSBStruct->otgConfig = 0;
            break;
        case USB_CONFIGURED:
         gUSBStruct->u8USBDeviceMode = 1;
            //DBG_MSG("USB Configured\n");
            break;
        default:
            break;
    }
    gUSBStruct->otgUSBState = toUSB;
}
