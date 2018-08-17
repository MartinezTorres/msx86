#include "msxSDL.h"

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

bool initAll() {

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
			map[16+i][j]=rand()%2;
			
	memset(GT[2][1], 0x0A ,8);
	memset(GT[2][2], 0xA0 ,8);
	memset(GT[2][3], 0xAA ,8);
	
	memset(CT[2][1], BBlack+FLightGreen ,24);
}

uint32_t posx = 0x1000000;
int posy = 0;
int speedx = 0;
int speedy = 0;
int accx = 0;
int accy = 0;

int facing = 1;
int backgroundAcc = 0;
uint32_t backgroundPos = 0x400;
int backgroundOffset = 0;
int backgroundMoving = 0;

int framen=0;
int isJumping=0;
int jumpReleased=1;

bool updateLoopOld() {
	
	if (speedx>=0) accx = std::max(-speedx,-0xD0);
	if (speedx<=0) accx = std::min(-speedx, 0xD0);

//	std::cout << speedx << " " << accx << " " << speedy << " " << accy <<  std::endl;
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
	speedx = std::max(speedx,-0x8000/2);
	speedx = std::min(speedx, 0x8000/2);
	speedy = std::max(speedy,-0xF000);
	speedy = std::min(speedy, 0x5000);


	posx+=speedx;
	posy+=speedy;
	posy = std::max(posy,0);

	framen++;
//	if (framen%3) return true;

	uint32_t backgroundPosGoal;
	if (facing==1) {		
		backgroundPosGoal = posx/0x4000-0x08*2;		
		if (speedx<0) facing=0;
	} else {
		backgroundPosGoal = posx/0x4000-0x14*2;		
		if (speedx>0) facing=1;
	}
	
	if (backgroundMoving == 0 and abs(backgroundPosGoal-backgroundPos)>2 and abs(speedx)>0x2000) {
		backgroundMoving = 1;
		
		backgroundOffset = (posx+0x800)/0x1000-backgroundPos*0x04;
		
		std::cout << "Off:" << backgroundOffset << std::endl;
	}
	if (abs(backgroundPosGoal-backgroundPos)>12 or abs(speedx)<0x1000) backgroundMoving = 2;
	if (abs(backgroundPosGoal-backgroundPos)<1) backgroundMoving = 0;

	std::cout << backgroundPosGoal << " " << backgroundPos << " " << backgroundMoving <<  std::endl;

	if (backgroundMoving==2) {
		if (backgroundPosGoal>backgroundPos) backgroundPos++;
		if (backgroundPosGoal<backgroundPos) backgroundPos--;
	}
	

	if (backgroundMoving==1) {

//		std::cout << "p1:" << backgroundPos << std::endl;
		std::cout << backgroundOffset << " " << ((posx+0x800)/0x1000-backgroundPos*0x04)  << std::endl;
		
		if (facing==1 and backgroundOffset <= (posx+0x800)/0x1000-(backgroundPos+1)*0x04) backgroundPos++;
		if (facing==0 and backgroundOffset >= (posx+0x800)/0x1000-(backgroundPos-1)*0x04) backgroundPos--;

//		std::cout << "p2:" << backgroundPos << " " << (posx+0x800)/0x1000-backgroundPos*0x04 << std::endl;
	
//		if (facing==1 and backgroundOffset<(posx+0x800)/0x1000-backgroundPos*0x04) backgroundOffset = (posx+0x800)/0x1000-backgroundPos*0x04;
//		if (facing==0 and backgroundOffset>(posx+0x800)/0x1000-backgroundPos*0x04) backgroundOffset = (posx+0x800)/0x1000-backgroundPos*0x04;
	
		std::cout << backgroundOffset << " " << ((posx+0x800)/0x1000-backgroundPos*0x04)  << std::endl;
		if (abs(backgroundOffset - ((posx+0x800)/0x1000-backgroundPos*0x04))>1) return true;

	}

	for (int i=0; i<TILE_HEIGHT; i++) {
		for (int j=0; j<TILE_WIDTH; j++) {
			if (backgroundPos%2) {
				PN[i][j]=2*map[i][(j+backgroundPos/2)%256]+1*map[i][(j+(backgroundPos+1)/2)%256];
			} else {
				PN[i][j]=3*map[i][(j+backgroundPos/2)%256];
			}
		}
	}

	SA[0].x = (posx+0x800)/0x1000-backgroundPos*0x04;
	SA[0].y = 8*16-8-posy/0x1000;

	return true;
}

