/*
 * dma.h
 *
 *  Created on: Apr 27, 2022
 *      Author: lpandit
 */

#ifndef DMA_H_
#define DMA_H_

#include <stdint.h>

void Init_DMA(int16_t * source, uint32_t count);
void Start_DMA_Playback();
void DMA0_IRQHandler(void);

#endif /* DMA_H_ */

