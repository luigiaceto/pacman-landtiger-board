#include <string.h>
#include "LPC17xx.h"
#include "timer/timer.h"
#include "GLCD/GLCD.h"
#include "sample.h"
#include "map/screen_drawer.h"
#include <stdio.h>
#include "ghost/blinky.h"

extern int PAC_cell_i;
extern int PAC_cell_j;
volatile int just_teleported=0;
extern int direction;
extern int mat_map[R][C];
extern int score;
extern int life;
extern int game_time;
extern int remaining_pills;
extern int game_paused;
volatile int to_resume=0;
extern int power_pill_generation_istant;
extern int blinky_mode;
extern int blinky_cell_i;
extern int blinky_cell_j;
extern int blinky_frightened_until;
extern int blinky_respawn_instant;
extern int blinky_just_frightened;
volatile int blinky_speed_toggle=1; 


void TIMER0_IRQHandler (void) {
	int new_PAC_cell_i=PAC_cell_i;
	int new_PAC_cell_j=PAC_cell_j;
	
	if (game_paused==1) {
		GUI_Text(100, 150, (uint8_t *) "PAUSE", Red, White);
		to_resume=1;
		disable_timer(0);
		LPC_TIM0->IR = 1;
		return;
	}
	if (to_resume==1) {
		clear_pause_message();
		to_resume=0;
	}
	
	int cell=mat_map[PAC_cell_i][PAC_cell_j];	
	

	if (cell==TELEPORT && just_teleported==0) { 
		if (PAC_cell_j==0) new_PAC_cell_j=27;
		else new_PAC_cell_j=0;
		move_pac_man(PAC_cell_i, PAC_cell_j, new_PAC_cell_i, new_PAC_cell_j);
		PAC_cell_j=new_PAC_cell_j;
		
		LPC_TIM0->IR = 1;
		just_teleported=1;
		return;
	}
	if (just_teleported==1) just_teleported=0;

	if (cell==PILL) {
		mat_map[PAC_cell_i][PAC_cell_j]=EMPTY;
		score+=10;
		remaining_pills--;
	}
	else if (cell==POWER_PILL) {
		mat_map[PAC_cell_i][PAC_cell_j]=EMPTY;
		score+=50;
		remaining_pills--;
		if (!(blinky_cell_j==13 && (blinky_cell_i==13 || blinky_cell_i==12))) {
			blinky_mode=FRIGHTENED;
			blinky_just_frightened=1;
			blinky_frightened_until=game_time-10;
		}
	}
	

	if (PAC_cell_i==blinky_cell_i && PAC_cell_j==blinky_cell_j && blinky_mode==FRIGHTENED) {
		score+=100;
		blinky_mode=EATEN;
		blinky_frightened_until=-1;
		blinky_respawn_instant=game_time-3;
		blinky_cell_i=13;
		blinky_cell_j=13;
	}
	
	if (direction==UP) new_PAC_cell_i--;
	else if (direction==DOWN) new_PAC_cell_i++;
	else if (direction==RIGHT) new_PAC_cell_j++;
	else if (direction==LEFT) new_PAC_cell_j--;
	int new_cell=mat_map[new_PAC_cell_i][new_PAC_cell_j]; // cella su cui pacman deve spostarsi
	
	if (new_cell==WALL) {
		new_PAC_cell_i=PAC_cell_i;
		new_PAC_cell_j=PAC_cell_j;
	} 
	else {
		move_pac_man(PAC_cell_i, PAC_cell_j, new_PAC_cell_i, new_PAC_cell_j);
		PAC_cell_i=new_PAC_cell_i;
		PAC_cell_j=new_PAC_cell_j;
	}
	
	
	if ((score/(life*1000))>0) life++;
	
	char score_char[15]="";
	char life_char[15]="";
	
	sprintf(score_char, "SCORE %4d", score);
	GUI_Text(0, 0, (uint8_t *) score_char, White, Black);
	
	sprintf(life_char, "LIFES %2d", life);
	GUI_Text(0, 20, (uint8_t *) life_char, White, Black);
	

	if (blinky_mode==FRIGHTENED || game_time>40) {
		if (blinky_speed_toggle==1) {
			move_blinky();
			blinky_speed_toggle=0;
		} else {
			blinky_speed_toggle=1;
		}
	} else { move_blinky(); }
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}



void TIMER1_IRQHandler (void) {
	char game_time_char[5]="";
	
	if (game_time==power_pill_generation_istant) {
		spawn_power_pills();
	}
	
	if (game_time>=0) {
		sprintf(game_time_char, "%4d", game_time);
		GUI_Text(200, 0, (uint8_t *) game_time_char, White, Black);
	}
	
	
	if (game_time==0 || life<=0) {
		disable_timer(0);
		disable_timer(1);
		LCD_Clear(Black);
		if (remaining_pills==0) {
			GUI_Text(100, 150, (uint8_t *) "VICTORY", Red, White);
		} else {
			GUI_Text(100, 150, (uint8_t *) "GAME OVER", Red, White);
		}
	}
	
	game_time--;
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}
