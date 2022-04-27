/*
 * dma.c
 *
 *  Created on: Apr 27, 2022
 *      Author: lpandit
 */

#include "MKL25Z4.h"

static uint16_t * DMA_Source;
static uint32_t   DMA_Byte_Count;

/*
 *
 */
void Init_DMA(uint16_t * source, uint32_t count)
{

	DMA_Source = source;
	DMA_Byte_Count = count;

	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	DMAMUX0->CHCFG[0] = 0;

	DMA0->DMA[0].DCR = DMA_DCR_EINT_MASK | DMA_DCR_SINC_MASK |
			DMA_DCR_SSIZE(2) | DMA_DCR_DSIZE(2) | DMA_DCR_ERQ_MASK | DMA_DCR_CS_MASK;

	NVIC_SetPriority(DMA0_IRQn, 2);
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	NVIC_EnableIRQ(DMA0_IRQn);

	DMAMUX0->CHCFG[0] = DMAMUX_CHCFG_SOURCE(54);
}

/*
 *
 */
void Start_DMA_Playback()
{
	DMA0->DMA[0].SAR  = DMA_SAR_SAR((uint32_t) DMA_Source);
	DMA0->DMA[0].DAR = DMA_DAR_DAR((uint32_t) (&(DAC0->DAT[0])));

	DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(DMA_Byte_Count);

	DMA0->DMA[0].DSR_BCR &= ~DMA_DSR_BCR_DONE_MASK;

	DMAMUX0->CHCFG[0] |= DMAMUX_CHCFG_ENBL_MASK;
}

/*
 *
 */
void DMA0_IRQHandler(void)
{
    DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE_MASK;

    Start_DMA_Playback();
}
