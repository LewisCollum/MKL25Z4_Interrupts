#include "uart.h"
#include "MKL25Z4.h"

void uartInit() {
	const uint32_t transmitterEnable = 0b00000100;
	const uint32_t receiverEnable = 0b0001000;
	const uint32_t receiverFullInterrupt = 0b00100000;
	SIM->SCGC4 |= 0x400;
	SIM->SOPT2 |= 0x4000000; //System Clock
	UART0->C2 = 0;
	UART0->BDH = 0x00;
	UART0->BDL = 0x1a;
	UART0->C4 = 0x0F;
	UART0->C1 = 0x00;
	UART0->C2 = transmitterEnable | receiverEnable | receiverFullInterrupt;
	NVIC_EnableIRQ(UART0_IRQn);
	NVIC_SetPriority(UART0_IRQn, 2);

	SIM->SCGC5 |= 0x200;
	PORTA->PCR[1] = 0x200;
}

void uartSend(unsigned char data) {
	UART0->D = data;
}

char uartIsSending() {
	return !(UART0->S1 & UART0_S1_TDRE_MASK);
}
