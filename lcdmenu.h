/*
 * lcdmenu.h
 *
 *  Created on: Mar 19, 2015
 *      Author: Kevin
 */

#ifndef LCDMENU_H_
#define LCDMENU_H_

int autonColor;
int autonSide;
int autonNum;
void lcdRender();
void lcdUpdate();

void lcdSetCenter(FILE* uart,unsigned char line,const char* str);
char lcdStr[17];
#define lcdPrintCenter(port,line,fmt,...) snprintf(lcdStr,17,fmt,__VA_ARGS__); lcdSetCenter(port,line,lcdStr);

#endif /* LCDMENU_H_ */
