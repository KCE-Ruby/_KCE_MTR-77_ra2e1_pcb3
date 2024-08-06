/**
 ******************************************************************************
 * @file    ADC.h
 * @author  KCE R&D Team
 * @date    2024-06-10
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

#define INC_ADC_H_ 
#ifdef INC_ADC_H_
/* task function protocol -----------------------------------------------*/
void ADC_Main(void);
void r_adc_callback(adc_callback_args_t * p_args);

#endif /* INC_ADC_H_ */
