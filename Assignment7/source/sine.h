/*
 * sine.h
 *
 *  Created on: Apr 25, 2022
 *      Author: lpandit
 */

#ifndef SINE_H_
#define SINE_H_

#include <stdint.h>

#define BITS 				(12)
/*
#define TRIG_SCALE_FACTOR	((1 << BITS)-2)
#define TABLE_SIZE          (6431)      //pi/2 * TRIG_SCALE_FACTOR
*/

#define TABLE_SIZE          3213
#define TRIG_SCALE_FACTOR   2045


#define PI_BY2              (TABLE_SIZE-1)  //pi/2 * TRIG_SCALE_FACTOR
#define PI      			(PI_BY2 + PI_BY2)
#define PI3_BY2 			(PI + PI_BY2)
#define TWO_PI     			(PI + PI)

void Init_Sine();
int fp_sin(int i);
uint32_t Init_Table(uint32_t s_freq, uint32_t t_freq, uint16_t *data);


#endif /* SINE_H_ */
