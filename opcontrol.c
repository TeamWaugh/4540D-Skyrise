#include "main.h"
#include "motors.h"
#include "sensors.h"
#include "encoders.h"
#include "record.h"
#include "lcdmenu.h"
#include "lcdmenu.h"
#include "lib.h"
#include "arm.h"

void operatorControl() {
	lcdRender();
	while (1) {
		int leftStick=joystickGetAnalog(1,3);
		leftStick=abs(leftStick)<minDrive?0:leftStick;
		motorSet(mTopLeft,mTopLeftDir*leftStick);
		motorSet(mBottomLeft,mBottomLeftDir*leftStick);

		int rightStick=joystickGetAnalog(1,2);
		rightStick=abs(rightStick)<minDrive?0:rightStick;
		motorSet(mTopRight,mTopRightDir*rightStick);
		motorSet(mBottomRight,mBottomRightDir*rightStick);

		int armDir=joystickGetDigital(1,5,JOY_UP)-joystickGetDigital(1,5,JOY_DOWN);
		int armDirLeft=armDir;
		int armDirRight=armDir;
		if (joystickGetDigital(1,8,JOY_LEFT)) {
			armDirRight=0;
		}
		if (joystickGetDigital(1,8,JOY_RIGHT)) {
			armDirLeft=0;
		}
		stabilizeArm(armDirLeft,armDirRight,1);

		int claw=(joystickGetDigital(1,6,JOY_UP)*127)+(joystickGetDigital(1,6,JOY_DOWN)*-127);
		motorSet(mClaw,mClawDir*claw);

		int tilt=(joystickGetDigital(1,8,JOY_UP)*127)+(joystickGetDigital(1,8,JOY_DOWN)*-127);
		motorSet(mTilt,mTiltDir*tilt);

		if (joystickGetDigital(1,7,JOY_LEFT)) {
			record("test");
			lcdRender();
		}

		if (joystickGetDigital(1,7,JOY_UP)) {
			play("test");
			lcdRender();
		}

		lcdUpdate();
		delay(20);
	}
}
