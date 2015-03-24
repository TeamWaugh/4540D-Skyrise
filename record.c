#include "main.h"
#include "motors.h"
#include "sensors.h"
#include "record.h"

volatile typedef struct {
	unsigned int dt;
	int ldif;
	int rdif;
	unsigned short arm;
} recordFrame;

recordFrame cFrame;
FILE* cStream;
unsigned long lastTime;
int lastEncLeft;
int lastEncRight;

void recordInit(const char* name) {
	cStream=fopen(name,"w");
	lastTime=millis();
	lastEncLeft=encoderGet(eLeft)*sEncLeftDir;
	lastEncRight=encoderGet(eRight)*sEncRightDir;
}

void fputns(const char* str,size_t n,FILE* stream) {
	for (int i=0;i<n;i++) fputc(str[n],stream);
}

void recordUpdate() {
	int cEncLeft=encoderGet(eLeft)*sEncLeftDir;
	cFrame.ldif=cEncLeft-lastEncLeft;
	lastEncLeft=cEncLeft;
	int cEncRight=encoderGet(eRight);
	cFrame.rdif=cEncRight-lastEncRight;
	lastEncRight=cEncRight*sEncRightDir;
	cFrame.arm=((analogRead(sArmLeft)*sArmLeftDir)+(analogRead(sArmRight)*sArmRightDir))/2;
	if (cFrame.ldif!=0&&cFrame.rdif!=0&&cFrame.arm!=0) {
		cFrame.dt=millis()-lastTime;
		fputns((char*)&cFrame,sizeof(cFrame),cStream);
	}
}

void recordEnd() {
	recordUpdate();
	fclose(cStream);
}

void recordRead(const char* name) {
	cStream=fopen(name,"r");

}

