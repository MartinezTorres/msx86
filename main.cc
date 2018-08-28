#include "msxSDL.h"
#include <unistd.h>

#define cropped(a,b,c) (a<(b)?(b):(a>(c)?(c):a))

typedef void *(*T_f)(void);

T_f I0_init();
T_f M0_menu();
T_f L0_levelInit();
T_f L1_levelMain();
T_f L1_levelDeath();
T_f L1_levelEnd();


static const uint16_t MAP_HEIGHT = 32;
static const uint16_t MAP_WIDTH = 512;

int map[MAP_WIDTH][MAP_HEIGHT];

uint8_t mySG[][8] = {

	{ 	0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000 },

	{ 	0b00111110,
		0b01000001,
		0b01011001,
		0b01000001,
		0b10000001,
		0b10111101,
		0b01000010,
		0b00111100 },


	{ 	0b00000000,
		0b00011100,
		0b00110010,
		0b01100001,
		0b11000001,
		0b11000010,
		0b01100100,
		0b00111000 },

	{ 	0b00000000,
		0b00011110,
		0b00110001,
		0b01100001,
		0b11000001,
		0b11000010,
		0b11000100,
		0b01111000 },


	{ 	0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000 },

	{ 	0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000 },
};


// BG is organized as | 1 | p0 | x0 | x1 | l0 | l1 | r0 | r1

uint8_t myBG0[4][8][2] = {

	{ 	{ 0b00000000, BBlack + FDarkBlue },
		{ 0b00000000, BBlack + FDarkBlue },
		{ 0b00000000, BBlack + FDarkBlue },
		{ 0b00000000, BBlack + FDarkBlue },
		{ 0b00000000, BBlack + FDarkBlue },
		{ 0b00000000, BBlack + FDarkBlue },
		{ 0b00000000, BBlack + FDarkBlue },
		{ 0b00000000, BBlack + FDarkBlue } },

	{ 	{ 0b00000000, BBlack + FDarkBlue },
		{ 0b00100000, BBlack + FDarkBlue },
		{ 0b00000000, BBlack + FDarkBlue },
		{ 0b00000000, BBlack + FDarkBlue },
		{ 0b00000000, BBlack + FDarkBlue },
		{ 0b00000100, BBlack + FDarkBlue },
		{ 0b00010000, BBlack + FDarkBlue },
		{ 0b00000000, BBlack + FDarkBlue } },

	{ 	{ 0b01111110, BBlack + FDarkBlue },
		{ 0b10100101, BBlack + FDarkBlue },
		{ 0b11011011, BBlack + FDarkBlue },
		{ 0b10100101, BBlack + FDarkBlue },
		{ 0b11011011, BBlack + FDarkBlue },
		{ 0b10100101, BBlack + FDarkBlue },
		{ 0b11011011, BBlack + FDarkBlue },
		{ 0b01111110, BBlack + FDarkBlue } },

	{ 	{ 0b01111110, BBlack + FDarkBlue },
		{ 0b10100101, BBlack + FDarkBlue },
		{ 0b11011011, BBlack + FDarkBlue },
		{ 0b10100101, BBlack + FDarkBlue },
		{ 0b11011011, BBlack + FDarkBlue },
		{ 0b10100101, BBlack + FDarkBlue },
		{ 0b11011011, BBlack + FDarkBlue },
		{ 0b01111110, BBlack + FDarkBlue } },
};


uint8_t myBG1[4][8][2] = {

	{ 	{ 0b00000000, BBlack + FDarkRed },
		{ 0b00000000, BBlack + FDarkRed },
		{ 0b00000000, BBlack + FDarkRed },
		{ 0b00000000, BBlack + FDarkRed },
		{ 0b00000000, BBlack + FDarkRed },
		{ 0b00000000, BBlack + FDarkRed },
		{ 0b00000000, BBlack + FDarkRed },
		{ 0b00000000, BBlack + FDarkRed } },

	{ 	{ 0b00000000, BBlack + FDarkRed },
		{ 0b00100000, BBlack + FDarkRed },
		{ 0b00000000, BBlack + FDarkRed },
		{ 0b00000000, BBlack + FDarkRed },
		{ 0b00000000, BBlack + FDarkRed },
		{ 0b00000100, BBlack + FDarkRed },
		{ 0b00010000, BBlack + FDarkRed },
		{ 0b00000000, BBlack + FDarkRed } },

	{ 	{ 0b01111110, BBlack + FDarkRed },
		{ 0b10100101, BBlack + FDarkRed },
		{ 0b11011011, BBlack + FDarkRed },
		{ 0b10100101, BBlack + FDarkRed },
		{ 0b11011011, BBlack + FDarkRed },
		{ 0b10100101, BBlack + FDarkRed },
		{ 0b11011011, BBlack + FDarkRed },
		{ 0b01111110, BBlack + FDarkRed } },

	{ 	{ 0b01111110, BBlack + FDarkRed },
		{ 0b10100101, BBlack + FDarkRed },
		{ 0b11011011, BBlack + FDarkRed },
		{ 0b10100101, BBlack + FDarkRed },
		{ 0b11011011, BBlack + FDarkRed },
		{ 0b10100101, BBlack + FDarkRed },
		{ 0b11011011, BBlack + FDarkRed },
		{ 0b01111110, BBlack + FDarkRed } },
};

