#include <reg52.h>

__sbit __at(0xB0+4) SER;
__sbit __at(0xB0+5) RCK;
__sbit __at(0xB0+6) SRCK;

void _74HC595_Write_Byte(unsigned char DATA)
{
    SRCK = 0;
    RCK = 0;
    for(unsigned char i = 0;i<8;i++)
    {
        SER = DATA&(0x80>>i);
        SRCK = 1;
        SRCK = 0;
    }
    RCK = 1;
    RCK = 0;
}