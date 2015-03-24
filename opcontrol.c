#include "main.h"
#include "motors.h"
#include "sensors.h"
#include "encoders.h"
#include "lcdmenu.h"
#include "lcdmenu.h"
#include "lib.h"

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

		int arm=(joystickGetDigital(1,5,JOY_UP)*-127)+(joystickGetDigital(1,5,JOY_DOWN)*127);
		motorSet(mArmLeft,mArmLeftDir*arm);
		motorSet(mArmRight,mArmRightDir*arm);

		lcdUpdate();
		delay(20);
	}
}
