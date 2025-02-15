#include "screen_drawer.h"
#include "GLCD/GLCD.h" 
#include "AsciiLib.h"
#include <stdlib.h>
#include "sample.h"
#include "timer/timer.h"

extern int mat_map[R][C];
extern int AD_current;
volatile int power_pill_generation_istant;
volatile int remaining_pills=0; 


/* dato i e j della matrice restituisce il pixel logico rispettivo sulla mappa */
void map_cell_to_screen(int i, int j, int* x, int* y) {
	static int base_x=24;
	static int base_y=57;
	
	*x=base_x+j*7;
	*y=base_y+i*7;
}

/* disegna un blocco di muro */
void draw_wall(int i, int j) {
	int x, y;
	
	map_cell_to_screen(i, j, &x, &y);
	LCD_DrawLine(x-3, y-3, x+3, y-3, Blue);
	LCD_DrawLine(x-3, y-2, x+3, y-2, Blue);
	LCD_DrawLine(x-3, y-1, x+3, y-1, Blue);
	LCD_DrawLine(x-3, y, x+3, y, Blue);
	LCD_DrawLine(x-3, y+1, x+3, y+1, Blue);
	LCD_DrawLine(x-3, y+2, x+3, y+2, Blue);
	LCD_DrawLine(x-3, y+3, x+3, y+3, Blue);
}

/* disegna una pill normale sullo schermo conoscendo posizione della cella */
void draw_pill(int i, int j) {
	int x, y;
	
	map_cell_to_screen(i, j, &x, &y);
	LCD_DrawLine(x, y, x, y, White);
}

/* disegna i muri e le pill nella mappa di gioco */
void draw_map() {
	int i, j;
	
	srand(AD_current);
	ADC_turn_off();
	power_pill_generation_istant=rand()%61; 
	
	for (i=0; i<R; i++) {
		for (j=0; j<C; j++) {
			if (mat_map[i][j]==WALL) {
				if (i==12 && (j==13 || j==14)) continue;
				draw_wall(i, j);
			}
			else if (mat_map[i][j]==PILL) {
				draw_pill(i, j);
				remaining_pills++;
			}
		}
	}
}

/* disegna una power pill sullo schermo conoscendo posizione della cella */
void draw_power_pill(int i, int j) {
	int x, y;
	
	map_cell_to_screen(i, j, &x, &y);
	LCD_DrawLine(x-1, y-1, x+1, y-1, Magenta);
	LCD_DrawLine(x-1, y, x+1, y, Magenta);
	LCD_DrawLine(x-1, y+1, x+1, y+1, Magenta);
}

/* spawna le power pill sulla mappa in modo randomico usando come seed l'ultimo valore campionato dall'ADC */
void spawn_power_pills() {
	int i, j;
	int max_power_pill=6;
	
	while (max_power_pill>0) {
		i=rand()%31;
		j=rand()%28;
		if (mat_map[i][j]==PILL) {
			mat_map[i][j]=POWER_PILL;
			draw_power_pill(i, j);
			max_power_pill--;
		}
	}
}

/* disegna PAC MAN sullo schermo conoscendo posizione della cella: se color=Black
	 cancella pacman/pill/powerpill, se color=Yellow disegna pacman */
void draw_pac_man_or_clear(int i, int j, int color) {
	int x, y;
	
	map_cell_to_screen(i, j, &x, &y);
	LCD_DrawLine(x-2, y-2, x+2, y-2, color);
	LCD_DrawLine(x-2, y-1, x+2, y-1, color);
	LCD_DrawLine(x-2, y, x+2, y, color);
	LCD_DrawLine(x-2, y+1, x+2, y+1, color);
	LCD_DrawLine(x-2, y+2, x+2, y+2, color);
}

/* muove PAC MAN sullo schermo da una cella all'altra */
void move_pac_man(int old_i, int old_j, int new_i, int new_j) {
	draw_pac_man_or_clear(old_i, old_j, Black); 	
	draw_pac_man_or_clear(new_i, new_j, Yellow); 	
}

/* usato per cancellare a pezzi il messaggio di PAUSE */
void clear_block(int i, int j) {
	int x, y;
	
	map_cell_to_screen(i, j, &x, &y);
	LCD_DrawLine(x-3, y-3, x+3, y-3, Black);
	LCD_DrawLine(x-3, y-2, x+3, y-2, Black);
	LCD_DrawLine(x-3, y-1, x+3, y-1, Black);
	LCD_DrawLine(x-3, y, x+3, y, Black);
	LCD_DrawLine(x-3, y+1, x+3, y+1, Black);
	LCD_DrawLine(x-3, y+2, x+3, y+2, Black);
	LCD_DrawLine(x-3, y+3, x+3, y+3, Black);
}

/* pulisce il messaggio di pausa al centro dello schermo dopo aver schiacciato una seconda volta INT0 */
void clear_pause_message() {
	int i, j;
	
	for (i=13; i<=15; i++) {
		for (j=11; j<=16; j++) {
			clear_block(i, j);
		}
	}
}
