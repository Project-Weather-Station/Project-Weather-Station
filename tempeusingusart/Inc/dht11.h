
#ifndef DHT11_H
#define DHT11_H
#include<stdio.h>
void DHT11_Init(void);
int DHT11_Read(int *temperature,int *humidity);


#endif /* DHT11_H_ */
