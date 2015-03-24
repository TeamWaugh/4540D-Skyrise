/*
 * encoders.h
 *
 *  Created on: Mar 18, 2015
 *      Author: Kevin
 */

#ifndef ENCODERS_H_
#define ENCODERS_H_

void enc(float l,float r,int edist);
void forward(int dist);
void backward(int dist);
#define encToDeg 3.5
void turnLeft(int deg);
void turnRight(int deg);
int encReverse;

#endif /* ENCODERS_H_ */
