#include "LPC17xx.h"
#include "RIT/RIT.h"
#include "timer/timer.h"
#include "sample.h"
#include "GLCD/GLCD.h"
#include "map/screen_drawer.h"
#include <stdlib.h>


extern int blinky_cell_i;
extern int blinky_cell_j;
volatile int blinky_last_direction=0; 
extern int mat_map[31][28];
extern int PAC_cell_i;
extern int PAC_cell_j;
volatile int blinky_mode=CHASE;
volatile int blinky_frightened_until=-1;
volatile int blinky_respawn_instant=-1;
extern int game_time;
volatile int blinky_just_frightened=0;
extern int life;


/* utilizzo un pò sporco di draw_pac_man_or_clear() dato che la sto usando per disegnare blinky */
void redraw_blinky(int old_i, int old_j, int new_i, int new_j) {
	
	draw_pac_man_or_clear(old_i, old_j, Black); 	
	if (blinky_mode==FRIGHTENED) {
		draw_pac_man_or_clear(new_i, new_j, Blue2);	
	} else {
		draw_pac_man_or_clear(new_i, new_j, Red); 
	}
	
	if (mat_map[old_i][old_j]==PILL) {
		draw_pill(old_i, old_j);
	}
	else if (mat_map[old_i][old_j]==POWER_PILL) {
		draw_power_pill(old_i, old_j);
	}
}


/* gestione del movimento più compatta rispetto alla cascata di if per PAC MAN */
void move_blinky() {
	int i, next_i, next_j, best_direction;
	int min_distance=100000;
	static int directions[4][2]={{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // sù i--, giù i++, sx j--, dx j++
	static int opposites[4]={1, 0, 3, 2};
	
	
	if (game_time<=blinky_frightened_until && blinky_frightened_until>0) {
		blinky_mode=CHASE;
		blinky_frightened_until=-1;
	}
	
	
	if (game_time<=blinky_respawn_instant && blinky_respawn_instant>0) {
		blinky_mode=CHASE;
		draw_pac_man_or_clear(blinky_cell_i, blinky_cell_j, Red);
		blinky_respawn_instant=-1;
	}
	
	switch(blinky_mode) {
		case CHASE:
			
			if (blinky_cell_j==13 && (blinky_cell_i==13 || blinky_cell_i==12)) {
				int next_i=blinky_cell_i+directions[0][0];
				int next_j=blinky_cell_j+directions[0][1];
				blinky_last_direction=1;
				redraw_blinky(blinky_cell_i, blinky_cell_j, next_i, next_j);
				blinky_cell_i=next_i;
				blinky_cell_j=next_j;
				return;
			}
	
			// esplora le 4 (3 in realtà) direzioni possibili (evitando il passo indietro come nel gioco originale) -> sù, giù, sx, dx
			for (i=0; i<4; i++) {
				
				if (i==blinky_last_direction) continue;

				int current_next_i=blinky_cell_i+directions[i][0];
				int current_next_j=blinky_cell_j+directions[i][1];

				if (mat_map[current_next_i][current_next_j]!=WALL) {
					// distanza di Manhattan da PAC MAN
					int distance= abs(current_next_j - PAC_cell_j) + abs(current_next_i - PAC_cell_i);

					if (distance<min_distance) {
							min_distance=distance;
							next_i=current_next_i;
							next_j=current_next_j;
							best_direction=i; 		
					}
				}
			}
			blinky_last_direction=opposites[best_direction]; // per opporre e ottenere la direzione in cui non andare al passo successivo
			break;
			
		case FRIGHTENED:
			
			if (blinky_just_frightened==1) {
				next_i=blinky_cell_i+directions[blinky_last_direction][0];
				next_j=blinky_cell_j+directions[blinky_last_direction][1];
				blinky_just_frightened=0;
				blinky_last_direction=opposites[blinky_last_direction];
				break;
			}
			
			// blinky sceglie la direzione in modo casuale quando è in frightened
			do {
				i=rand()%4;
				next_i=blinky_cell_i+directions[i][0];
				next_j=blinky_cell_j+directions[i][1];
			} while(i==blinky_last_direction || mat_map[next_i][next_j]==WALL);
			
			blinky_last_direction=opposites[i];
			break;
		
		case EATEN:
			return;
		
		default:
			break;
	}
	
	redraw_blinky(blinky_cell_i, blinky_cell_j, next_i, next_j);
	
	
	if (blinky_mode==CHASE && blinky_cell_i==PAC_cell_i && blinky_cell_j==PAC_cell_j) {
		life--;
		if (life>0) {
			PAC_cell_i=23;
			PAC_cell_j=13;
			draw_pac_man_or_clear(PAC_cell_i, PAC_cell_j, Yellow);
		}
	}
	
	blinky_cell_i=next_i;
	blinky_cell_j=next_j;
	
	
	if (mat_map[blinky_cell_i][blinky_cell_j]==TELEPORT) { 
		if (blinky_cell_j==0) blinky_cell_j=27;
		else blinky_cell_j=0;
		redraw_blinky(next_i, next_j, blinky_cell_i, blinky_cell_j);
	}
}
