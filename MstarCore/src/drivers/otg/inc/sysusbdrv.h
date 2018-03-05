#ifndef __SYSUSBDRV_H__
#define __SYSUSBDRV_H__


//#define fpUsbCmdLoop        usbDrvFuncTab.loop
//#define fpSCSI_vendor       scsiCmdFuncTab.vend
//#define fpSCSI_disk         scsiCmdFuncTab.disk
//#define fpdisk_rom          diskCmdFuncTab.rom
//#define fpdisk_ram          diskCmdFuncTab.ram

//extern  void                *ptrusbStruct;

typedef void    (*fnUSBCmd)(void *pUsbStruct);

typedef struct usbDrvFuncTab_s
{
    fnUSBCmd        loop;       /* CmdLoop */
} usbDrvFuncTab_t;


/* SCSI commands function table */
typedef unsigned char	(*fnSCSI_Command)(void *pcbw, void *pcsw, void *pUsbStruct);

typedef struct scsiFuncTab_s
{
    fnSCSI_Command      vend;   /* vendor commands */
    fnSCSI_Command      disk;   /* disk commands */
} SCSIFuncTab_t;

/* SCSI disk command function table */
typedef struct DiskCmdFuncTab_s
{
    fnSCSI_Command      rom;    /* rom disk command */
    fnSCSI_Command      ram;    /* ram disk command */
} DiskCmdFuncTab_t;


extern  usbDrvFuncTab_t     usbDrvFuncTab;
extern SCSIFuncTab_t		scsiCmdFuncTab;
extern DiskCmdFuncTab_t		diskCmdFuncTab;

void    MDrv_USB_CmdLoop(void *pUsbStruct);
#endif
