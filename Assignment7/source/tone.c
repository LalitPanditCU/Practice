/*
 * tone.c
 *
 *  Created on: Apr 27, 2022
 *      Author: lpandit
 */
#include "tone.h"
#include "dma.h"

#include "sine.h"

#define  MAX_TABLE       512
#define  SAMPLING_FREQ   48000   //48 KHz

typedef enum  {
	f_440Hz = 440,
	f_587Hz = 587,
	f_659Hz = 659,
	f_880Hz = 880
} frequency_t;

typedef struct {
	frequency_t  f;
	uint16_t     tone[MAX_TABLE];
	uint32_t     count;
} plot_t;

plot_t  p_440hz,
        p_587hz,
		p_659hz,
		p_880hz;

plot_t  *c_ptr;

/*
 *
 */
void Init_Tone(void)
{
	p_440hz.f = f_440Hz;
	p_440hz.count = Init_Table(SAMPLING_FREQ, p_440hz.f, p_440hz.tone);

	p_587hz.f = f_587Hz;
	p_587hz.count = Init_Table(SAMPLING_FREQ, p_587hz.f, p_587hz.tone);

	p_659hz.f = f_659Hz;
	p_659hz.count = Init_Table(SAMPLING_FREQ, p_659hz.f, p_659hz.tone);

	p_880hz.f = f_880Hz;
	p_880hz.count = Init_Table(SAMPLING_FREQ, p_880hz.f, p_880hz.tone);

}

/*
 *
 */
void tone_callback(void)
{
	static int count = 0;

	count++;

	if (count == c_ptr->f)
	{
		switch (c_ptr->f)
		{
		case  f_440Hz:
			c_ptr = &p_587hz;
			break;
		case f_587Hz:
			c_ptr = &p_659hz;
			break;
		case f_659Hz:
			c_ptr = &p_880hz;
			break;
		case f_880Hz:
			c_ptr = &p_440hz;
			break;
		}
		count = 0;
	    Init_DMA(c_ptr->tone, c_ptr->count * 2, tone_callback);
	}

}

/*
 *
 */
void Play_Tones()
{
	 c_ptr = &p_440hz;
     Init_DMA(c_ptr->tone, c_ptr->count * 2, tone_callback);
     Start_DMA_Playback();
}
