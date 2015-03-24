#include "main.h"
#include "motors.h"
#include "sensors.h"
#include "encoders.h"
#include "lcdmenu.h"
#include "lib.h"
#include <math.h>

int slopeLen=100;
float adjustment=20;

void enc(float l,float r,int edist) {
	int lBase=encoderGet(eLeft);
	int rBase=encoderGet(eRight);
	int cdist=0;
	lcdRender();
	while (cdist<edist) {
		int lDist=(encoderGet(eLeft)-lBase)/l;
		int rDist=(encoderGet(eRight)-rBase)/r;
		cdist=favg(lDist,rDist);

		float power=fclampf(fminf(cdist/slopeLen,(edist-cdist)/(slopeLen/2))*127,minDrive,127);
		float offset=(lDist-rDist)*(adjustment/power);

		int powerLeft=fclampf(power-offset,-127,127);
		motorSet(mTopLeft,mTopLeftDir*powerLeft);
		motorSet(mBottomLeft,mBottomLeftDir*powerLeft);

		int powerRight=fclampf(power+offset,-127,127);
		motorSet(mTopRight,mTopRightDir*powerRight);
		motorSet(mBottomRight,mBottomRightDir*powerRight);

		lcdUpdate();
		delay(20);
	}
}

int encReverse=0;

void forward(int dist) {
	enc(1,1,dist);
}

void backward(int dist) {
	enc(-1,-1,dist);
}

void turnLeft(int deg) {
	if (encReverse) {
		enc(1,-1,deg*encToDeg);
	} else {
		enc(-1,1,deg*encToDeg);
	}
}

void turnRight(int deg) {
	if (encReverse) {
		enc(-1,1,deg*encToDeg);
	} else {
		enc(1,-1,deg*encToDeg);
	}
}
