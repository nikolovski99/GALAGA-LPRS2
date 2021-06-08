
///////////////////////////////////////////////////////////////////////////////
// Headers.

#include <stdint.h>
#include "system.h"
#include <stdio.h>
#include "start_poruka_rgb333.h"
#include "pobeda_poruka_rgb333.h"
#include "end_poruka_rgb333.h"
#include "level1_poruka_rgb333.h"
#include "level2_poruka_rgb333.h"
#include "level3_poruka_rgb333.h"
#include "sprites_rgb333.h"
#include "muve1_rgb333.h"
#include "boss_rgb333.h"

///////////////////////////////////////////////////////////////////////////////
// HW stuff.

#define WAIT_UNITL_0(x) while(x != 0){}
#define WAIT_UNITL_1(x) while(x != 1){}

#define SCREEN_IDX1_W 640
#define SCREEN_IDX1_H 480
#define SCREEN_IDX4_W 320
#define SCREEN_IDX4_H 240
#define SCREEN_RGB333_W 160
#define SCREEN_RGB333_H 120

#define SCREEN_IDX4_W8 (SCREEN_IDX4_W/8)

#define gpu_p32 ((volatile uint32_t*)LPRS2_GPU_BASE)
#define palette_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0x1000))
#define unpack_idx1_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0x400000))
#define pack_idx1_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0x600000))
#define unpack_idx4_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0x800000))
#define pack_idx4_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0xa00000))
#define unpack_rgb333_p32 ((volatile uint32_t*)(LPRS2_GPU_BASE+0xc00000))
#define joypad_p32 ((volatile uint32_t*)LPRS2_JOYPAD_BASE)

typedef struct {
	unsigned a      : 1;
	unsigned b      : 1;
	unsigned z      : 1;
	unsigned start  : 1;
	unsigned up     : 1;
	unsigned down   : 1;
	unsigned left   : 1;
	unsigned right  : 1;
} bf_joypad;
#define joypad (*((volatile bf_joypad*)LPRS2_JOYPAD_BASE))

typedef struct {
	uint32_t m[SCREEN_IDX1_H][SCREEN_IDX1_W];
} bf_unpack_idx1;
#define unpack_idx1 (*((volatile bf_unpack_idx1*)unpack_idx1_p32))



///////////////////////////////////////////////////////////////////////////////
// Game config.

#define STEP 1
///////////////////////////////////////////////////////////////////////////////
// Game data structures.

typedef struct {
	uint16_t x;
	uint16_t y;
} point_t;

typedef struct {
	point_t pos;
	int is_alive;
	int zivoti;
} spaceship_t;

typedef struct {
	point_t pos;
	int is_alive;
	int step;
	int desno;
	int zivoti;
	int umro; 
} enemy_t;

typedef struct {
	point_t pos;
	int is_shot;
	int raketa;
} laser_t;


typedef struct {
	spaceship_t spaceship;
} game_state_t;


