#include "LPC17xx.h"
#include "RIT/RIT.h"
#include "timer/timer.h"
#include "sample.h"
#include "GLCD/GLCD.h"
#include "map/screen_drawer.h"


volatile int int0_pressed=0;
volatile int key1_pressed=0;
volatile int direction=0;
volatile int game_paused=1;
extern adc_off;


void RIT_IRQHandler (void) {		
	static int down=0;
	static int left=0;
	static int right=0;
	static int up=0;
	
	if (adc_off==0) {
		ADC_start_conversion();
	}
	
	if ((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
		/* Joytick DOWN pressed */
		down++;
		switch(down){
			case 1:
				direction=DOWN;
				break;
			default:
				break;
		}
	}
	else{
			down=0;
	}
	
	if ((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
		/* Joytick LEFT pressed */
		left++;
		switch(left){
			case 1:
				direction=LEFT;
				break;
			default:
				break;
		}
	}
	else{
			left=0;
	}
	
	if ((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
		/* Joytick RIGHT pressed */
		right++;
		switch(right){
			case 1:
				direction=RIGHT;
				break;
			default:
				break;
		}
	}
	else{
			right=0;
	}
	
	if ((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
		/* Joytick UP pressed */
		up++;
		switch(up){
			case 1:
				direction=UP;
				break;
			default:
				break;
		}
	}
	else{
			up=0;
	}
	
	
	/* button INT0 management */
	if (int0_pressed>=1){ 
		if ((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */
			switch(int0_pressed){				
				case 2:															  
					if (game_paused==1) { 					    
						game_paused=0;
						enable_timer(0);
						enable_timer(1);
					} else {                            
						disable_timer(1);
						game_paused=1;
					}
					break;
				default:
					break;
			}
			int0_pressed++;
		}
		else {																	  /* button released */
			int0_pressed=0;			
			NVIC_EnableIRQ(EINT0_IRQn);							
			LPC_PINCON->PINSEL4    |= (1 << 20); 
		}
	}
	
  LPC_RIT->RICTRL |= 0x1;											/* clear interrupt flag */
	
  return;
}
