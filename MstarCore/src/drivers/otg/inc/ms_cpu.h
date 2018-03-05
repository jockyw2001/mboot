#ifndef _MS_CPU_H_
#define _MS_CPU_H_

#define FIFO_ADDRESS(e)  (otgRegAddress + (e<<3) + M_FIFO_EP0)
#define FIFO_DATA_PORT  (otgRegAddress+M_REG_FIFO_DATA_PORT)

#define SWOP(X)  (X = X)
#endif  /* _MS_CPU_H_ */