bool updateLoop() {
	
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
	posx+=speedx;
	posy+=speedy;

	posy = std::max(posy,0);
	if (posy==0) isJumping=0;

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
	SA[0].y = 16*8-8-(posy-winY+0x800)/0x1000;

	if (facing== 1) SA[0].pattern = 0x02+((framen/4)%2);
	if (facing==-1) SA[0].pattern = 0x82+((framen/4)%2);
	if (abs(speedx)<0x100) SA[0].pattern = 1;
	
	SA[0].color = BDarkYellow;
	if (isJumping==2) SA[0].color = BCyan;

	
	for (int i=0; i<TILE_HEIGHT; i++) {
		for (int j=0; j<TILE_WIDTH; j++) {
			int x4=winX/0x4000;
			if (x4%2) {
				PN[i][j]=2*map[(j+x4/2)%256][i]+1*map[(j+(x4+1)/2)%256][i];
			} else {
				PN[i][j]=3*map[(j+x4/2)%256][i];
			}
		}
	}
	

	/*


	uint32_t backgroundPosGoal;
	if (facing==1) {		
		backgroundPosGoal = posx/0x4000-0x08*2;		
		if (speedx<0) facing=0;
	} else {
		backgroundPosGoal = posx/0x4000-0x14*2;		
		if (speedx>0) facing=1;
	}
	
	if (backgroundMoving == 0 and abs(backgroundPosGoal-backgroundPos)>2 and abs(speedx)>0x2000) {
		backgroundMoving = 1;
		
		backgroundOffset = (posx+0x800)/0x1000-backgroundPos*0x04;
		
		std::cout << "Off:" << backgroundOffset << std::endl;
	}
	if (abs(backgroundPosGoal-backgroundPos)>12 or abs(speedx)<0x1000) backgroundMoving = 2;
	if (abs(backgroundPosGoal-backgroundPos)<1) backgroundMoving = 0;

	std::cout << backgroundPosGoal << " " << backgroundPos << " " << backgroundMoving <<  std::endl;

	if (backgroundMoving==2) {
		if (backgroundPosGoal>backgroundPos) backgroundPos++;
		if (backgroundPosGoal<backgroundPos) backgroundPos--;
	}
	

	if (backgroundMoving==1) {

//		std::cout << "p1:" << backgroundPos << std::endl;
		std::cout << backgroundOffset << " " << ((posx+0x800)/0x1000-backgroundPos*0x04)  << std::endl;
		
		if (facing==1 and backgroundOffset <= (posx+0x800)/0x1000-(backgroundPos+1)*0x04) backgroundPos++;
		if (facing==0 and backgroundOffset >= (posx+0x800)/0x1000-(backgroundPos-1)*0x04) backgroundPos--;

//		std::cout << "p2:" << backgroundPos << " " << (posx+0x800)/0x1000-backgroundPos*0x04 << std::endl;
	
//		if (facing==1 and backgroundOffset<(posx+0x800)/0x1000-backgroundPos*0x04) backgroundOffset = (posx+0x800)/0x1000-backgroundPos*0x04;
//		if (facing==0 and backgroundOffset>(posx+0x800)/0x1000-backgroundPos*0x04) backgroundOffset = (posx+0x800)/0x1000-backgroundPos*0x04;
	
		std::cout << backgroundOffset << " " << ((posx+0x800)/0x1000-backgroundPos*0x04)  << std::endl;
		if (abs(backgroundOffset - ((posx+0x800)/0x1000-backgroundPos*0x04))>1) return true;

	}

	for (int i=0; i<TILE_HEIGHT; i++) {
		for (int j=0; j<TILE_WIDTH; j++) {
			if (backgroundPos%2) {
				PN[i][j]=2*map[i][(j+backgroundPos/2)%256]+1*map[i][(j+(backgroundPos+1)/2)%256];
			} else {
				PN[i][j]=3*map[i][(j+backgroundPos/2)%256];
			}
		}
	}

*/
	return true;
}


int main(int argc, const char* argv[]) {
	return mainLoop();
}
