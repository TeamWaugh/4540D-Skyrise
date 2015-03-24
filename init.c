#include "main.h"
#include "motors.h"
#include "lcdmenu.h"
#include "sensors.h"

void initializeIO() {

}

void initialize() {
	eLeft=encoderInit(sEncLeftTop,sEncLeft,sEncLeftDir==1?1:-1);
	encoderReset(eLeft);
	eRight=encoderInit(sEncRight,sEncRightTop,sEncRightDir==1?1:-1);
	encoderReset(eRight);
	lcdInit(uart1);
	lcdSetBacklight(uart1,true);
	lcdRender();
	while (!isEnabled()) {
		lcdUpdate();
		wait(50);
	}
}
