#include "main.h"
#include "lib.h"
#include "motors.h"
#include "sensors.h"
#include "encoders.h"
#include "arm.h"
#include "record.h"

enum {
	ac_null,
	ac_move,
	ac_turn,
	ac_arm,
	ac_claw,
	ac_tilt,
};

volatile typedef struct {
	unsigned char action;
	long param;
} recordFrame;

recordFrame cFrame;
FILE* cStream;
unsigned long lastTime;

void fputns(const char* str,size_t n,FILE* stream) {
	for (int i=0;i<n;i++) fputc(str[i],stream);
}

void writeFrame() {
	if (cFrame.action!=ac_null) {
		fputns((const char*)&cFrame.action,sizeof(cFrame.action),cStream);
		fputns((const char*)&cFrame.param,sizeof(cFrame.param),cStream);
	}
}

int readFrame() {
	size_t len;
	len=fread((void*)&cFrame.action,sizeof(cFrame.action),1,cStream);
	if (len!=sizeof(cFrame.action)) return 0;
	len=fread((void*)&cFrame.param,sizeof(cFrame.param),1,cStream);
	if (len!=sizeof(cFrame.param)) return 0;
	return 1;
}

void newFrame(unsigned char action) {
	if (cFrame.action!=action) {
		writeFrame();
		cFrame.action=action;
		cFrame.param=0;
	}
}

void record(const char* name) {
	lcdPrint(uart1,1,"Recording %s",name);
	cStream=fopen(name,"w");
	cFrame.action=ac_null;
	while (!joystickGetDigital(1,7,JOY_RIGHT)) {
		char moveDir=joystickGetAnalog(1,3);
		char turnDir=joystickGetAnalog(1,1);
		int armDir=joystickGetDigital(1,5,JOY_UP)-joystickGetDigital(1,5,JOY_DOWN);
		int clawDir=joystickGetDigital(1,6,JOY_UP)-joystickGetDigital(1,6,JOY_DOWN);
		int tiltDir=joystickGetDigital(1,8,JOY_UP)-joystickGetDigital(1,8,JOY_DOWN);
		if (abs(moveDir)>=minDrive) {
			newFrame(ac_move);
			int lBase=encoderGet(eLeft);
			int rBase=encoderGet(eRight);
			while (abs(moveDir)>=minDrive) {
				motorSet(mTopLeft,mTopLeftDir*moveDir);
				motorSet(mBottomLeft,mBottomLeftDir*moveDir);
				motorSet(mTopRight,mTopRightDir*moveDir);
				motorSet(mBottomRight,mBottomRightDir*moveDir);
				moveDir=joystickGetAnalog(1,3);
				delay(50);
			}
			motorSet(mTopLeft,0);
			motorSet(mBottomLeft,0);
			motorSet(mTopRight,0);
			motorSet(mBottomRight,0);
			cFrame.param+=avg((encoderGet(eLeft)-lBase)*sEncLeftDir,(encoderGet(eRight)-rBase)*sEncRightDir);
		} else if (abs(turnDir)>=minDrive) {
			newFrame(ac_turn);
			int lBase=encoderGet(eLeft);
			int rBase=encoderGet(eRight);
			while (abs(turnDir)>=minDrive) {
				motorSet(mTopLeft,mTopLeftDir*turnDir);
				motorSet(mBottomLeft,mBottomLeftDir*turnDir);
				motorSet(mTopRight,mTopRightDir*-turnDir);
				motorSet(mBottomRight,mBottomRightDir*-turnDir);
				turnDir=joystickGetAnalog(1,1);
				delay(50);
			}
			motorSet(mTopLeft,0);
			motorSet(mBottomLeft,0);
			motorSet(mTopRight,0);
			motorSet(mBottomRight,0);
			cFrame.param+=avg((encoderGet(eLeft)-lBase)*sEncLeftDir,(encoderGet(eRight)-rBase)*-sEncRightDir)/encToDeg;
		} else if (armDir) {
			newFrame(ac_arm);
			while (armDir) {
				motorSet(mArmLeft,mArmLeftDir*armDir*127);
				motorSet(mArmRight,mArmRightDir*armDir*127);
				armDir=joystickGetDigital(1,5,JOY_UP)-joystickGetDigital(1,5,JOY_DOWN);
				delay(50);
			}
			motorSet(mArmLeft,0);
			motorSet(mArmRight,0);
			cFrame.param=avg(analogRead(sArmLeft)*sArmLeftDir,analogRead(sArmRight)*sArmRightDir);
		} else if (clawDir) {
			newFrame(ac_claw);
			char lClawDir=clawDir;
			unsigned long baseTime=millis();
			while (clawDir==lClawDir) {
				motorSet(mClaw,mClawDir*clawDir*127);
				clawDir=joystickGetDigital(1,6,JOY_UP)-joystickGetDigital(1,6,JOY_DOWN);
				delay(50);
			}
			motorSet(mClaw,mClawDir*clawDir*127);
			cFrame.param+=lClawDir*(millis()-baseTime);
		} else if (tiltDir) {
			newFrame(ac_tilt);
			char ltiltDir=tiltDir;
			unsigned long baseTime=millis();
			while (tiltDir==ltiltDir) {
				motorSet(mTilt,mTiltDir*tiltDir*127);
				tiltDir=joystickGetDigital(1,8,JOY_UP)-joystickGetDigital(1,8,JOY_DOWN);
				delay(50);
			}
			motorSet(mTilt,mTiltDir*tiltDir*127);
			cFrame.param+=ltiltDir*(millis()-baseTime);
		} else {
			delay(50);
		}
	}
	writeFrame();
	fclose(cStream);
}

void play(const char* name) {
	lcdPrint(uart1,1,"Playing %s",name);
	cStream=fopen(name,"r");
	while (readFrame()) {
		if (cFrame.action==ac_move) {
			lcdPrint(uart1,2,"forward %d",(int)cFrame.param);
			forward((int)cFrame.param);
		} else if (cFrame.action==ac_turn) {
			lcdPrint(uart1,2,"turn %d",(int)cFrame.param);
			turnRight((int)cFrame.param);
		} else if (cFrame.action==ac_arm) {
			lcdPrint(uart1,2,"arm %d",(int)cFrame.param);
			setArm(cFrame.param);
		} else if (cFrame.action==ac_claw) {
			motorSet(mClaw,mClawDir*(cFrame.param>0?127:-127));
			delay(abs(cFrame.param));
			motorSet(mClaw,0);
		} else if (cFrame.action==ac_tilt) {
			motorSet(mTilt,mTiltDir*(cFrame.param>0?127:-127));
			delay(abs(cFrame.param));
			motorSet(mTilt,0);
		}
	}
	fclose(cStream);
}