struct TRect16 {
	int16_t x,y,dx,dy;
};

struct uint8_tp {
	uint8_t x,y;
};

struct TPair16 {
	int16_t x,y;
};

struct TMap {
	
	TPair16 pos;
	uint8_tp size = {128,24};
	uint8_tp initPos = {2, 2};
	uint8_t tiles[24][128];
};

struct TEntity {
	
	bool enabled;
	uint16_t type;
	TPair16 pos, speed, acc;
	int8_t facing;
	int8_t state;
	int8_t step;
	TRect16 hitbox;
};

struct TLevelState {
	
	uint32_t frameN;
	bool jumpReleased = true;
	
	
	TMap map;
	TEntity entities[32];
};

TLevelState levelState;

enum { T_PLAYER };
enum { ST_RESTING, ST_JUMP0, ST_JUMP1, ST_JUMP2 };

T_f I0_init() {

	for (int i=0; i<sizeof(SG); i+=2) {
		((uint8_t *)SG)[i+0] = 0XAA;
		((uint8_t *)SG)[i+1] = 0XAA/2;
	}

	memcpy(SG,mySG,sizeof(mySG));
	
	for (int j=0; j<127; j++)
		for (int i=0; i<8; i++)
			SG[j+128][i] = reverse8(SG[j][i]);

	for (int nt=0; nt<3; nt++) {
		for (int h=0; h<4; h++) {
			for (int cl=0; cl<4; cl++) {
				for (int cr=0; cr<4; cr++) {
					for (int l=0; l<8; l++) {
						GT[nt][0x80+(h<<4)+(cl<<2)+(cr<<0)][l] = 
							(myBG0[cl][l][0]<<(h*2)) + (myBG0[cr][l][0]>>(8-h*2));
						CT[nt][0x80+(h<<4)+(cl<<2)+(cr<<0)][l] = 
							std::max(myBG0[cl][l][1], myBG0[cr][l][1]);
					}
				}
			}
		}
	}
	
	return T_f(M0_menu);
}

T_f M0_menu() {	

	return T_f(L0_levelInit);
}

T_f L0_levelInit() {
	
	levelState.map.pos.x=0x0000;
	levelState.map.pos.y=0x0000;
	
	for (uint8_t i8=0; i8<32; i8++)
		levelState.entities[i8].enabled = false;
	levelState.frameN = 0;
	
	auto &player = levelState.entities[0];

	player.enabled = true;
	player.type = T_PLAYER;
	player.pos = {0x400,0x400};
	player.speed = {0x0,0x80};
	player.acc = {0x0,0x0};
	player.facing = 0x1;
	player.state = ST_JUMP0;
	player.step = 0;
	player.hitbox = {0x20,0x20,0xDF,0xDF};
	
	const char mapInfo[] = 
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
"a                                                                                                                              a"
"a                                                                                                                              a"
"a                                                                                                                              a"
"                                                                                                                               a"
"                     aaaaaaa                                                                                                   a"
"                                                                                                                               a"
"a                                                                                                                              a"
"a                                                                                                                              a"
"a                     aaaaaa                                                                                                   a"
"a   aaaaa a                                                                                                                    a"
"a                                                                                                                              a"
"a                                                                                                                              a"
"a              aaaaa a                                                                                                         a"
"a                                                                                                                              a"
"a     aaaaa           a a aa  aaa        aaaa                                                                                  a"
"a                                a   a       a                                                                                 a"
"a            aaaaaa                aaa       a                                                                                 a"
"a                               a                                                                                              a"
"a                        a  a             a a                                                                                  a"
"a    aaaaa                a    a          a                                                                                    a"
"a                                        aa      aaaa                                                                          a"
"a                                 aaaa                                                                                         a"
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

	for (int i=0; i<levelState.map.size.y; i++) {
		for (int j=0; j<levelState.map.size.x; j++) {
			switch(mapInfo[i*128+j]) {
			case 'a':
				levelState.map.tiles[23-i][j] = 2;
				break;
			default:
				levelState.map.tiles[23-i][j] = 0;
			}
		}
	}

	return T_f(L1_levelMain);
}

