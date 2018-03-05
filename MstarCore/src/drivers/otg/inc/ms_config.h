#ifndef _MS_CONFIG_H_
#define _MS_CONFIG_H_

#define UTMIBaseAddress  0x1f200000+(0x3a80*2)
#define otgRegAddress      0x1f200000+(0x11700*2)
#define USBCBaseAddress 0x1f200000+(0x0700*2)


#define OffShift            1
#define otgNumEPDefs        4
#define SCSI_BLOCK_NUM      5000
#define SCSI_BLOCK_SIZE     512
#define MONTAGE_EVB
//#define MASS_BUFFER_SIZE    (4 * 1024)
#define MAX_DMA_CHANNEL     1
#endif  /* _MS_CONFIG_H_ */
