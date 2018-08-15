#include "msxSDL.h"


int map[TILE_HEIGHT][256];

bool initAll() {

	for (int i=0; i<sizeof(SG); i+=2) {
		((uint8_t *)SG)[i+0] = 0XAA;
		((uint8_t *)SG)[i+1] = 0XAA/2;
	}

	memcpy(PN[0],"Unsafe",6);
	
	SA[0].y=20;
	SA[0].x=30;
	SA[0].pattern=0;
	SA[0].color=BLightGreen;

	SA[1].y=21;
	SA[1].x=40;
	SA[1].pattern=0;
	SA[1].color=BLightGreen;

	SA[2].y=22;
	SA[2].x=50;
	SA[2].pattern=0;
	SA[2].color=BLightGreen;

	SA[3].y=23;
	SA[3].x=60;
	SA[3].pattern=0;
	SA[3].color=BLightGreen;
	
	SA[4].y=24;
	SA[4].x=70;
	SA[4].pattern=0;
	SA[4].color=BLightGreen;

	SA[5].y=25;
	SA[5].x=80;
	SA[5].pattern=0;
	SA[5].color=BLightGreen;
	
	for (int i=0; i<8; i++)
		for (int j=0; j<256; j++)
			map[i][j]=rand()%2;
			
	memset(GT[0][1], 0x0A ,8);
	memset(GT[0][2], 0xAA ,8);
	memset(GT[0][3], 0xA0 ,8);
	
	memset(CT[0][1], BBlack+FDarkGreen ,24);
}

uint32_t posx = 0x1000000;
int posy = 0;
int speedx = 0;
int speedy = 0;
int accx = 0;
int accy = 0;

int facing = 1;
int backgroundAcc = 0;
uint32_t backgroundPos = 0x200;

int framen=0;
bool updateLoop() {
	
	if (speedx>0) accx = -0xD0;
	if (speedx<0) accx =  0xD0;
	accy=-0x900;
	
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

	if (keys[' ']) {
		if (posy==0)
			speedy = 0x5000;
		else
			accy = -0x280;
	}
	
	speedx+=accx;
	speedy+=accy;
//	speedx = std::max(speedx,-0x2900);
//	speedx = std::min(speedx, 0x2900);
	speedx = std::max(speedx,-0x8000/3);
	speedx = std::min(speedx, 0x8000/3);
//	speedy = std::max(speedy,-0xF000);
//	speedy = std::min(speedy, 0x5000);

	posx+=speedx;
	posy+=speedy;
	posy = std::max(posy,0);


	framen++;
	if (framen%3) return true;

	uint32_t backgroundPosGoal;
	if (facing==1) {		
		backgroundPosGoal = posx/0x8000-0x08;		
		if (speedx<0) facing=0;
	} else {
		backgroundPosGoal = posx/0x8000-0x14;		
		if (speedx>0) facing=1;
	}
	
	if (backgroundPosGoal>backgroundPos) backgroundPos++;
	if (backgroundPosGoal<backgroundPos) backgroundPos--;
	
	std::cout << backgroundPosGoal << " " << backgroundPos << std::endl;

	for (int i=0; i<8; i++)
		for (int j=0; j<TILE_WIDTH; j++)
			PN[i][j]=2*map[i][(j+backgroundPos)%256];


	SA[0].x = (posx+0x800)/0x1000-backgroundPos*0x8;
	SA[0].y = 180-posy/0x1000;

	return true;
}


int main(int argc, const char* argv[]) {
	return mainLoop();
}
