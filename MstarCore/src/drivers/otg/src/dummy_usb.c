#include <dummy_usb.h>
//int gVar = 0;
const int gConstVar =123;
unsigned char dummy_usb_getbyte(void)
{
    unsigned char test = 0;
    int test1;
//    static int gStaticVar = 0;
//    gVar++;
//    gStaticVar++;
    test1 = gConstVar;
    return (test++)%128;
}