T_f L1_levelMain() {

	auto &map = levelState.map;	
	auto &player = levelState.entities[0];
	if (player.enabled) {

		if (player.speed.x>=0) player.acc.x = std::max(-player.speed.x,-0x2);
		if (player.speed.x<=0) player.acc.x = std::min(-player.speed.x, 0x2);
		
		if (keys[SDLK_RIGHT%256]) {
//			if (player.facing>=0)
				player.acc.x=0x4;
//			else
//				player.acc.x=0x2;
		}

		if (keys[SDLK_LEFT%256]) {
//			if (player.facing<=0)
				player.acc.x=-0x4;
//			else
//				player.acc.x=-0x2;
		}

		if (keys[SDLK_DOWN%256]) {}

		if (keys[SDLK_UP%256]) {}

		if (keys[' ']) {
			if (player.state<ST_JUMP0 and levelState.jumpReleased) {
				player.speed.y = 0x74;
				player.state++;
				levelState.jumpReleased = false;
			} else {
				player.acc.y = -0x5;
			}
		} else {
			player.acc.y=-0x12;
			levelState.jumpReleased = true;
		}
		
		player.speed.x += player.acc.x;
		player.speed.y += player.acc.y;
		player.speed.x = cropped(player.speed.x,-0x40,0x40);
		player.speed.y = cropped(player.speed.y,-0xFF,0xA0);
		player.pos.x += player.speed.x;
		player.pos.y += player.speed.y;
	

		if (player.pos.y<0) 
			return T_f(L1_levelDeath);

		if (player.pos.x<0) 
			return T_f(L1_levelEnd);
	
		uint8_t x0 =  (player.pos.x + player.hitbox.x)>>8;
		uint8_t x1 =  (player.pos.x + player.hitbox.x + player.hitbox.dx)>>8;
		uint8_t y0 =  (player.pos.y + player.hitbox.y)>>8;
		uint8_t y1 =  (player.pos.y + player.hitbox.y + player.hitbox.dy)>>8;

		uint8_t x0r =  (player.pos.x + player.hitbox.x)&0xFF;
		uint8_t x1r =  0xFF - ((player.pos.x + player.hitbox.x + player.hitbox.dx)&0xFF);
		uint8_t y0r =  (player.pos.y + player.hitbox.y)&0xFF;
		uint8_t y1r =  0xFF - ((player.pos.y + player.hitbox.y + player.hitbox.dy)&0xFF);
		
		uint8_t colVec = 0;
		if (map.tiles[y0][x0]>0) colVec +=0x1;
		if (map.tiles[y0][x1]>0) colVec +=0x2;
		if (map.tiles[y1][x0]>0) colVec +=0x4;
		if (map.tiles[y1][x1]>0) colVec +=0x8;
		
		enum { LEFT=0x1, RIGHT=0x2, TOP=0x4, BOTTOM=0x8};
		
		uint8_t colResponse = 0;
		
		switch (colVec) {
		case 0x01: if (x0r>y0r) colResponse = LEFT;  else colResponse = BOTTOM;	break;
		case 0x02: if (x1r>y0r) colResponse = RIGHT; else colResponse = BOTTOM;	break;
		case 0x04: if (x0r>y1r) colResponse = LEFT;  else colResponse = TOP;    break;
		case 0x08: if (x1r>y1r) colResponse = RIGHT; else colResponse = TOP;    break;
			
		case 0xF-0x1: colResponse = RIGHT+TOP;    break;
		case 0xF-0x2: colResponse = LEFT +TOP;    break;
		case 0xF-2-4: case 0xF-0x4: colResponse = RIGHT+BOTTOM; break;
		case 0xF-1-8: case 0xF-0x8: colResponse = LEFT +BOTTOM; break;

		case 0x1+0x2: colResponse = BOTTOM; break;
		case 0x4+0x8: colResponse = TOP;    break;
		case 0x1+0x4: colResponse = LEFT;   break;
		case 0x2+0x8: colResponse = RIGHT;  break;
		
		default:
			break;
		}
		
		if ( colResponse & BOTTOM ) { // Collision below
			
			player.state = ST_RESTING;
			player.pos.y = (uint16_t(y0+1)<<8) - player.hitbox.y;			
			player.speed.y = 0;
		}
		
		if ( colResponse & TOP ) { // Collision above
			
			player.pos.y = (uint16_t(y1)<<8) - player.hitbox.y - player.hitbox.dy;			
			player.speed.y = 0;
		}

		if ( colResponse & LEFT ) { // Collision left
			
			player.speed.x += 0x08; 
			player.pos.x = (uint16_t(x0+1)<<8) - player.hitbox.x;			
		} 
		if ( colResponse & RIGHT ) { // Collision left
			
			player.speed.x -= 0x08; 
			player.pos.x = (uint16_t(x1)<<8) - player.hitbox.x - player.hitbox.dx;			
		}
		
	}
	
	for (uint8_t i8=1; i8<32; i8++) {
		if (levelState.entities[i8].enabled) {
			switch (levelState.entities[i8].type) {
				case T_PLAYER:
					break;
				default:
					break;				
			}
		}
	}
	
	player.facing = 0;
	if (player.speed.x<0) player.facing=-1;
	if (player.speed.x>0) player.facing= 1;

	
	int idealMapSpeed = (player.pos.x-((0xF8-player.facing*0x40)<<4) - map.pos.x)>>4;
	map.pos.x += idealMapSpeed; 
	
	map.pos.x = cropped(map.pos.x,0,((map.size.x-32)<<8)-1);
	
	int displayMapPosX = ((map.pos.x+0x20)>>6)<<6;
	
	int spritePosX = (player.pos.x+0x10-displayMapPosX)>>5;

	static int nSkipped = 0;
	nSkipped++;
	if (nSkipped<0) {
		if (spritePosX < SA[0].x and player.speed.x>0) return T_f(L1_levelMain);
		if (spritePosX > SA[0].x and player.speed.x<0) return T_f(L1_levelMain);
	}
	nSkipped=0;
	
	SA[0].x =           spritePosX;
	SA[0].y = 24*8-8-1-((player.pos.y+0x10-map.pos.y)>>5);

	SA[0].pattern = 1;
	SA[0].color = BDarkYellow;
	static int framen=0;
	framen++;
	if (player.facing== 1) SA[0].pattern = 0x02+((framen/3)%2);
	if (player.facing==-1) SA[0].pattern = 0x82+((framen/3)%2);
	
	
/*	for (int i=0; i<TILE_HEIGHT; i++) {
		for (int j=0; j<TILE_WIDTH; j++) {
			int x4=(displayMapPosX+0x40)>>7;
			if (x4&1) {
				PN[i][j]=(map.tiles[23-i][j+(x4>>1)]<<3)+map.tiles[23-i][j+((x4+1)>>1)];
			} else {
				PN[i][j]=0xF0+map.tiles[23-i][j+(x4>>1)];
			}
		}
	}*/
	
	int displayMapPosY = 0;

	{
		int x2=(displayMapPosX+0x20)>>6;
		uint8_t pv = 0x80 + ((x2&3)<<4);
		std::cout << x2 << " " << int(((x2&3)<<4)) <<  std::endl;
		for (int i=0; i<TILE_HEIGHT; i++) {
			uint8_t *p00 = &map.tiles[23-i][(x2>>2)];
			uint8_t *p01 = p00+1;
			for (int j=0; j<TILE_WIDTH; j++) {
				PN[i][j]= pv + (*p00<<2) + *p01;
//				std::cout << int(*p00) << " " << int(*p01) << std::endl;
				p00++; p01++; 
			}
		}	
	}
	
	return T_f(L1_levelMain);
}

T_f L1_levelDeath() {

	std::cout << "Death" << std::endl;
	return T_f(M0_menu);
}

T_f L1_levelEnd() {

	std::cout << "Goal reached!" << std::endl;
	return T_f(M0_menu);
}


T_f state_ptr;
bool updateLoop() { state_ptr = T_f((*state_ptr)()); return true;}
int main(int argc, const char* argv[]) {
	
	state_ptr = T_f(I0_init);
	return mainLoop();
}
