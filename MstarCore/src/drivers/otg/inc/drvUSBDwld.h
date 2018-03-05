#ifndef _DRV_USBDWLD_H
#define _DRV_USBDWLD_H

#define XROM_USB_UPLOAD_FIXEDLEN    0x00
#define XROM_USB_DOWNLOAD_KEEP      0x01
#define XROM_USB_UPLOAD_SIZE        0x02
#define XROM_USB_ERR_CODE           0x03
#define XROM_USB_DOWNLOAD_END       0x04
#define XROM_USB_SEL_ROMRAM         0x05
#define XROM_USB_UPLOAD_VARLEN      0x06
#define XROM_USB_UPLOAD_LOG	    0x16

void MDrv_USB_Init(void);
u8 usb_dongle_detect(void);
void ebable_UPLL(void);

#endif  // _DRV_USBDWLD_H
