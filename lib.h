/*
 * lib.h
 *
 *  Created on: Mar 18, 2015
 *      Author: Kevin
 */

#ifndef LIB_H_
#define LIB_H_

double favg(double a,double b);
float favgf(float a,float b);
double fclamp(double n,double min,double max);
float fclampf(float n,float min,float max);
double fround(double a,double n);
float froundf(float a,float n);
int min(int a,int b);
int max(int a,int b);
int clamp(int n,int min,int max);
int mod(int a,int b);

#endif /* LIB_H_ */
