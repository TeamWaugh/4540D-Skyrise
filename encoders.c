/*
 * encoders.c
 *
 *  Created on: Mar 18, 2015
 *      Author: Kevin
 */

#include "main.h"
#include "motors.h"
#include "encoders.h"
#include "lib.h"
#include <math.h>

int slopeLen=200;
float adjustment=20;

void enc(float l,float r,int edist) {
	encoderReset(eLeft);
	encoderReset(eRight);
	int cdist=0;
	while (cdist<edist) {
		int lDist=encoderGet(eLeft)/l;
		int rDist=encoderGet(eRight)/r;
		cdist=favg(lDist,rDist);

		float power=fclampf(fminf(cdist/slopeLen,(edist-cdist)/(slopeLen/2))*127,minDrive,127);
		float offset=(lDist-rDist)*(adjustment/power);

		int powerLeft=fclampf(power-offset,-127,127);
		motorSet(mTopLeft,mTopLeftDir*powerLeft);
		motorSet(mBottomLeft,mBottomLeftDir*powerLeft);

		int powerRight=fclampf(power+offset,-127,127);
		motorSet(mTopRight,mTopRightDir*powerRight);
		motorSet(mBottomRight,mBottomRightDir*powerRight);
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
		enc(1,-1,deg*3.5);
	} else {
		enc(-1,1,deg*3.5);
	}
}

void turnRight(int deg) {
	if (encReverse) {
		enc(-1,1,deg*3.5);
	} else {
		enc(1,-1,deg*3.5);
	}
}
