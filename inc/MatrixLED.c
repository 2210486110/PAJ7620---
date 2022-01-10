#include <reg52.h>
#include <74HC595.h>
#include <Delay.h>

void MatrixLED_Show_Column(unsigned char Column,unsigned char DATA)
{
    _74HC595_Write_Byte(DATA);
    P0 = ~(0x80>>Column);
    Delay(1);
    P0 = 0xFF;
}