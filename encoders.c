#include "main.h"
#include "motors.h"
#include "sensors.h"
#include "encoders.h"
#include "lcdmenu.h"
#include "lib.h"
#include <math.h>

int slopeLen=30;
float adjustment=0.4;
int speed=127;

void enc(float l,float r,int edist) {
	if (edist<0) {
		edist=-edist;
		l=-l;
		r=-r;
	}
	int lBase=encoderGet(eLeft)*sEncLeftDir;
	int rBase=encoderGet(eRight)*sEncRightDir;
	int cdist=0;
	lcdRender();
	while (cdist<edist) {
		int lDist=((encoderGet(eLeft)*sEncLeftDir)-lBase)/l;
		int rDist=((encoderGet(eRight)*sEncRightDir)-rBase)/r;
		cdist=favg(lDist,rDist);

		float power=fclampf(fminf(cdist/slopeLen,(edist-cdist)/(slopeLen*4))*speed,minDrive,speed);
		//float power=64;
		float offset=(lDist-rDist)*adjustment;

		int powerLeft=fclampf(power-offset,-127,127);
		motorSet(mTopLeft,mTopLeftDir*powerLeft*l);
		motorSet(mBottomLeft,mBottomLeftDir*powerLeft*l);

		int powerRight=fclampf(power+offset,-127,127);
		motorSet(mTopRight,mTopRightDir*powerRight*r);
		motorSet(mBottomRight,mBottomRightDir*powerRight*r);

		lcdUpdate();
		delay(20);
	}
	motorSet(mTopLeft,0);
	motorSet(mBottomLeft,0);
	motorSet(mTopRight,0);
	motorSet(mBottomRight,0);
}

int encReverse=0;

void forward(int dist) {
	speed=127;
	enc(1,1,dist);
}

void backward(int dist) {
	speed=127;
	enc(-1,-1,dist);
}

void turnLeft(int deg) {
	speed=64;
	if (encReverse) {
		enc(1,-1,deg*encToDeg);
	} else {
		enc(-1,1,deg*encToDeg);
	}
}

void turnRight(int deg) {
	speed=64;
	if (encReverse) {
		enc(-1,1,deg*encToDeg);
	} else {
		enc(1,-1,deg*encToDeg);
	}
}
