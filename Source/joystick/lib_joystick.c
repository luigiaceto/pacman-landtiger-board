#include "LPC17xx.h"
#include "joystick.h"

/*----------------------------------------------------------------------------
  Function that initializes joysticks and switch them off
 *----------------------------------------------------------------------------*/

void joystick_init(void) {
	// Joystick Down (P1.26)
  LPC_PINCON->PINSEL3 &= ~(3 << 20);   
  LPC_GPIO1->FIODIR   &= ~(1 << 26);  

  // Joystick Left (P1.27)
  LPC_PINCON->PINSEL3 &= ~(3 << 22);   
  LPC_GPIO1->FIODIR   &= ~(1 << 27);   

  // Joystick Right (P1.28)
  LPC_PINCON->PINSEL3 &= ~(3 << 24);   
  LPC_GPIO1->FIODIR   &= ~(1 << 28);   
	
	// Joystick Up (P1.29)
  LPC_PINCON->PINSEL3 &= ~(3<<26);		 
	LPC_GPIO1->FIODIR   &= ~(1<<29);		  
}
