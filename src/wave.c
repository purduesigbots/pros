/*
 * wave.c - VEX Speaker support, wave table generator, and synth
 *
 * Copyright (c) 2011-2016, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include <periph.h>
#include <semphr.h>
#include <wave.h>

// FORMULA: Entry = round(32768 * F_note / F_sample) | F_sample = 20K
// That's 71 notes and rest
#define NUM_NOTES 72

// FLASH Wave table for tone generation (+/- 12 bits = 13 total bits)
static const int16_t SINE_WAVE[128] = {
	0, 203, 405, 606, 805, 1003, 1198, 1390, 1579, 1764,
	1944, 2120, 2291, 2456, 2615, 2768, 2913, 3052, 3183, 3307, 3422, 3529, 3628, 3717, 3798,
	3869, 3930, 3982, 4025, 4057, 4080, 4092, 4095, 4087, 4070, 4042, 4005, 3958, 3901, 3834,
	3759, 3674, 3580, 3477, 3366, 3246, 3119, 2984, 2841, 2692, 2536, 2374, 2206, 2033, 1855,
	1672, 1485, 1294, 1101, 904, 706, 505, 304, 101, -101, -304, -505, -706, -904, -1101, -1294,
	-1485, -1672, -1855, -2033, -2206, -2374, -2536, -2692, -2841, -2984, -3119, -3246, -3366,
	-3477, -3580, -3674, -3759, -3834, -3901, -3958, -4005, -4042, -4070, -4087, -4095, -4092,
	-4080, -4057, -4025, -3982, -3930, -3869, -3798, -3717, -3628, -3529, -3422, -3307, -3183,
	-3052, -2913, -2768, -2615, -2456, -2291, -2120, -1944, -1764, -1579, -1390, -1198, -1003,
	-805, -606, -405, -203, 0,
};

// A way to look up the notes using an 8-bit index, saving codespace for the song!
static const uint16_t NOTE_TABLE[NUM_NOTES] = {
	0, 114, 121, 128, 135, 143, 152, 161, 170, 180,
	191, 203, 215, 227, 241, 255, 270, 286, 303, 321, 340, 361, 382, 405, 429, 454, 481, 510,
	540, 572, 606, 642, 681, 721, 764, 809, 858, 909, 963, 1020, 1080, 1145, 1213, 1285, 1361,
	1442, 1528, 1619, 1715, 1817, 1925, 2039, 2160, 2289, 2425, 2569, 2722, 2884, 3055, 3237,
	3429, 3633, 3849, 4078, 4320, 4577, 4850, 5138, 5443, 5767, 6110, 6473,
};

// Note correction table to fixup the sharp and flat signs
static const uint8_t NOTE_CORRECTION[7] = { 10, 12, 1, 3, 5, 6, 8 };

// Output for the wave, this is local here so the optimizer will cut it if no speaker usage
#define WAVE_BUFFER_SIZE 256
static uint16_t waveOutput[WAVE_BUFFER_SIZE];

// Wavetable generator status structure
typedef struct {
	// Sample time within the note
	volatile uint16_t sampleTime;
	// Current sample increase rate
	volatile uint16_t sample;
	// Countdown timer for note duration
	volatile uint16_t countdown;
	uint16_t RESERVED;
} WaveStatus;

// RTTTL track default structure
typedef struct {
	// Track tempo in BPM
	uint16_t tempo;
	// Track default octave #
	uint8_t octave;
	// Track default duration
	uint8_t duration;
} TrackSettings;

// Wave channel statuses
#define WAVE_NUM 3
static WaveStatus wave[WAVE_NUM];
static Semaphore waveNeeded;

// Added to the signed values to get a middle of the road value
#define WAVE_DC_OFFSET 2047

// TEST CASE
#if 0
	const char * rttl[] = {
		"Super Mario:d=4,o=5,b=100:"
		"16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,"
		"8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,"
		"8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,"
		"8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,"
		"8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,8p.,"
		"8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,"
		"8p,16d#6,8p,16d6,8p,16c6",

		"Super Mario:d=4,o=4,b=100:"
		"16d,16d,32p,8d,16d,8d,8g5,8p,8g,8p,"
		"8g5,16p,8e,16p,8c,16p,8f,8g,16f#,8f,16e.,16c5,16e5,8f5,16d5,8e5,8c5,16a,16b,8g,16p,"
		"8g5,16p,8e,16p,8c,16p,8f,8g,16f#,8f,16e.,16c5,16e5,8f5,16d5,8e5,8c5,16a,16b,8g,16p,"
		"8c,16p,16g,8p,8c5,8f,16p,16c5,16c5,16c5,8f,"
		"8c,16p,16e,8p,16g,16c5,p.,8g,"
		"8c,16p,16g,8p,8c5,8f,16p,16c5,16c5,16c5,8f,"
		"8c,16g#,8p,16a#,8p,16c5",

		NULL,
	};
#endif

// speakerStop - Stops sound from the speaker
static void speakerStop() {
	// Load the array with 50% values
	for (uint32_t i = 0; i < WAVE_BUFFER_SIZE; i++)
		waveOutput[i] = WAVE_DC_OFFSET;
	// Reset waves to default (0)
	uint32_t *alias = (uint32_t *)&wave[0];
	for (uint32_t i = 0; i < (WAVE_NUM << 1); i++)
		alias[i] = 0;
}

// speakerInit - Prepares speaker for use and enables interrupt
// Speaker library increases RAM usage by ~1K and FLASH usage by ~4K, be warned!
void speakerInit() {
	// Prepare semaphore
	waveNeeded = semaphoreCreate();
	semaphoreTake(waveNeeded, 0UL);
	speakerStop();
	// Set up DMA location and count
	DMA2_Channel3->CMAR = (uint32_t)(&waveOutput[0]);
	DMA2_Channel3->CNDTR = (uint32_t)WAVE_BUFFER_SIZE;
	// Turn on DMA
	DMA2_Channel3->CCR |= DMA_CCR_EN;
}

// speakerParseNumber - Helper for speakerPlayArray
static const char * _speakerParseNumber(const char *ptr, uint32_t *number) {
	uint32_t n = 0;
	char c;
	// Iterate until non numeric or out of characters
	while ((c = *ptr) != 0 && c >= '0' && c <= '9') {
		// No sign-extension can occur here as c has MSB clear!
		n = (10 * n) + (uint32_t)(c - '0');
		ptr++;
	}
	*number = n;
	return ptr;
}

// _speakerParseSettings - Inline helper for speakerPlayArray
static INLINE const char * _speakerParseSettings(const char *ptr, TrackSettings *ts) {
	char c; uint32_t ret;
	// Strip the names from each one
	while ((c = *ptr++) != 0 && c != ':');
	if (c)
		// Pull the default settings: d=#,b=#,o=#
		while ((c = *ptr++) != 0 && c != ':')
			if (*ptr == '=') {
				ptr++;
				// b=, d=, or o=
				if (c == 'b') {
					// Beats per minute
					ptr = _speakerParseNumber(ptr, &ret);
					if (ret < 30) ret = 30;
					ts->tempo = (uint16_t)ret;
				} else if (c == 'd') {
					// Duration
					ptr = _speakerParseNumber(ptr, &ret);
					if (ret < 1 || ret > 32)
						ret = 0;
					else
						ret = 32 / ret;
					ts->duration = (uint8_t)ret;
				} else if (c == 'o') {
					// Only 3-7
					ptr = _speakerParseNumber(ptr, &ret);
					if (ret < 3 || ret > 7)
						ret = 5;
					ts->octave = (uint8_t)ret;
				}
			}
	return ptr;
}

// _speakerNextNote - Inline helper for speakerPlayArray
static INLINE const char * _speakerNextNote(const char *ptr, uint32_t num, TrackSettings *def) {
	char c;
	uint32_t dur = 0, oct = 0, note = 0;
	// Scan for comma delimited notes
	while ((c = *ptr) != 0) {
		if (c != ' ') {
			// Duration in 64ths
			ptr = _speakerParseNumber(ptr, &dur);
			if (dur < 1 || dur > 32)
				dur = (uint32_t)def->duration;
			else
				dur = 32 / dur;
			c = *ptr;
			if (c >= 'a' && c <= 'g') {
				// No sign-extension can occur here as c has MSB clear!
				note = (uint32_t)NOTE_CORRECTION[(uint32_t)(c - 'a')];
				ptr++;
			} else if (c == 'p') {
				// Pause
				note = 0;
				ptr++;
			}
			c = *ptr;
			if (c == '#' || c == '_') {
				// Sharp note
				note++;
				ptr++;
			}
			// Dot?
			if (*ptr == '.') {
				dur = (3 * dur) >> 1;
				ptr++;
			}
			// Octave 3-7
			ptr = _speakerParseNumber(ptr, &oct);
			// Pull defaults if invalid values found
			if (oct < 3 || oct > 7)
				oct = def->octave;
			// Write out to array
			wave[num].countdown = (uint16_t)(dur * (uint32_t)def->tempo);
			if (note != 0)
				note = note + oct * 12 - 25;
			wave[num].sample = NOTE_TABLE[note];
			// Wait for the ','
			while ((c = *ptr) != 0 && c != ',') ptr++;
			if (c) ptr++;
			return ptr;
		}
		ptr++;
	}
	// At end of string
	wave[num].countdown = 0;
	wave[num].sample = 0;
	return ptr;
}

#if 0
// speakerPlayArray - Play up to 3 RTTTL tracks simultaneously
void speakerPlayArray(unsigned const char * const * const note,
		unsigned const char * const * const dur, unsigned char tempo) {
	// Calculate new tempo
	uint32_t newTempo = (uint16_t)(4688U / (uint32_t)tempo);
	unsigned const char * rNote[WAVE_NUM]; unsigned const char * rDur[WAVE_NUM];
	// Copy to new array
	for (uint32_t i = 0; i < WAVE_NUM) {
		rNote[i] = (const char *)note[i];
		rDur[i] = (const char *)dur[i];
	}
	bool status;
	// Start the timers of war
	TIM6->EGR = TIM_EGR_UG;
	DMA2->IFCR = DMA_IFCR_CGIF3;
	TIM6->CR1 |= TIM_CR1_CEN;
	do {
		uint16_t wcount;
		status = true;
		for (uint32_t i = 0; i < WAVE_NUM; i++) {
			wcount = wave[i].countdown;
			if (wcount == 2)
				// Short pauses between notes
				wave[i].sample = 0;
			else if (wcount == 0) {
				// Done with note
				wcount = (uint16_t)((uint32_t)(*(rDur[i])++) * newTempo);
				if (wcount == 0) status = false;
				wave[i].countdown = wcount;
				wave[i].sample = NOTE_TABLE[*(rNote[i])++];
			}
		}
		// Wait for the speaker data to become available
		semaphoreTake(waveNeeded, MAX_DELAY);
	} while (status);
	// All done, stop the timer, do NOT reset DAC to 0 or 2048 or we will click and pop
	TIM6->CR1 &= ~TIM_CR1_CEN;
	speakerStop();
}

#endif

// speakerPlayArray - Play up to 3 RTTTL tracks simultaneously
void speakerPlayArray(const char * * rttl) {
	TrackSettings ts[3]; bool status;
	for (uint32_t i = 0; i < WAVE_NUM; i++) {
		const char *ptr = rttl[i];
		if (ptr != NULL) {
			// Prepare RTTTL string
			rttl[i] = _speakerParseSettings(ptr, &ts[i]);
			ts[i].tempo = (uint16_t)(1172U / (uint32_t)ts[i].tempo);
		}
	}
	// Start the timers of war
	TIM6->EGR = TIM_EGR_UG;
	DMA2->IFCR = DMA_IFCR_CGIF3;
	TIM6->CR1 |= TIM_CR1_CEN;
	do {
		uint16_t wcount;
		status = true;
		for (uint32_t i = 0; i < WAVE_NUM; i++) {
			wcount = wave[i].countdown;
			if (wcount == 2)
				// Short pauses between notes
				wave[i].sample = 0;
			else if (wcount == 0) {
				// Done with note
				if (rttl[i]) {
					rttl[i] = _speakerNextNote(rttl[i], i, &ts[i]);
					if (wave[i].countdown == 0) status = false;
				} else
					wave[i].countdown = 1;
			}
		}
		// Wait for the speaker data to become available
		semaphoreTake(waveNeeded, MAX_DELAY);
	} while (status);
	// All done, stop the timer, do NOT reset DAC to 0 or 2048 or we will click and pop
	TIM6->CR1 &= ~TIM_CR1_CEN;
	speakerStop();
}

// speakerPlayRtttl - Play a single RTTTL string encoded track
// To play polyphonic sound, use speakerPlayArray
void speakerPlayRtttl(const char *rttl) {
	const char * all_rttl[3];
	// Init one track
	all_rttl[0] = rttl;
	all_rttl[1] = NULL;
	all_rttl[2] = NULL;
	speakerPlayArray(all_rttl);
}

// speakerShutdown - Power down the speaker, kill interrupt, stop tones
void speakerShutdown() {
	semaphoreDelete(waveNeeded);
	// Turn off the DMA and timer
	TIM6->CR1 &= ~TIM_CR1_CEN;
	DMA2_Channel3->CCR &= ~DMA_CCR_EN;
	// Wait for the accesses to flush
	__dsb();
	// Clear DAC back to default (0), may cause click and pop?
	DAC->DHR12R1 = (uint32_t)0;
}

// ISR_DMA2_Channel3 - "Transfer complete" ISR that reloads the data array with next samples
// Also fires halfway through to load the beginning half (so that any latency will avoid noise
// at the update frequency)
IRQ ISR_DMA2_Channel3() {
	register int32_t out;
	register uint16_t t0, t1, s0, s1, t2, s2;
	uint16_t *w;
	// Shut off alarm clock
	uint32_t flags = DMA2->ISR;
	DMA2->IFCR = DMA_IFCR_CGIF3;
	// Correct location
	if (flags & DMA_ISR_TCIF3)
		w = &waveOutput[WAVE_BUFFER_SIZE >> 1];
	else
		w = &waveOutput[0];
	t0 = wave[0].sampleTime;
	s0 = wave[0].sample;
	t1 = wave[1].sampleTime;
	s1 = wave[1].sample;
	t2 = wave[2].sampleTime;
	s2 = wave[2].sample;
	for (uint32_t i = (WAVE_BUFFER_SIZE >> 1); i; i--) {
		// WAVE CHANNEL 0
		// Overflow is acceptable here
		t0 += s0;
		// Get sine index by LSRing by 9
		out = (int32_t)SINE_WAVE[t0 >> 9];
		// WAVE CHANNEL 1
		t1 += s1;
		out += (int32_t)SINE_WAVE[t1 >> 9];
		// WAVE CHANNEL 2
		t2 += s2;
		out += (int32_t)SINE_WAVE[t2 >> 9];
		// We have a total of 14 bits (13+13) to pigeonhole into 10, sign bits are erased
		*w++ = (uint16_t)(((out + 4) >> 3) + WAVE_DC_OFFSET);
	}
	// Write back
	wave[0].sampleTime = t0;
	wave[1].sampleTime = t1;
	wave[2].sampleTime = t2;
	if (flags & DMA_ISR_TCIF3) {
		wave[0].countdown--;
		wave[1].countdown--;
		wave[2].countdown--;
		// Give control to whichever task is updating sound
		bool cs = false;
		semaphoreGiveISR(waveNeeded, &cs);
		if (cs) _taskYield();
	}
}
