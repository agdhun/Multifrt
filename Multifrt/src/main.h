/*
 * main.h
 *
 * Created: 2015/8/11 11:30:28
 *  Author: agdhun
 */ 


#ifndef MAIN_H_
#define MAIN_H_

/** \name USART interface definitions
 * @{
 */
#define Mul_MODULE              SERCOM5
#define Mul_SERCOM_MUX_SETTING  USART_RX_1_TX_0_XCK_1
#define Mul_SERCOM_PINMUX_PAD0  PINMUX_PB16C_SERCOM5_PAD0
#define Mul_SERCOM_PINMUX_PAD1  PINMUX_PB17C_SERCOM5_PAD1
#define Mul_SERCOM_PINMUX_PAD2  PINMUX_UNUSED
#define Mul_SERCOM_PINMUX_PAD3  PINMUX_UNUSED
/** @} */

//! [module_inst]
struct usart_module usart_instance;
struct usart_module usart_inst1;
//! [module_inst]

#define TEM 't'
#define HUM 'h'
#define CO 'o'
#define CH4 'c'
#define Mul 'm'

int tem, hum, co, ch4;
uint8_t num[3];
int issuc = false;
static uint8_t mq2_init_value, mq7_init_value;

void configure_usart(void);
void Do(uint16_t dotemp, const uint16_t user);
void Send(uint8_t user);
void configure_usart1(void);

#endif /* MAIN_H_ */