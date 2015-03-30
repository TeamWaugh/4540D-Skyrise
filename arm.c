#include "main.h"
#include "lib.h"
#include "sensors.h"
#include "motors.h"
#include "arm.h"

void setArm(int val) {
	int arm;
	do {
		arm=avg(analogRead(sArmLeft)*sArmLeftDir,analogRead(sArmRight)*sArmRightDir);
		int dir=arm-val>0?127:-127;
		motorSet(mArmLeft,mArmLeftDir*dir);
		motorSet(mArmRight,mArmRightDir*dir);
	} while (abs(arm-val)>50);
	motorSet(mArmLeft,0);
	motorSet(mArmRight,0);
}

int armHold=0;

int invalid(int v) {
	return v<900||v>3100;
}

void stabilizeArm(int ld,int rd,int hold) {
	// detect failure of pots
	int al=analogRead(sArmLeft)*sArmLeftDir;
	int ar=analogRead(sArmRight)*sArmRightDir;
	int avg=0;
	int of=0; // adjustment offset
	if (invalid(al)&&invalid(ar)) {
		armHold=0;
		// dual failure
	} else if (invalid(ar)) {
		// right pot failed
		avg=al;
	} else if (invalid(al)) {
		// left pot failed
		avg=ar;
	} else {
		// pots working
		of=(al-ar)/4;
	}
	int hp=0; // hold power
	if (hold&&(!invalid(armHold))&&avg&&(!ld)&&(!rd)) {
		if (ld||rd) {
			armHold=avg;
		} else {
			hp=((avg-armHold)/2)*127;
		}
	}
	motorSet(mArmLeft,clamp((mArmLeftDir*127*ld)+hp-of,-127,127));
	motorSet(mArmRight,clamp((mArmRightDir*127*rd)+hp+of,-127,127));
}

