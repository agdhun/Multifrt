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
		//��ʱ������forѭ��
		if(U8FLAG==1)break;
		//�ж�����λ��0����1
		// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1
		U8comdata<<=1;
		U8comdata|=U8Temp; //0
	}//rof
}

//------------  --------------------
//-----ʪ�ȶ�ȡ�ӳ��� ------------
//----------------------  ----------
//----���±�����Ϊȫ�ֱ���--------
//----�¶ȸ�8λ== U8T_data_H------
//----�¶ȵ�8λ== U8T_data_L------
//----ʪ�ȸ�8λ== U8RH_data_H-----
//----ʪ�ȵ�8λ== U8RH_data_L-----
//----У�� 8λ == U8checkdata-----
//----��������ӳ�������----------
//---- delay_us();, delay_ms();COM();
//---------------------    -----------
void RH(void)
{
	//��������18ms
	port_pin_set_config(DATA, &pinc);
	port_pin_set_output_level(DATA, false);
	delay_ms(18);
	port_pin_set_output_level(DATA, true);
	//������������������ ������ʱ20us
	delay_us(20);
	//������Ϊ���� �жϴӻ���Ӧ�ź�
	port_pin_set_output_level(DATA, true);
	port_pin_set_config(DATA, &pini);
	//�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������
	
	if(!port_pin_get_input_level(DATA)) //T !
	{
		U8FLAG=2;
		//�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����
		
		while((!port_pin_get_input_level(DATA))&&U8FLAG++);
		U8FLAG=2;
		
		//�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
		while((port_pin_get_input_level(DATA))&&U8FLAG++);
		//���ݽ���״̬
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
		//����У��
		
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