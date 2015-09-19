/*
 * dht11.c
 *
 * Created: 2015/8/6 15:03:54
 *  Author: agdhun
 */ 
#include "dht11.h"
#include <asf.h>

int dht_run(const int DHT)
{
	int temp;
	RH();
	delay_ms(10);
	switch(DHT)
	{
		case 'm':
			temp = U8T_data_H + 100 * U8RH_data_H;break;
		case 't':
			temp = U8T_data_H;break;
		case 'h':
			temp = U8RH_data_H;break;
		default:
			temp = U8T_data_H + 100 * U8RH_data_H;break;
	}
	return temp;
	//U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L
}

void Run_COM(void)
{
	unsigned char  i;
	for(i=0;i<8;i++)
	{
		U8FLAG=2;
		
		while((!port_pin_get_input_level(DATA))&&U8FLAG++);
		delay_us(15);
		U8Temp=0;
		if(port_pin_get_input_level(DATA))U8Temp=1;
		U8FLAG=2;
		while((port_pin_get_input_level(DATA))&&U8FLAG++);
		//超时则跳出for循环
		if(U8FLAG==1)break;
		//判断数据位是0还是1
		// 如果高电平高过预定0高电平值则数据位为 1
		U8comdata<<=1;
		U8comdata|=U8Temp; //0
	}//rof
}

//------------  --------------------
//-----湿度读取子程序 ------------
//----------------------  ----------
//----以下变量均为全局变量--------
//----温度高8位== U8T_data_H------
//----温度低8位== U8T_data_L------
//----湿度高8位== U8RH_data_H-----
//----湿度低8位== U8RH_data_L-----
//----校验 8位 == U8checkdata-----
//----调用相关子程序如下----------
//---- delay_us();, delay_ms();COM();
//---------------------    -----------
void RH(void)
{
	//主机拉低18ms
	port_pin_set_config(DATA, &pinc);
	port_pin_set_output_level(DATA, false);
	delay_ms(18);
	port_pin_set_output_level(DATA, true);
	//总线由上拉电阻拉高 主机延时20us
	delay_us(20);
	//主机设为输入 判断从机响应信号
	port_pin_set_output_level(DATA, true);
	port_pin_set_config(DATA, &pini);
	//判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行
	
	if(!port_pin_get_input_level(DATA)) //T !
	{
		U8FLAG=2;
		//判断从机是否发出 80us 的低电平响应信号是否结束
		
		while((!port_pin_get_input_level(DATA))&&U8FLAG++);
		U8FLAG=2;
		
		//判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
		while((port_pin_get_input_level(DATA))&&U8FLAG++);
		//数据接收状态
		Run_COM();
		
		U8RH_data_H_temp=U8comdata;
		Run_COM();
		U8RH_data_L_temp=U8comdata;
		Run_COM();
		U8T_data_H_temp=U8comdata;
		Run_COM();
		U8T_data_L_temp=U8comdata;
		Run_COM();
		U8checkdata_temp=U8comdata;
		port_pin_set_config(DATA, &pinc);
		port_pin_set_output_level(DATA, true);
		//数据校验
		
		U8Temp=(U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
		
		if(U8Temp==U8checkdata_temp)
		{
			
			U8RH_data_H=U8RH_data_H_temp;
			U8RH_data_L=U8RH_data_L_temp;
			U8T_data_H=U8T_data_H_temp;
			U8T_data_L=U8T_data_L_temp;
			U8checkdata=U8checkdata_temp;
			
		}//fi
	}//fi
}