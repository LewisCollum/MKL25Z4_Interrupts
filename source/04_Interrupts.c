#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "delay.h"
#include "keypad.h"
#include "lcd.h"
#include "common.h"
#include "uart.h"

unsigned char key;
unsigned char uartKey;

enum RunStates {
	waiting, sending, fetching
} state;

void setup() {
	__disable_irq();

	BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

	state = waiting;
	__enable_irq();
}

void run() {
	while (state == waiting);

	__disable_irq();
	if (state == sending) {
		lcdWriteDataToRow(key, 0);
		uartSend(key);
		while(uartIsSending());
		state = waiting;
	}
	else if (state == fetching) {
		lcdWriteDataToRow(uartKey, 1);
		state = waiting;
	}
	__enable_irq();
}

int main(void) {
	setup();
	while(1) run();
    return 0;
}

void PORTD_IRQHandler() {
	if(state == waiting) {
		key = keypadGetPressedKey();
		state = sending;
	}
	PORTD->ISFR = 0xF;
}

void UART0_IRQHandler() {
	if(state == waiting) {
		uartKey = UART0->D;
		state = fetching;
	}
}


