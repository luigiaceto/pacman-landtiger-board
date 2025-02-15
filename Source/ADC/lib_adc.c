#include "LPC17xx.h"
#include "adc.h"

volatile int adc_off=0;

/*----------------------------------------------------------------------------
  Function that initializes ADC
 *----------------------------------------------------------------------------*/
void ADC_init (void) {

  LPC_PINCON->PINSEL3 |=  (3UL<<30);    /* P1.31 is AD0.5                     */

  LPC_SC->PCONP       |=  (1<<12);      /* Enable power to ADC block          */

  LPC_ADC->ADCR        =  (1<< 5) |     /* select AD0.5 pin                   */
                          (4<< 8) |     /* ADC clock is 25MHz/5               */
                          (1<<21);      /* enable ADC                         */ 

  LPC_ADC->ADINTEN     =  (1<< 8);      /* global enable interrupt            */

  NVIC_EnableIRQ(ADC_IRQn);             /* enable ADC Interrupt               */
}

void ADC_start_conversion (void) {
	LPC_ADC->ADCR |=  (1<<24);            /* Start A/D Conversion 				      */
}				 

void ADC_turn_off() {
	LPC_SC->PCONP &= ~(1 << 12);  				/* Spegne l'alimentazione al blocco ADC */
	adc_off=1;
}
