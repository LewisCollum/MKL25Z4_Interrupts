#include "keypad.h"
#include "MKL25Z4.h"
#include "delay.h"
#include "common.h"

const char hexKeys[] = {
		'1', '2', '3', 'A',
		'4', '5', '6', 'B',
		'7', '8', '9', 'C',
		'*', '0', '#', 'D'
};

unsigned char rowCurrent = 0;
unsigned char columnCurrent = 0;
unsigned char columnMask = 0;
unsigned char rowMask = 0;

void keypadDriveRowLow();

void keypadInit() {
	for(int i = 0; i <= 3; ++i) {
		PORTC->PCR[i] = gpio | pullup;
		PORTD->PCR[i] &= ~0xF0000;
		PORTD->PCR[i] = gpio | pullup | 0xA0000;
	}
	PTC->PDDR |= 0xF;
	PTD->PDDR &= ~0xF;
	keypadDriveRowLow();
	NVIC_EnableIRQ(PORTD_IRQn);
	NVIC_SetPriority(PORTD_IRQn, 3);
}

void keypadDriveRowLow() {
	PTC->PCOR = 0xF;
}

char keypadIsPressed() {
	return PTD->PDIR & 0xF? 0: 1;
}

void keypadScan() {
	columnCurrent = 0;
	for(rowCurrent = 0; rowCurrent <= 3; ++rowCurrent) {
		rowMask = 1 << rowCurrent;
		PTC->PDOR = ~rowMask & 0xF; //111..1110, 111..1101, etc.
		delayUs(5);
		columnMask = ~PTD->PDIR & 0xF;
		if (columnMask) {
			unsigned char columnMaskCopy = columnMask;
			while (columnMaskCopy >>= 1)
				++columnCurrent;
			break;
		}
	}
	keypadDriveRowLow();
}

char keypadGetPressedKey() {
	keypadScan();
	if (!columnMask) return 0;
	return hexKeys[rowCurrent * 4 + columnCurrent];
}
