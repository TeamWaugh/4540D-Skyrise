#include "main.h"
#include <string.h>
#include <math.h>
#include "lib.h"
#include "lcdmenu.h"
#include "motors.h"
#include "sensors.h"

int autonColor=1;
int autonSide=1;
int autonNum=1;

int mSelect=1;
int mNum=0;

void lcdSetCenter(FILE* uart,unsigned char line,const char* str) {
	int len=min(strlen(str),16);
	int off=(16-len)/2;
	char out[17]="                ";
	for (int i=0;((i<len)&(i<17));i++) {
		out[off+i]=str[i];
	}
	out[16]=0;
	lcdSetText(uart,line,out);
}

void lcdRender() {
	switch (mNum) {
		case 0:
			mSelect=mod(mSelect,3);
			lcdPrintCenter(uart1,1,"4540D %d.%dv",powerLevelMain()/1000,(powerLevelMain()%100)/10);
			lcdPrint(uart1,2,"%cDebug%cRed%cBlue",
				mSelect==0?0xF6:0x20,
				mSelect==1?0xF6:0x20,
				mSelect==2?0xF6:0x20
			);
		break;
		case 1:
			mSelect=mod(mSelect,5);
			switch (mSelect) {
				case 0:
					lcdSetCenter(uart1,1,"Sensors");
					lcdSetText(uart1,2," < Back       >");
				break;
				case 1:
					lcdPrintCenter(uart1,1,"%d.%dv",powerLevelBackup()/1000,(powerLevelBackup()%100)/10);
					lcdSetText(uart1,2," < Backup     >");
				break;
				case 2:
					lcdPrintCenter(uart1,1,"%d %d",encoderGet(eLeft)*sEncLeftDir,encoderGet(eRight)*sEncRightDir);
					lcdSetText(uart1,2," < Encoders   >");
				break;
				case 3:
					lcdPrintCenter(uart1,1,"%d %d",analogRead(sArmLeft)*sArmLeftDir,analogRead(sArmRight)*sArmRightDir);
					lcdSetText(uart1,2," < Arm        >");
				break;
				case 4:
					lcdPrintCenter(uart1,1,"%d",analogRead(sClawTilt)*sClawTiltDir);
					lcdSetText(uart1,2," < Claw       >");
				break;
			}
		break;
		case 2:
			mSelect=mod(mSelect,3);
			lcdSetCenter(uart1,1,autonColor==1?" Red":" Blue");
			lcdPrint(uart1,2,"%cBack%cLeft%cRight",
				mSelect==0?0xF6:0x20,
				mSelect==1?0xF6:0x20,
				mSelect==2?0xF6:0x20
			);
		break;
		case 3:
			mSelect=mod(mSelect,5);
			lcdPrintCenter(uart1,1,"%s %s %d",autonColor==1?" Red":" Blue",autonSide==1?"Left":"Right",autonNum);
			lcdPrint(uart1,2,"%cBack%c1%c2%c3%c4",
				mSelect==0?0xF6:0x20,
				mSelect==1?0xF6:0x20,
				mSelect==2?0xF6:0x20,
				mSelect==3?0xF6:0x20,
				mSelect==4?0xF6:0x20
			);
		break;
	}
}

unsigned char readBtns() {
	// use joystick buttons to control menu only when robot is disabled
	return lcdReadButtons(uart1)|(((!isEnabled())*7)&(
		(joystickGetDigital(1,7,JOY_LEFT)*LCD_BTN_LEFT)|
		(joystickGetDigital(1,7,JOY_DOWN)*LCD_BTN_CENTER)|
		(joystickGetDigital(1,7,JOY_UP)*LCD_BTN_CENTER)|
		(joystickGetDigital(1,7,JOY_RIGHT)*LCD_BTN_RIGHT)
	));
}

void lcdUpdate() {
	unsigned char btn=readBtns();
	if (btn==LCD_BTN_LEFT) {
		mSelect--;
		lcdRender();
	} else if (btn==LCD_BTN_RIGHT) {
		mSelect++;
		lcdRender();
	} else if (btn==LCD_BTN_CENTER) {
		switch (mNum) {
			case 0:
				if (mSelect==0) {
					mNum=1;
				} else {
					if (mSelect==1) {
						autonColor=1;
					} else if (mSelect==2) {
						autonColor=2;
					}
					mNum=2;
					mSelect=1;
				}
			break;
			case 1:
				if (mSelect==0) {
					mNum=0;
				}
			break;
			case 2:
				if (mSelect==0) {
					mNum=0;
				} else {
					if (mSelect==1) {
						autonSide=1;
					} else if (mSelect==2) {
						autonSide=2;
					}
					mNum=3;
				}
				mSelect=1;
			break;
			case 3:
				if (mSelect==0) {
					mNum=2;
					mSelect=1;
				} else {
					autonNum=mSelect;
				}
			break;
		}
		lcdRender();
	} else if (mNum==1||btn) {
		lcdRender();
	}
	// wait until button is released
	if (btn) {
		while (readBtns()) wait(50);
	}
}
