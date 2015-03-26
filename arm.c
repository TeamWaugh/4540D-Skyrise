#include "main.h"
#include "lib.h"
#include "sensors.h"
#include "motors.h"

void setArm(int val) {
	int arm;
	do {
		arm=avg(analogRead(sArmLeft)*sArmLeftDir,analogRead(sArmRight)*sArmRightDir);
		int dir=arm-val>0?127:-127;
		motorSet(mArmLeft,mArmLeftDir*dir);
		motorSet(mArmRight,mArmRightDir*dir);
	} while (abs(arm-val)>100);
}
