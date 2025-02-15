#include "button.h"
#include "LPC17xx.h"

/**
 * @brief  Function that initializes Buttons
 */
void BUTTON_init(void) {
	
	// INT0
  LPC_PINCON->PINSEL4    |= (1 << 20);		 /* External interrupt 0 pin selection */
  LPC_GPIO2->FIODIR      &= ~(1 << 10);    /* PORT2.10 defined as input          */
	
  LPC_SC->EXTMODE = 0x7;
				 
  NVIC_EnableIRQ(EINT0_IRQn);              
	NVIC_SetPriority(EINT0_IRQn, 3);
}
