/*
 * dht11.h
 *
 * Created: 2015/8/6 15:03:38
 *  Author: agdhun
 */ 
#ifndef DHT11_H_
#define DHT11_H_

#define DATA PIN_PB00

unsigned char U8FLAG, U8Temp;
unsigned char U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
unsigned char U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
unsigned char U8comdata;

struct port_config pinc;
struct port_config pini;

void Run_COM(void);
void RH(void);
int dht_run(const int DHT);

#endif /* DHT11_H_ */