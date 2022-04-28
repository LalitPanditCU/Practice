/*
 * tone.c
 *
 *  Created on: Apr 27, 2022
 *      Author: lpandit
 */
#include "tone.h"
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
	int16_t      tone[MAX_TABLE];
	uint32_t     count;
} plot_t;

plot_t  p_440hz,
        p_587hz,
		p_659hz,
		p_880hz;

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
