/*
 * mq.h
 *
 * Created: 2015/8/6 15:02:19
 *  Author: agdhun
 */ 


#ifndef MQ_H_
#define MQ_H_

#define MQ2 ADC_POSITIVE_INPUT_PIN16
#define MQ7 ADC_POSITIVE_INPUT_PIN17

struct adc_module adc_instance;

int mq_run(const unsigned long MQ);

#endif /* MQ_H_ */