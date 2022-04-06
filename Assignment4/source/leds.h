/*
 * leds.h
 *
 *  Created on: Apr 4, 2022
 *      Author: lpandit
 */

#ifndef LEDS_H_
#define LEDS_H_

void red_led_on(uint8_t level);
void green_led_on(uint8_t level);
void blue_led_on(uint8_t level);
void init_leds();
void set_led_colors(uint32_t color);


#endif /* LEDS_H_ */
