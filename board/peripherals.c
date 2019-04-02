#include "peripherals.h"
#include "MKL25Z4.h"
#include "delay.h"
#include "lcd.h"
#include "keypad.h"
#include "common.h"
#include "uart.h"

void clockEnable();
void lcdInitDecorated();

void BOARD_InitBootPeripherals(void) {
	clockEnable();
	lcdInitDecorated();
	keypadInit();
	uartInit();
}

void clockEnable() {
	SIM->SCGC5 |= clockPortC;
	SIM->SCGC5 |= clockPortD;
	SIM->SCGC5 |= clockPortE;
}

void lcdInitDecorated() {
	lcdInit();
	lcdCommand(lcd4Bit | lcd2Line | lcdSkinny);
	lcdCommand(lcdOn | lcdCursorOn | lcdCursorBlinkOn);
	lcdCommand(lcdClear);
	lcdEnableClearAtColumnMax(16);
}



