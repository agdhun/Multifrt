/*
 * mq.c
 *
 * Created: 2015/8/6 15:01:37
 *  Author: agdhun
 */ 
#include "mq.h"
#include <asf.h>

void configure_adc(const unsigned long ADC_PIN);
void adc_get_config_defaults_n(struct adc_config *const config, const unsigned long ADC_PIN);

int mq_run(const unsigned long MQ)
{
	uint16_t result;
	configure_adc(MQ);
	adc_start_conversion(&adc_instance);
	do {
		/* Wait for conversion to be done and read out result */
	} while (adc_read(&adc_instance, &result) == STATUS_BUSY);
	//! [get_res]
	//预设参考电压是3.3v
	result = result*33/4096;
	adc_disable(&adc_instance);
	return result;
}
void configure_adc(const unsigned long ADC_PIN)
{
	//! [setup_config]
	struct adc_config config_adc;
	//! [setup_config]
	//! [setup_config_defaults]
	adc_get_config_defaults_n(&config_adc, ADC_PIN);
	//! [setup_config_defaults]

	//! [setup_set_config]
	adc_init(&adc_instance, ADC, &config_adc);
	//! [setup_set_config]

	//! [setup_enable]
	adc_enable(&adc_instance);
	//! [setup_enable]
}

void adc_get_config_defaults_n(struct adc_config *const config, const unsigned long ADC_PIN)
{
	Assert(config);
	config->clock_source                  = GCLK_GENERATOR_0;
	config->reference                     = ADC_REFERENCE_AREFB;
	config->clock_prescaler               = ADC_CLOCK_PRESCALER_DIV4;
	config->resolution                    = ADC_RESOLUTION_12BIT;
	config->window.window_mode            = ADC_WINDOW_MODE_DISABLE;
	config->window.window_upper_value     = 0;
	config->window.window_lower_value     = 0;
	config->gain_factor                   = ADC_GAIN_FACTOR_1X;
	config->positive_input                = ADC_PIN;
	config->negative_input                = ADC_NEGATIVE_INPUT_GND ;
	config->accumulate_samples            = ADC_ACCUMULATE_DISABLE;
	config->divide_result                 = ADC_DIVIDE_RESULT_DISABLE;
	config->left_adjust                   = false;
	config->differential_mode             = false;
	config->freerunning                   = false;
	config->event_action                  = ADC_EVENT_ACTION_DISABLED;
	config->run_in_standby                = false;
	config->reference_compensation_enable = false;
	config->correction.correction_enable  = false;
	config->correction.gain_correction    = ADC_GAINCORR_RESETVALUE;
	config->correction.offset_correction  = ADC_OFFSETCORR_RESETVALUE;
	config->sample_length                 = 0;
	config->pin_scan.offset_start_scan    = 0;
	config->pin_scan.inputs_to_scan       = 0;
}