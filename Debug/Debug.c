#include <stdio.h>
#include "common.h"
#include "Debug.h"

void print_binary_8(uint8_t bin)
{
    uint8_t z = 0b10000000;
    while (z>0)
    {
        if (bin & z)
           printf("1");
        else
            printf("0");
            z>>=1;
    }
}

void print_binary_16(uint16_t bin)
{
    uint16_t z = 0b1000000000000000;
    while (z>0)
    {
        if (bin & z) {
            printf("1");
        }
        else {
            printf("0");
        }
        z>>=1;
    }
}