void draw_sprite_from_atlas(
	uint16_t src_x,
	uint16_t src_y,
	uint16_t w,
	uint16_t h,
	uint16_t dst_x,
	uint16_t dst_y
) {

	for(uint16_t y = 0; y < h; y++){
		for(uint16_t x = 0; x < w; x++){
			uint32_t src_idx =
				(src_y+y)*Pacman_Sprite_Map__w +
				(src_x+x);
			uint32_t dst_idx =
				(((dst_y+y)%SCREEN_RGB333_H)*SCREEN_RGB333_W) +
				(dst_x+x)%SCREEN_RGB333_W;
			uint16_t pixel = Pacman_Sprite_Map__p[src_idx];
			unpack_rgb333_p32[dst_idx] = (pixel)?pixel:unpack_rgb333_p32[dst_idx];
		}
	}


}
void draw_poruke(
	uint16_t src_x,
	uint16_t src_y,
	uint16_t w,
	uint16_t h,
	uint16_t dst_x,
	uint16_t dst_y
) {

	for(uint16_t y = 0; y < h; y++){
		for(uint16_t x = 0; x < w; x++){
			uint32_t src_idx =
				(src_y+y)*start_poruka__w +
				(src_x+x);
			uint32_t dst_idx =
				(((dst_y+y)%SCREEN_RGB333_H)*SCREEN_RGB333_W) +
				(dst_x+x)%SCREEN_RGB333_W;
			uint16_t pixel = start_poruka__p[src_idx];
			unpack_rgb333_p32[dst_idx] = (pixel)?pixel:unpack_rgb333_p32[dst_idx];
		}
	}


}
void draw_poruke1(
	uint16_t src_x,
	uint16_t src_y,
	uint16_t w,
	uint16_t h,
	uint16_t dst_x,
	uint16_t dst_y
) {

	for(uint16_t y = 0; y < h; y++){
		for(uint16_t x = 0; x < w; x++){
			uint32_t src_idx =
				(src_y+y)*pobeda_poruka__w +
				(src_x+x);
			uint32_t dst_idx =
				(((dst_y+y)%SCREEN_RGB333_H)*SCREEN_RGB333_W) +
				(dst_x+x)%SCREEN_RGB333_W;
			uint16_t pixel = pobeda_poruka__p[src_idx];
			unpack_rgb333_p32[dst_idx] = (pixel)?pixel:unpack_rgb333_p32[dst_idx];
		}
	}


}
void draw_poruke_end(
	uint16_t src_x,
	uint16_t src_y,
	uint16_t w,
	uint16_t h,
	uint16_t dst_x,
	uint16_t dst_y
) {

	for(uint16_t y = 0; y < h; y++){
		for(uint16_t x = 0; x < w; x++){
			uint32_t src_idx =
				(src_y+y)*end_poruka__w +
				(src_x+x);
			uint32_t dst_idx =
				(((dst_y+y)%SCREEN_RGB333_H)*SCREEN_RGB333_W) +
				(dst_x+x)%SCREEN_RGB333_W;
			uint16_t pixel = end_poruka__p[src_idx];
			unpack_rgb333_p32[dst_idx] = (pixel)?pixel:unpack_rgb333_p32[dst_idx];
		}
	}
}
void draw_level1(
	uint16_t src_x,
	uint16_t src_y,
	uint16_t w,
	uint16_t h,
	uint16_t dst_x,
	uint16_t dst_y
) {

	for(uint16_t y = 0; y < h; y++){
		for(uint16_t x = 0; x < w; x++){
			uint32_t src_idx =
				(src_y+y)*level1_poruka__w +
				(src_x+x);
			uint32_t dst_idx =
				(((dst_y+y)%SCREEN_RGB333_H)*SCREEN_RGB333_W) +
				(dst_x+x)%SCREEN_RGB333_W;
			uint16_t pixel = level1_poruka__p[src_idx];
			unpack_rgb333_p32[dst_idx] = (pixel)?pixel:unpack_rgb333_p32[dst_idx];
		}
	}
}
void draw_level2(
	uint16_t src_x,
	uint16_t src_y,
	uint16_t w,
	uint16_t h,
	uint16_t dst_x,
	uint16_t dst_y
) {

	for(uint16_t y = 0; y < h; y++){
		for(uint16_t x = 0; x < w; x++){
			uint32_t src_idx =
				(src_y+y)*level2_poruka__w +
				(src_x+x);
			uint32_t dst_idx =
				(((dst_y+y)%SCREEN_RGB333_H)*SCREEN_RGB333_W) +
				(dst_x+x)%SCREEN_RGB333_W;
			uint16_t pixel = level2_poruka__p[src_idx];
			unpack_rgb333_p32[dst_idx] = (pixel)?pixel:unpack_rgb333_p32[dst_idx];
		}
	}
}
void draw_level3(
	uint16_t src_x,
	uint16_t src_y,
	uint16_t w,
	uint16_t h,
	uint16_t dst_x,
	uint16_t dst_y
) {

	for(uint16_t y = 0; y < h; y++){
		for(uint16_t x = 0; x < w; x++){
			uint32_t src_idx =
				(src_y+y)*level3_poruka__w +
				(src_x+x);
			uint32_t dst_idx =
				(((dst_y+y)%SCREEN_RGB333_H)*SCREEN_RGB333_W) +
				(dst_x+x)%SCREEN_RGB333_W;
			uint16_t pixel = level3_poruka__p[src_idx];
			unpack_rgb333_p32[dst_idx] = (pixel)?pixel:unpack_rgb333_p32[dst_idx];
		}
	}
}

