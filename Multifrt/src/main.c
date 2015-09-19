/**
 * \file
 *
 * \brief SAM USART Quick Start
 *
 * Copyright (C) 2012-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
//注意事项：
//MQ的引脚使用前务必与实际接线一致，务必修改，位置定义在driver/mq/mq.h
//DATA为DHT11的数据接口，务必与实物连接图保持一致，定义在/driver/dht11/dht11.h
//esp8266通过 串口发送数据时务必修改rx_bufer的长度，定义位于/driver/uart/uart.h
//务必修改usart_write_callback函数

#include <asf.h>
#include "dri.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
	
//! [setup_init]
	system_init();

	//延时、串口初始化
	delay_init();
	configure_usart();
	configure_usart1();
	
	//配置输入输出变量
	port_get_config_defaults(&pini);
	port_get_config_defaults(&pinc);
	pinc.direction = PORT_PIN_DIR_OUTPUT;
	
	//延时20s，使MQ2、MQ7充分初始化
	delay_s(20);
	
	//MQ2、MQ7初始化输出模拟电压值
	mq2_init_value = mq_run(MQ2);
	mq7_init_value = mq_run(MQ7);
	
	//对ESP8266进行WIFI配置初始化
	esp_init();
//! [setup_init]

	//port_group_set_output_level(LED_0_PIN, LED_0_ACTIVE);
	
//! [main]
	//定义串口接收变量，用户端口号
	uint16_t temp='H', user = '*';
	
//! [main_loop]
	while (true) {
		if(usart_read_wait(&usart_instance, &temp) == STATUS_OK)
		{
			delay_us(8);
			if(temp == 'D')
			{
				while(usart_write_wait(&usart_inst1, 'T')!=STATUS_OK){}
				if(usart_read_wait(&usart_instance, &temp) == STATUS_OK)
				{
					
					if(temp == ',')
					{
						while(usart_write_wait(&usart_inst1, 'E')!=STATUS_OK){}
						if(usart_read_wait(&usart_instance, &temp) == STATUS_OK)
						{
							user = temp;
							while(usart_write_wait(&usart_inst1, 'S')!=STATUS_OK){}
							while(true)
							{
								if(usart_read_wait(&usart_instance, &temp) == STATUS_OK)
								{
									if(islower(temp))
									{
										while(usart_write_wait(&usart_inst1, 'T')!=STATUS_OK){}
										Do(temp, user);
										break;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	//! [main]
}

//! [setup]
void configure_usart(void)
{
	//! [setup_config]
	struct usart_config config_usart;
	//! [setup_config]
	//! [setup_config_defaults]
	usart_get_config_defaults(&config_usart);
	//! [setup_config_defaults]

	//! [setup_change_config]
	config_usart.baudrate    = 115200;
	config_usart.mux_setting = Mul_SERCOM_MUX_SETTING;
	config_usart.pinmux_pad0 = Mul_SERCOM_PINMUX_PAD0;
	config_usart.pinmux_pad1 = Mul_SERCOM_PINMUX_PAD1;
	config_usart.pinmux_pad2 = Mul_SERCOM_PINMUX_PAD2;
	config_usart.pinmux_pad3 = Mul_SERCOM_PINMUX_PAD3;
	//! [setup_change_config]

	//! [setup_set_config]
	while (usart_init(&usart_instance,
	Mul_MODULE, &config_usart) != STATUS_OK) {
	}
	//! [setup_set_config]

	//! [setup_enable]
	usart_enable(&usart_instance);
	//! [setup_enable]
}
//! [setup]

void Do(const uint16_t dotemp, const uint16_t user)
{
	
	switch(dotemp)
	{
		case TEM:
			tem = dht_run(TEM);
			num[0] = tem / 10 + '0';
			num[1] = tem % 10 + '0';
			num[2]='\n';
			Send(user);
			delay_ms(20);
			break;
			
			//湿度
		case HUM:
			hum = dht_run(HUM);
			num[0] = hum / 10 +'0';
			num[1] = hum % 10 +'0';
			num[2] = '\n';
			Send(user);
			delay_ms(20);
			break;
			
			//CO相对浓度
		case CO://****待完善
			co =  mq2_init_value;
			num[0] = co / 10 + '0';
			num[1] = co % 10 + '0';
			num[2] = '\n';
			Send(user);
			co = mq_run(MQ7);
			num[0] = co / 10 + '0';
			num[1] = co % 10 + '0';
			num[2] = '\n';
			Send(user);
			break;
			
			//CH4相对浓度
		case CH4://**daiwanshang
			ch4 =  mq7_init_value;
			num[0] = ch4 / 10 + '0';
			num[1] = ch4 % 10 + '0';
			num[2] = '\n';
			Send(user);
			ch4 = mq_run(MQ7);
			num[0] = ch4 / 10 + '0';
			num[1] = ch4 % 10 + '0';
			num[2] = '\n';
			Send(user);
			break;
		default:;
	}
}

void Send(uint8_t user)
{
	uint16_t stemp;
	uint8_t sn = true;
	uint8_t cnt = 1;
	uint8_t Temp[17];
	sprintf(Temp, "AT+CIPSEND=%c,3\r\n", user);
	
	while(usart_write_buffer_job(&usart_instance, Temp, sizeof(Temp)) != STATUS_OK){}
	while(sn)
	if(usart_read_wait(&usart_instance, &stemp) == STATUS_OK)
	{
		if(stemp == '>')
		{
			while(usart_write_buffer_wait(&usart_instance, num, sizeof(num)) != STATUS_OK){}
			while(true)
			if(usart_read_wait(&usart_instance, &stemp) == STATUS_OK)
			{
				if(stemp == 'K')
				{
					sn = false;
					break;
				}
				else if(cnt == 1000)
				{
					Send(user);
					sn = false;
					break;
				}
				cnt ++;
			}
		}
	}
}

void configure_usart1(void)
{
	//! [setup_config]
	struct usart_config config_usart;
	//! [setup_config]
	//! [setup_config_defaults]
	usart_get_config_defaults(&config_usart);
	//! [setup_config_defaults]

	//! [setup_change_config]
	config_usart.baudrate    = 115200;
	config_usart.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	config_usart.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	config_usart.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	config_usart.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	config_usart.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
	//! [setup_change_config]

	//! [setup_set_config]
	while (usart_init(&usart_inst1,
	EDBG_CDC_MODULE, &config_usart) != STATUS_OK) {
	}
	//! [setup_set_config]

	//! [setup_enable]
	usart_enable(&usart_inst1);
	//! [setup_enable]
}