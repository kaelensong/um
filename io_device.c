#include "io_device.h"

uint8_t readbyte() 
{
        uint8_t input = getchar();
        return input;
}


void printbyte(int output) 
{
        int hello = putchar(output);
        (void) hello;
}