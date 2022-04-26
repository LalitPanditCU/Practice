/*
 * sine.h
 *
 *  Created on: Apr 25, 2022
 *      Author: lpandit
 */

#ifndef SINE_H_
#define SINE_H_

#define BITS 				(12)
#define TRIG_SCALE_FACTOR	(1 << 12)
#define TABLE_SIZE          (6434)      //[i/2 * TRIG_SCALE_FACTOR

#define PI_BY2              TABLE_SIZE  //pi/2 * TRIG_SCALE_FACTOR
#define PI      			(PI_BY2 + PI_BY2)
#define PI3_BY2 			(PI + PI_BY2)
#define TWO_PI     			(PI + PI)

void init_sine();
int fp_sin(int i);



#endif /* SINE_H_ */
