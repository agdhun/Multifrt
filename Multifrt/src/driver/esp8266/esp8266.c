/*
 * esp8266.c
 *
 * Created: 2015/8/6 17:13:24
 *  Author: agdhun
 */ 
#include <asf.h>
#include "esp8266.h"

extern struct usart_module usart_instance;

uint8_t CIPMUX[]="AT+CIPMUX=1\r\n";							//����������ģʽ
uint8_t CIPSERVER[]="AT+CIPSERVER=1,7671\r\n";				//�������������˿ں���Ϊ7671
uint8_t CIPSTO[]="AT+CIPSTO=50\r\n";						//���÷�����50s��ʱ

void esp_init(void)
{
	int cnt = 1;
	uint16_t temp;
	
	//����ָ�������Ӧ����δ�ɹ������·���
	while(!issuc)
	if(usart_write_buffer_wait(&usart_instance, CIPMUX, sizeof(CIPMUX)) == STATUS_OK)
	{
		while(true)
		{
			if(usart_read_wait(&usart_instance,&temp) == STATUS_OK)
			
			if(temp =='K')
			{
				
				delay_ms(1);
				issuc = true;
				break;
			}
			cnt ++;
			if(cnt == 1000)
			{
				issuc = false;
				break;
			}
		}
	}
	
	cnt = 1;
	issuc = false;

	//����ָ�������Ӧ����δ�ɹ������·���
	while(!issuc)
	if(usart_write_buffer_wait(&usart_instance, CIPSERVER, sizeof(CIPSERVER)) == STATUS_OK)
	{
		
		while(true)
		{
			if(usart_read_wait(&usart_instance,&temp) == STATUS_OK)
			
			if(temp =='K')
			{
				
				delay_ms(1);
				issuc = true;
				break;
			}
			cnt ++;
			if(cnt == 1000)
			{
				issuc = false;
				break;
			}
		}
	}
	//SendCom(CIPSTO);
}