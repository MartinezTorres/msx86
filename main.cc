#include "msxSDL.h"
#include <unistd.h>

#define cropped(a,b,c) (a<(b)?(b):(a>(c)?(c):a))

bool (*state_ptr)();
bool I0_init();
bool M0_menu();
bool L0_levelInit();
bool L1_levelMain();
bool L1_levelDeath();
bool L1_levelEnd();


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

	{ 	0b00111100,
		0b01000010,
		0b01010110,
		0b01000010,
		0b10000001,
		0b10011101,
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


uint8_t myBG[8][8] = {

	{ 	0b00000000,
		0b00100000,
		0b00000000,
		0b00000000,
		0b00000100,
		0b00001000,
		0b00000000,
		0b00000000 },

	{ 	0b01111110,
		0b10000001,
		0b10000001,
		0b10000001,
		0b10000001,
		0b10000001,
		0b10000001,
		0b01111110 },

	{ 	0b01111110,
		0b10000001,
		0b10000001,
		0b10000001,
		0b10000001,
		0b10000001,
		0b10000001,
		0b01111110 },

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

	{ 	0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000 },

};



/*
bool updateLoopOld() {
	
	if (speedx>=0) accx = std::max(-speedx,-0xD0);
	if (speedx<=0) accx = std::min(-speedx, 0xD0);
	
	if (keys[SDLK_RIGHT%256]) {
		if (speedx>=0)
			accx=0xE4;
		else
			accx=0x1A0;
	}

	if (keys[SDLK_LEFT%256]) {
		if (speedx<=0)
			accx=-0xE4;
		else
			accx=-0x1A0;
	}

	if (keys[SDLK_DOWN%256]) {
	}

	if (keys[SDLK_UP%256]) {
	}

	if (keys[SDLK_UP%256]) {
	}

	if (keys[' ']) {
		if (isJumping<2 and jumpReleased==1) {
//			speedy = 0x5000;
			speedy = 0x4000;
			isJumping++;
			jumpReleased = 0;
		} else {
			accy = -0x280;
		}
	} else {
		accy=-0x900;
		jumpReleased = 1;
	}
	
	speedx+=accx;
	speedy+=accy;
	speedx = std::max(speedx,-0x2900);
	speedx = std::min(speedx, 0x2900);
	speedy = std::max(speedy,-0xF000);
	speedy = std::min(speedy, 0x5000);

	//posy = std::max(posy,0x40000);
	//if (posy==0x40000) isJumping=0;
	bool collision = map[(posx+0x4000)/0x8000%512][24-(posy+0x4000)/0x8000];
	if (collision) speedy=std::max(0,speedy);
	if (collision and speedy<=0) isJumping=0;

	posx+=speedx;
	posy+=speedy;
	

	if (facing== 1 and speedx<0 or accx<-0x1000) facing=-1;
	if (facing==-1 and speedx>0 or accx> 0x1000) facing= 1;
	
	static int winX = posx - TILE_WIDTH*4*0x1000;
	static int winY = 0;
	
	int posScreenXold = (posx-winX+0x800)/0x1000;
	int posScreenYold = winY-8*8-8-posy/0x1000;
	
	int winXspeed = (posScreenXold-TILE_WIDTH*4)*0x2900/(TILE_WIDTH*2);
	winXspeed = round(winXspeed/double(0x1000))*0x1000;
	
	winX += winXspeed;

	framen++;
//	if (framen%3) return true;
	
	SA[0].x = (posx-winX+0x800)/0x1000;
	SA[0].y = 24*8-8-(posy-winY+0x800)/0x1000;

	if (facing== 1) SA[0].pattern = 0x02+((framen/4)%2);
	if (facing==-1) SA[0].pattern = 0x82+((framen/4)%2);
	if (abs(speedx)<0x100) SA[0].pattern = 1;
	
	SA[0].color = BDarkYellow;
	if (isJumping==2) SA[0].color = BCyan;

	//std::cout << map[(posx+0x4000)/0x8000%512][24-(posy+0x4000)/0x8000] << std::endl;

//	SA[0].color = collision?BDarkYellow:BCyan;

	
	for (int i=0; i<TILE_HEIGHT; i++) {
		for (int j=0; j<TILE_WIDTH; j++) {
			int x4=winX/0x4000;
			if (x4%2) {
				PN[i][j]=2*map[(j+x4/2)%MAP_WIDTH][i]+1*map[(j+(x4+1)/2)%MAP_WIDTH][i];
			} else {
				PN[i][j]=3*map[(j+x4/2)%MAP_WIDTH][i];
			}
		}
	}

	return true;
}*/




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

bool I0_init() {

	for (int i=0; i<sizeof(SG); i+=2) {
		((uint8_t *)SG)[i+0] = 0XAA;
		((uint8_t *)SG)[i+1] = 0XAA/2;
	}

	memcpy(SG,mySG,sizeof(mySG));
	
	for (int j=0; j<127; j++)
		for (int i=0; i<8; i++)
			SG[j+128][i] = reverse8(SG[j][i]);

/*
	memset(GT[0][1], 0x0A ,8);
	memset(GT[0][2], 0xA0 ,8);
	memset(GT[0][3], 0xAA ,8);

	memset(GT[1][1], 0x0A ,8);
	memset(GT[1][2], 0xA0 ,8);
	memset(GT[1][3], 0xAA ,8);
			
	memset(GT[2][1], 0x0A ,8);
	memset(GT[2][2], 0xA0 ,8);
	memset(GT[2][3], 0xAA ,8);*/
	
	for (int i=0; i<3; i++)
		for (int j=0; j<8; j++)
			for (int k=0; k<8; k++)
				for (int l=0; l<8; l++)
					GT[i][(j<<3)+k][l]=(myBG[j][l]<<4)+(myBG[k][l]>>4);

	for (int i=0; i<3; i++)
		for (int j=0; j<8; j++)
			for (int l=0; l<8; l++)
				GT[i][0xF0+j][l]=myBG[j][l];


	
	memset(CT, BBlack+FDarkBlue ,sizeof(CT));

std::cerr << sizeof(CT[0][0]) << std::endl;
	for (int i=0; i<3; i++)
		memset(CT[i][0xF0], BBlack+FDarkBlue, sizeof(CT[i][0xF0]));
	
	
	state_ptr = &M0_menu;
}

bool M0_menu() {	auto &player = levelState.entities[0];


	state_ptr = L0_levelInit;
	return true;
}

bool L0_levelInit() {
	
	levelState.map.pos.x=0x0000;
	levelState.map.pos.y=0x0000;
	
	for (uint8_t i8=0; i8<32; i8++)
		levelState.entities[i8].enabled = false;
	levelState.frameN = 0;
	
	auto &player = levelState.entities[0];

	player.enabled = true;
	player.type = T_PLAYER;
	player.pos = {0x200,0x200};
	player.speed = {0x0,0x40};
	player.acc = {0x0,0x0};
	player.facing = 0x1;
	player.state = ST_JUMP0;
	player.step = 0;
	player.hitbox = {0x10,0x10,0x69,0x69};
	
	const char mapInfo[] = 
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
"a                                                                                                                              a"
"a                                                                                                                              a"
"a                                                                                                                              a"
"a                                                                                                                              a"
"a                    aaaaaaa                                                                                                   a"
"a                                                                                                                              a"
"a                                                                                                                              a"
"a                                                                                                                              a"
"a                     aaaaaa                                                                                                   a"
"a   aaaaa a                                                                                                                    a"
"a                                                                                                                              a"
"a                                                                                                                              a"
"a              aaaaa a                                                                                                         a"
"a                                                                                                                              a"
"a     aaaaa                                                                                                                    a"
"a                                                                                                                              a"
"a            aaaaaa                                                                                                            a"
"a                                                                                                                              a"
"a                                                                                                                              a"
"a    aaaaa                                                                                                                     a"
"a                                                                                                                              a"
"a                                                                                                                              a"
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

	//std::cerr << sizeof(levelState.map.tiles) << " " << sizeof(mapInfo) << std::endl;

//	for (int i=0; i<24; i++)
//		memcpy(levelState.map.tiles[23-i], &mapInfo[i*128], sizeof(levelState.map.tiles[23-i]));

	for (int i=0; i<levelState.map.size.y; i++) {
		for (int j=0; j<levelState.map.size.x; j++) {
			switch(mapInfo[i*128+j]) {
			case 'a':
				levelState.map.tiles[23-i][j] = 1+((i+j)&1);
				break;
			default:
				levelState.map.tiles[23-i][j] = 0;
			}
		}
	}

	state_ptr = L1_levelMain;
	return true;
}

bool L1_levelMain() {

	auto &map = levelState.map;	
	auto &player = levelState.entities[0];
	if (player.enabled) {

		if (player.speed.x>=0) player.acc.x = std::max(-player.speed.x,-0x1);
		if (player.speed.x<=0) player.acc.x = std::min(-player.speed.x, 0x1);
		
		if (keys[SDLK_RIGHT%256]) {
			if (player.facing>=0)
				player.acc.x=0x2;
			else
				player.acc.x=0x1;
		}

		if (keys[SDLK_LEFT%256]) {
			if (player.facing<=0)
				player.acc.x=-0x2;
			else
				player.acc.x=-0x1;
		}

		if (keys[SDLK_DOWN%256]) {}

		if (keys[SDLK_UP%256]) {}

		if (keys[' ']) {
			if (player.state<ST_JUMP2 and levelState.jumpReleased) {
	//			speedy = 0x5000;
				player.speed.y = 0x40;
				player.state++;
				levelState.jumpReleased = false;
			} else {
				player.acc.y = -0x2;
			}
		} else {
			player.acc.y=-0x09;
			levelState.jumpReleased = true;
		}
		
		player.speed.x += player.acc.x;
		player.speed.y += player.acc.y;
		player.speed.x = cropped(player.speed.x,-0x20,0x20);
		player.speed.y = cropped(player.speed.y,-0x7F,0x50);
		
	
		uint8_t x0 =  (player.pos.x + player.hitbox.x)>>7;
		uint8_t x1 =  (player.pos.x + player.hitbox.x + player.hitbox.dx)>>7;
		uint8_t x0m = (player.pos.x + player.hitbox.x-1)>>7;
		uint8_t x1p = (player.pos.x + player.hitbox.x + player.hitbox.dx+1)>>7;
		uint8_t y0 =  ((player.pos.y + player.hitbox.y)>>7);
		uint8_t y1 =  ((player.pos.y + player.hitbox.y + player.hitbox.dy)>>7);
		uint8_t y0m = ((player.pos.y + player.hitbox.y-1)>>7);
		uint8_t y1p = ((player.pos.y + player.hitbox.y + player.hitbox.dy+1)>>7);

		
		std::cerr << int(x0) << " " << int(y0) << " " << player.pos.y << " " << player.speed.y << " " << player.acc.y << std::endl;
//		usleep(100000);

		if ( player.speed.y<=0 and ( (map.tiles[y0m][x0] | map.tiles[y0m][x1]) > 0 ) ) { // Collision below
			
			player.state = ST_RESTING;
			
			player.pos.y = (uint16_t(y0m+1)<<7) - player.hitbox.y;			
			player.speed.y = 0;

			std::cerr << "col: " << player.pos.y << std::endl;


			y0 =  ((player.pos.y + player.hitbox.y)>>7);
			y1 =  ((player.pos.y + player.hitbox.y + player.hitbox.dy)>>7);
			y0m = ((player.pos.y + player.hitbox.y-1)>>7);
			y1p = ((player.pos.y + player.hitbox.y + player.hitbox.dy+1)>>7);
		}

		if ( player.speed.x<=0 and ( (map.tiles[y0][x0m] | map.tiles[y1][x0m]) > 0 ) ) { // Collision left
			
		//	player.state = ST_RESTING;
			player.acc.x = -player.acc.x; 
			player.speed.x = -player.speed.x; 
//			player.pos.y = (uint16_t(24-y0)<<7) - player.hitbox.y;			
			player.speed.y = 0;

//			std::cerr << "col: " << player.pos.y << std::endl;
		}
		std::cerr << int(x0) << " " << int(y0) << " " << player.pos.y << " " << player.speed.y << " " << player.acc.y << " " << "X" << std::endl;

		player.pos.x += player.speed.x;
		player.pos.y += player.speed.y;

		std::cerr << int(x0) << " " << int(y0) << " " << player.pos.y << " " << player.speed.y << " " << player.acc.y << " " << "Y" << std::endl;
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

	
	
	map.pos.x = player.pos.x-((0xF8+player.facing*0x40)<<3);
	
	map.pos.x = cropped(map.pos.x,0,(map.size.x-32)*0x80-1);
	
	std::cerr << player.pos.x << " " << map.pos.x << " " << ((map.pos.x-player.pos.x)>>7) << std::endl;
	
	SA[0].x =         (player.pos.x+0x8-map.pos.x)>>4;
	SA[0].y = 24*8-8-1-((player.pos.y+0x8-map.pos.y)>>4);

	SA[0].pattern = 1;
	SA[0].color = BDarkYellow;
	static int framen=0;
	framen++;
	if (player.facing== 1) SA[0].pattern = 0x02+((framen/4)%2);
	if (player.facing==-1) SA[0].pattern = 0x82+((framen/4)%2);
	
	
	for (int i=0; i<TILE_HEIGHT; i++) {
		for (int j=0; j<TILE_WIDTH; j++) {
			int x4=(map.pos.x+0x20)>>6;
			if (x4&1) {
				PN[i][j]=(map.tiles[23-i][j+(x4>>1)]<<3)+map.tiles[23-i][j+((x4+1)>>1)];
			} else {
				PN[i][j]=0xF0+map.tiles[23-i][j+(x4>>1)];
			}
		}
	}
	
	
/*

	//posy = std::max(posy,0x40000);
	//if (posy==0x40000) isJumping=0;
	bool collision = map[(posx+0x4000)/0x8000%512][24-(posy+0x4000)/0x8000];
	if (collision) speedy=std::max(0,speedy);
	if (collision and speedy<=0) isJumping=0;

	posx+=speedx;
	posy+=speedy;
	

	if (facing== 1 and speedx<0 or accx<-0x1000) facing=-1;
	if (facing==-1 and speedx>0 or accx> 0x1000) facing= 1;
	
	static int winX = posx - TILE_WIDTH*4*0x1000;
	static int winY = 0;
	
	int posScreenXold = (posx-winX+0x800)/0x1000;
	int posScreenYold = winY-8*8-8-posy/0x1000;
	
	int winXspeed = (posScreenXold-TILE_WIDTH*4)*0x2900/(TILE_WIDTH*2);
	winXspeed = round(winXspeed/double(0x1000))*0x1000;
	
	winX += winXspeed;

	framen++;
//	if (framen%3) return true;
	
	SA[0].x = (posx-winX+0x800)/0x1000;
	SA[0].y = 24*8-8-(posy-winY+0x800)/0x1000;

	if (facing== 1) SA[0].pattern = 0x02+((framen/4)%2);
	if (facing==-1) SA[0].pattern = 0x82+((framen/4)%2);
	if (abs(speedx)<0x100) SA[0].pattern = 1;
	
	SA[0].color = BDarkYellow;
	if (isJumping==2) SA[0].color = BCyan;

	//std::cout << map[(posx+0x4000)/0x8000%512][24-(posy+0x4000)/0x8000] << std::endl;

//	SA[0].color = collision?BDarkYellow:BCyan;

	
	for (int i=0; i<TILE_HEIGHT; i++) {
		for (int j=0; j<TILE_WIDTH; j++) {
			int x4=winX/0x4000;
			if (x4%2) {
				PN[i][j]=2*map[(j+x4/2)%MAP_WIDTH][i]+1*map[(j+(x4+1)/2)%MAP_WIDTH][i];
			} else {
				PN[i][j]=3*map[(j+x4/2)%MAP_WIDTH][i];
			}
		}
	}*/

//	state_ptr = L1_levelMain;
	return true;
}

bool L1_levelDeath() {

	state_ptr = M0_menu;
	return true;
}

bool L1_levelEnd() {

	state_ptr = M0_menu;
	return true;
}



bool updateLoop() { return (*state_ptr)(); }
int main(int argc, const char* argv[]) {
	
	state_ptr = &I0_init;
	return mainLoop();
}
