#include "lcd.h"
#include "MKL25Z4.h"
#include "delay.h"
#include "common.h"

#define RS 4
#define EN 8

unsigned int columnMax;
unsigned int lcdCursorColumn[] = {0, 0};
const unsigned int lcdCursorRowBegin[] = {0x80, 0xc0};
const unsigned int lcdCursorRowEnd[] = {0xa7, 0xe7};
char clearEnabledAtMax = false;

void setRow(unsigned int row);
void lcdWriteNibble(unsigned char data, unsigned char control);

void lcdInit() {
	for (int i = 8; i <= 13; ++i) {
		PORTC->PCR[i] = gpio;
		PTC->PDDR |= 1 << i;
	}

	delayMs(30);                /* initialization sequence */
	lcdWriteNibble(0x30, 0);
	delayMs(10);
	lcdWriteNibble(0x30, 0);
	delayMs(1);
	lcdWriteNibble(0x30, 0);
	delayMs(1);
	lcdWriteNibble(0x20, 0);  /* use 4-bit data mode */
	delayMs(1);
}

void lcdWriteNibble(unsigned char data, unsigned char control) {
    data &= 0xF0;       /* clear lower nibble for control */
    control &= 0x0F;    /* clear upper nibble for data */
    PTC->PDOR |= (data | control) << 6;       /* RS = 0, R/W = 0 */
    PTC->PDOR |= (data | control | EN) << 6;  /* pulse E */
    delayMs(0);
    PTC->PDOR = data;
    PTC->PDOR = 0;
}

void lcdCommand(unsigned char command) {
    lcdWriteNibble(command & 0xF0, 0);   /* upper nibble first */
    lcdWriteNibble(command << 4, 0);     /* then lower nibble */

    if (command < 4)
        delayMs(4);         /* commands 1 and 2 need up to 1.64ms */
    else
        delayMs(1);         /* all others 40 us */
}

void lcdWriteData(unsigned char data) {
    lcdWriteNibble(data & 0xF0, RS);    /* upper nibble first */
    lcdWriteNibble(data << 4, RS);      /* then lower nibble  */
    delayMs(1);
}

void lcdSetRowColumn(unsigned int row, unsigned int column) {
	if (lcdCursorColumn[row] != column)
		lcdCursorColumn[row] = column;
	lcdCommand(lcdCursorRowBegin[row] | column);
}

unsigned char isColumnMaxAtRow(unsigned int row) {
	return columnMax == lcdCursorColumn[row];
}

void lcdClearRow(unsigned int row) {
	lcdSetRowColumn(row, 0);
	while(!isColumnMaxAtRow(row))
		lcdWriteDataToRow(' ', row);

	lcdSetRowColumn(row, 0);
}

void lcdWriteDataToRow(unsigned char data, unsigned int row) {
	lcdSetRowColumn(row, lcdCursorColumn[row]);
	if (clearEnabledAtMax && isColumnMaxAtRow(row)) {
		lcdClearRow(row);
	}
	lcdWriteData(data);
	lcdCursorColumn[row]++;

}

void lcdEnableClearAtColumnMax(unsigned int max) {
	columnMax = max;
	clearEnabledAtMax = true;
}
