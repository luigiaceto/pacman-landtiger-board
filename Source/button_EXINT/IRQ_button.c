#include "button.h"
#include "LPC17xx.h"

extern int int0_pressed;
extern int key1_pressed;

void EINT0_IRQHandler (void) {	  					/* INT0														 */		
	NVIC_DisableIRQ(EINT0_IRQn);
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection 						 */
	int0_pressed=1;
	LPC_SC->EXTINT &= (1 << 0);     					/* clear pending interrupt         */
}

void EINT1_IRQHandler (void) {	  		      /* KEY1														 */
	
	LPC_SC->EXTINT &= (1 << 1);               /* clear pending interrupt         */
}

void EINT2_IRQHandler (void) {	  	        /* KEY2														 */

  LPC_SC->EXTINT &= (1 << 2);               /* clear pending interrupt         */    
}