void draw_boss(
	uint16_t src_x,
	uint16_t src_y,
	uint16_t w,
	uint16_t h,
	uint16_t dst_x,
	uint16_t dst_y
) {

	for(uint16_t y = 0; y < h; y++){
		for(uint16_t x = 0; x < w; x++){
			uint32_t src_idx =
				(src_y+y)*boss__w +
				(src_x+x);
			uint32_t dst_idx =
				(((dst_y+y)%SCREEN_RGB333_H)*SCREEN_RGB333_W) +
				(dst_x+x)%SCREEN_RGB333_W;
			uint16_t pixel = boss__p[src_idx];
			unpack_rgb333_p32[dst_idx] = (pixel)?pixel:unpack_rgb333_p32[dst_idx];
		}
	}
}

void draw_muve1(
	uint16_t src_x,
	uint16_t src_y,
	uint16_t w,
	uint16_t h,
	uint16_t dst_x,
	uint16_t dst_y
) {

	for(uint16_t y = 0; y < h; y++){
		for(uint16_t x = 0; x < w; x++){
			uint32_t src_idx =
				(src_y+y)*muve1__w +
				(src_x+x);
			uint32_t dst_idx =
				(((dst_y+y)%SCREEN_RGB333_H)*SCREEN_RGB333_W) +
				(dst_x+x)%SCREEN_RGB333_W;
			uint16_t pixel = muve1__p[src_idx];
			unpack_rgb333_p32[dst_idx] = (pixel)?pixel:unpack_rgb333_p32[dst_idx];
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Game code.

int main(void) {

	// Setup.
	gpu_p32[0] = 3; // RGB333 mode.
	gpu_p32[0x800] = 0x00ff00ff; // Magenta for HUD.


	int sirina = SCREEN_RGB333_W;
	int visina = SCREEN_RGB333_H;
	int brojac = 0;
	int stanje = 0;
	int pljuc = 0;
	int rafal = 0;
	int tmp = 0;
	int current = 2;
	int nivo = 0;
	
	//DEFINISANJE KARAKTERA U IGRICI
	
	game_state_t gs;
	gs.spaceship.pos.x = 32;
	gs.spaceship.pos.y = 32;
	gs.spaceship.is_alive = 1;
	gs.spaceship.zivoti = 3;
	
	enemy_t muve[5];

	for(int i=0; i<5; i++) {
		muve[i].pos.x = (i+1) * 16;
		muve[i].pos.y = 0;
		muve[i].is_alive = 1;
		muve[i].desno = 1;
		muve[i].umro = 0;

	}
	
	enemy_t boss;
	boss.pos.x = 16;
	boss.pos.y = 16;
	boss.desno = 1;
	boss.zivoti = 3;
	boss.is_alive = 0;
	
	
	laser_t laser[3];
	for(int i=0; i<3; i++) {
		laser[i].is_shot = 0;
		laser[i].pos.x = gs.spaceship.pos.x;
		laser[i].pos.y= 104;
		laser[i].raketa = 0;
	}
	
	laser_t laser_muva[5];
	for (int i=0; i<5; i++) {
		
		laser_muva[i].raketa = 0;
		laser_muva[i].pos.x = muve[i].pos.x;
		laser_muva[i].pos.y = muve[i].pos.y;	
		laser_muva[i].is_shot = 0;
		
	}
	
	laser_t laserBoss;
	laserBoss.raketa = 0;
	laserBoss.pos.x = boss.pos.x;
	laserBoss.pos.y = boss.pos.y;
	laserBoss.is_shot = 0;
	

	typedef enum{RIGHT, LEFT, UP, DOWN} direction;
	direction dir = RIGHT;
	//POMOCNA ZA KRETANJE MUVA//
	
	//BELE TACKE U SVEMIRU //
	uint16_t bt[320],pbt[320];
		for(uint16_t r = 8,i=0; r < SCREEN_RGB333_H; r=r+8){
			for(uint16_t c = 8; c < SCREEN_RGB333_W;c=c+8){	
				pbt[i]=r*SCREEN_RGB333_W + c;
				bt[i]=1;
				i++;
			}
		}
	
	
	while(1){
		/////////////////////////////////////
		// KONTROLE

		int mov_x = 0;
		int mov_y = 0;
		if(joypad.right){
			mov_x = +1;
			dir = RIGHT;
		}else if(joypad.left){
			mov_x = -1;
			dir = LEFT;

		}else if(joypad.up){
			mov_y = -1;
			dir = UP;

		}else if(joypad.down){
			mov_y = +1;
			dir = DOWN;

		}
		
		if(joypad.b) {
			pljuc = 1;
		}
		if(joypad.z){
			stanje = 1;
		}
		if(joypad.a) {
			rafal = 1;
		}

		/////////////////////////////////////
		// Gameplay.
		
		//KRETANJE I OGRANICENJE BRODA, MUVA I LASERA
		
		if((gs.spaceship.pos.x += mov_x*STEP) > SCREEN_RGB333_W){
			if(mov_x < 0){
				gs.spaceship.pos.x = 0;
			} else {
				if((gs.spaceship.pos.x += mov_x*16) > SCREEN_RGB333_W){
						gs.spaceship.pos.x = 140;
				}
			}
		}
		
		for(int i=0; i<5; i++) {
			if (muve[i].pos.x == 140) {
			
				muve[i].desno = 0;
			} else if (muve[i].pos.x == 20) {
				muve[i].desno = 1;
			}
			
		
		}
		
		
		for(int i=0; i<5; i++) {
			if (muve[i].desno) {
			
				muve[i].pos.x += STEP;
			} else {
				muve[i].pos.x -= STEP;
			}
		
		}
		
		// KRETANJE BOSSA
		if (boss.pos.x == 130) {	
			boss.desno = 0;
		} else if (boss.pos.x == 20) {
			boss.desno = 1;
		}
		
		if (boss.desno) {
			boss.pos.x += 1.5 * STEP;
		} else {
			boss.pos.x -= 1.5 * STEP;
		}
		
		
		
		for(int i=0; i<5; i++) {
			if (laser_muva[i].pos.y >110) {
				laser_muva[i].pos.y = muve[i].pos.y;
			}
		}
		
		if(laserBoss.pos.y > 110) {
			laserBoss.pos.y = boss.pos.y;
		}
		
		
		//PROVERA DA LI SU LASERI POGODILI METU
		
		if(laser[1].is_shot) {
				for(int i = 0; i < 5; i++) {
					if( muve[i].is_alive) {
						if(laser[1].pos.y == muve[i].pos.y)  {
							if((muve[i].pos.x - 8 <= laser[1].pos.x) && (muve[i].pos.x + 8 >= laser[1].pos.x)) {
								muve[i].is_alive = 0;
								laser_muva[i].is_shot = 0;
								laser[1].is_shot = 0;
							}
						}
					} 
				}
				
				if(boss.is_alive) {
					if(laser[1].pos.y == boss.pos.y)  {
							if((boss.pos.x - 14 <= laser[1].pos.x) && (boss.pos.x + 14 >= laser[1].pos.x)) {
								printf("%d", boss.zivoti);
								boss.zivoti--;
							}
					}
				}
			}
			
		for (int i=0; i<5; i++) {
			if(laser_muva[i].is_shot) {			
				if(gs.spaceship.is_alive) {		
					//printf("Poz lasera x %d y %d, poz broda x %d y %d\n", laser_muva[i].pos.x,laser_muva[i].pos.y, gs.spaceship.pos.x, gs.spaceship.pos.y);	
						if(laser_muva[i].pos.y == 104)  {
							if((gs.spaceship.pos.x - 8 <= laser_muva[i].pos.x) && (gs.spaceship.pos.x + 8 >= laser_muva[i].pos.x)) {
								gs.spaceship.zivoti--;
								laser_muva[i].is_shot = 1;
							}
						}
				} 
			
			}
		}
		
		if(laserBoss.is_shot) {			
				if(gs.spaceship.is_alive) {			
						if(laserBoss.pos.y == 104)  {
							if((gs.spaceship.pos.x - 8 <= laserBoss.pos.x) && (gs.spaceship.pos.x + 8 >= laserBoss.pos.x)) {
								gs.spaceship.zivoti--;
								laserBoss.is_shot = 1;
							}
						}
				} 
			
			}
		
		
		
		
		if (muve[0].is_alive == 0 && muve[1].is_alive == 0 && muve[2].is_alive == 0 && muve[3].is_alive == 0 && muve[4].is_alive == 0) {
			tmp = 4;
			brojac++;
		}
		if(tmp == 4 && brojac == 1){
			nivo = 1;
			for(int i=0; i<5; i++) {
				
				muve[i].is_alive = 1;
				if (i == 0 || i == 2 || i == 4) {
					laser_muva[i].is_shot = 1;
				}	
				muve[i].umro = 0;
				
			}
			tmp = 0;			
		}
		
		if(tmp == 4 && brojac == 2){
			boss.is_alive = 1;
			laserBoss.is_shot = 1;
			nivo = 2;
			for(int i=0; i<5; i++) {
				
				muve[0].is_alive = 1;
				laser_muva[i].is_shot = 0;		
				
			}
			tmp = 0;			
		}
		
		if (boss.zivoti == 0) {
			boss.is_alive = 0;
			stanje = 2;
		}
		
		if(gs.spaceship.zivoti == 0) {
			gs.spaceship.is_alive = 0;
			stanje = 4;
		}
		
		//NACIN PUCANJA a ili b
		
		if (pljuc) {	
				pljuc = 0;	
				for(int i = 0; i<3; i++) {
					laser[i].is_shot = 1;
					laser[i].raketa = 0;
				
					laser[i].pos.x = gs.spaceship.pos.x;
					laser[i].pos.y= 104;	
				}			
				
		} 
		
		if(rafal) {
			rafal = 0;
			laser[0].is_shot = 0;
			laser[1].is_shot = 1;
			laser[1].raketa = 1;
			laser[2].is_shot = 0;
				
			laser[1].pos.x = gs.spaceship.pos.x;
			laser[1].pos.y= 104;	
		}
		
		
		/////////////////////////////////////
		// Drawing.


		// Detecting rising edge of VSync.
		WAIT_UNITL_0(gpu_p32[2]);
		WAIT_UNITL_1(gpu_p32[2]);
		// Draw in buffer while it is in VSync.

	
		// Black background.
		for(uint16_t r = 0; r < SCREEN_RGB333_H; r++){
			for(uint16_t c = 0; c < SCREEN_RGB333_W; c++){
				unpack_rgb333_p32[r*SCREEN_RGB333_W + c] = 0000;
			}
		}

		//ISCRTAVANJE BELIH TACKI
		//SA NETACNIM JEDENJEM, jede samo kad ga ubode u oko
		for(uint16_t i=0,r = 8; r < SCREEN_RGB333_H; r=r+8){
			for(uint16_t c = 8; c < SCREEN_RGB333_W;c=c+8){	
				
				unpack_rgb333_p32[r*SCREEN_RGB333_W + c] = 0xffffffff;
				
			}
		}
		
		// ICRTAVANJE MUVA U ZAVISNOSTI OD TRENUTNOG NIVOA
		if(stanje ==1){
			
			if (nivo == 0) {
				for (int i=0; i<5; i++) {
					if (muve[i].is_alive == 1) {
						draw_muve1(0,0,16,16, muve[i].pos.x, muve[i].pos.y);
						draw_level1(0,0,130,81,0,12);					
					} else if(muve[i].is_alive == 0 && muve[i].umro < 3) {
						draw_sprite_from_atlas(13*16,0,16,16, muve[i].pos.x, muve[i].pos.y);
						draw_level1(0,0,130,81,0,12);	
						muve[i].umro++;
					}
				
				}
			} else if(nivo == 1) {
				for (int i=0; i<5; i++) {
					if (muve[i].is_alive == 1) {
						draw_sprite_from_atlas(11*16+2,13*16,16,16, muve[i].pos.x, muve[i].pos.y); 
						draw_level2(0,0,130,81,0,12);
					} else if(muve[i].is_alive == 0 && muve[i].umro < 3) {
						draw_sprite_from_atlas(13*16,0,16,16, muve[i].pos.x, muve[i].pos.y);
						draw_level1(0,0,130,81,0,12);	
						muve[i].umro++;
					}
				
				}
			} else {
				if(boss.is_alive) {
					if (boss.zivoti == 3) {
						for(int i=1; i<=3; i++) {
							draw_sprite_from_atlas(
							16,12*16,16,16, sirina - 16*i, 0);
						}		 
					} else if(boss.zivoti == 2) {
						draw_sprite_from_atlas(
						16,12*16,16,16, sirina -16, 0);
					
						draw_sprite_from_atlas(
						16,12*16,16,16, sirina -32, 0);
					} else {
						draw_sprite_from_atlas(
						16,12*16,16,16, sirina -16, 0);
					}
				draw_boss(
					0,0,32,32, boss.pos.x, boss.pos.y);
					draw_level3(0,0,130,81,0,12);
				}
			}
			
		
			for (int i = 0; i<3; i++) {
				if (laser[i].is_shot) {
					
					if (laser[i].pos.y <=0) {
						draw_sprite_from_atlas(12*16+4,13*16,6,6, gs.spaceship.pos.x, 104);
						laser[i].is_shot = 0;
					} else {
						if(laser[i].raketa) {
							laser[i].pos.y = (laser[i].pos.y)-1.5;
							draw_sprite_from_atlas(12*16+5,11*16,16,16, laser[i].pos.x + i*5, (laser[i].pos.y));
						} else {
							laser[i].pos.y = (laser[i].pos.y)-1.5;
							draw_sprite_from_atlas(12*16+4,13*16,6,6, laser[i].pos.x + i*5, (laser[i].pos.y));
						}
						
					}
					
				}
			}
		
			for (int i=0; i<5; i++) {
				if(laser_muva[i].is_shot) {
					laser_muva[i].pos.y = (laser_muva[i].pos.y)+1;
				if(laser_muva[i].pos.y < 10) {
					laser_muva[i].pos.x = muve[i].pos.x;
				}
				draw_sprite_from_atlas(12*16+4,13*16,6,6, laser_muva[i].pos.x, (laser_muva[i].pos.y));
				}
			}
			
			if(laserBoss.is_shot) {
				laserBoss.pos.y = (laserBoss.pos.y)+1;
				laserBoss.pos.x = boss.pos.x;
				draw_sprite_from_atlas(5*16,14*16,16,16, laserBoss.pos.x, (laserBoss.pos.y));
				}
			
			if (gs.spaceship.is_alive) {
				if (gs.spaceship.zivoti == 3) {
						for(int i=1; i<=3; i++) {
							draw_sprite_from_atlas(
							1*16,11*16,16,16, sirina - 16*i, 104);
						}		 
					} else if(gs.spaceship.zivoti == 2) {
						draw_sprite_from_atlas(
						1*16,11*16,16,16, sirina -16, 104);
					
						draw_sprite_from_atlas(
						1*16,11*16,16,16, sirina -32, 104);
					} else {
						draw_sprite_from_atlas(
						1*16,11*16,16,16, sirina -16, 104);
					}
				draw_sprite_from_atlas(
					12*16+4,13*16,16,16, gs.spaceship.pos.x, 104
				);
			}	
			
			
		} 
		
	//GAME OVER	
	if(stanje == 4){
		draw_poruke_end(0,0,140,41,10,20);
		draw_sprite_from_atlas(12*16+4,13*16,16,16, gs.spaceship.pos.x, 104);
	}
	
	//VICTORY
	if(stanje == 2){
		draw_poruke1(0,0,140,76,10,20);
		draw_sprite_from_atlas(12*16+4,13*16,16,16, gs.spaceship.pos.x, 104);
	}
	
	//POCETNA
	if(stanje == 0){
		draw_poruke(0,0,140,41,10,20);
		draw_sprite_from_atlas(12*16+4,13*16,16,16, gs.spaceship.pos.x, 104);
	}

} while(1);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
