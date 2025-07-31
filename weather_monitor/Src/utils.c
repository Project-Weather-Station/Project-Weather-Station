/*
 * utils.c
 *
 *  Created on: Jul 24, 2025
 *      Author: sunbeam
 */


#include<stdint.h>


void int_to_str(uint8_t num, char *buf)
{
    int i = 0;
    if (num == 0)
    {
        buf[i++] = '0';
    }
    else
    {
        while (num)
        {
            buf[i++] = (num % 10) + '0';
            num /= 10;
        }
    }
    buf[i] = '\0';

    for (int j = 0; j < i / 2; j++)
    {
        char tmp = buf[j];
        buf[j] = buf[i - j - 1];
        buf[i - j - 1] = tmp;
    }
}
