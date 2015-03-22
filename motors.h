/*
 * motors.h
 *
 *  Created on: Mar 19, 2015
 *      Author: Kevin
 */

#ifndef MOTORS_H_
#define MOTORS_H_

#define mArmLeft         1
#define mArmLeftDir      1

#define mBottomLeft      4
#define mBottomLeftDir   1

#define mBottomRight     5
#define mBottomRightDir -1

#define mTopLeft         6
#define mTopLeftDir      1

#define mTopRight        7
#define mTopRightDir    -1

#define mArmRight        9
#define mArmRightDir     1

// the minimum motor power to move the drive train
#define minDrive 20

Encoder eLeft;
Encoder eRight;

#endif /* MOTORS_H_ */
