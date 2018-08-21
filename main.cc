#include "msxSDL.h"

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

	{ 	0b00000000,
		0b00111100,
		0b01000010,
		0b10000001,
		0b10000001,
		0b10000001,
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

struct TPair16 {
	int16_t x,y;
};

struct TMap {
	int8_t tileMap[16];
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
	
	TPair16 cameraPos;	
	TEntity entities[32];
	uint32_t frameN;
};

TLevelState levelState;
bool jumpReleased = true;

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
	
	memcpy(PN[0],"Unsafe",6);
	
	SA[0].y=20;
	SA[0].x=30;
	SA[0].pattern=0x00;
	SA[0].color=BLightGreen;

	SA[1].y=21;
	SA[1].x=40;
	SA[1].pattern=0x01;
	SA[1].color=BLightGreen;

	SA[2].y=22;
	SA[2].x=50;
	SA[2].pattern=0x02;
	SA[2].color=BLightGreen;

	SA[3].y=23;
	SA[3].x=60;
	SA[3].pattern=0x03;
	SA[3].color=BLightGreen;
	
	SA[4].y=24;
	SA[4].x=70;
	SA[4].pattern=0x82;
	SA[4].color=BLightGreen;

	SA[5].y=25;
	SA[5].x=80;
	SA[5].pattern=0x83;
	SA[5].color=BLightGreen;
	
	for (int i=0; i<MAP_WIDTH; i++)
		for (int j=0; j<MAP_HEIGHT; j++)
			map[i][j]=!(rand()%4);

	for (int i=0; i<MAP_WIDTH; i++)
		for (int j=0; j<16; j++)
			map[i][j]=0;

	memset(GT[0][1], 0x0A ,8);
	memset(GT[0][2], 0xA0 ,8);
	memset(GT[0][3], 0xAA ,8);

	memset(GT[1][1], 0x0A ,8);
	memset(GT[1][2], 0xA0 ,8);
	memset(GT[1][3], 0xAA ,8);
			
	memset(GT[2][1], 0x0A ,8);
	memset(GT[2][2], 0xA0 ,8);
	memset(GT[2][3], 0xAA ,8);
	
	memset(CT, BBlack+FLightGreen ,sizeof(CT));
	
	state_ptr = &M0_menu;
}

bool M0_menu() {	auto &player = levelState.entities[0];


	state_ptr = L0_levelInit;
	return true;
}

bool L0_levelInit() {
	
	levelState.cameraPos.x=0x0000;
	levelState.cameraPos.y=0x0000;
	
	for (uint8_t i8=0; i8<32, i8++)
		levelState.entities.enabled = false;
	levelState.frameN = 0;
	
	auto &player = levelState.entities[0];

	player.enabled = true;
	player.type = T_PLAYER;
	player.pos = {0x1000,0x600};
	player.speed = {0x0,0x40};
	player.acc = {0x0,0x0};
	player.facing = 0x1;
	player.state = ST_JUMP0;
	player.step = 0;
	player.hitbox = {0x00,0x00,0x79,0x79};
	
	
//int isJumping=0;
//int jumpReleased=1;

	state_ptr = L1_levelMain;
	return true;
}

bool L1_levelMain() {
	
	auto &player = levelState.entities[0];
	if (player.enabled) {

		if (player.speed.x>=0) player.acc.x = std::max(-player.speed.x,-0x1);
		if (player.speed.x<=0) player.acc.y = std::min(-player.speed.y, 0x1);
		
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

		if (keys[SDLK_DOWN%256]) {
		}

		if (keys[SDLK_UP%256]) {
		}

		if (keys[' ']) {
			if (player.state<ST_JUMP2 and jumpReleased) {
	//			speedy = 0x5000;
				player.speed.y = 0x40;
				player.state++;
				jumpReleased = false;
			} else {
				accy = -0x2;
			}
		} else {
			accy=-0x9;
			jumpReleased = true;
		}
		
		speedx += accx;
		speedy += accy;
		speedx = std::max(speedx,-0x2900);
		speedx = std::min(speedx, 0x2900);
		speedy = std::max(speedy,-0xF000);
		speedy = std::min(speedy, 0x5000);
	
		
	}
	
		
	for (uint8_t i8=0; i8<32, i8++) {
		if (levelState.entities.enabled) {
			switch (levelState.entities.type) {
				case T_PLAYER:
					break;
				default:
					break;				
			}
			
			
		}
	}


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
