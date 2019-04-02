#ifndef LCD_H_
#define LCD_H_

enum lcdCommand {
	lcdClear = 0x1
};

enum onOffControl {
	lcdOn = 0b1100,
	lcdCursorOn = 0b1010,
	lcdCursorBlinkOn = 0b1001
};

enum functionSet {
	lcd8Bit = 0b110000,
	lcd4Bit = 0b100000,

	lcd2Line = 0b101000,
	lcd1Line = 0b100000,

	lcdWide = 0b100100,
	lcdSkinny = 0b100000
};

void lcdInit();
void lcdCommand(unsigned char command);
void lcdWriteData(unsigned char data);
void lcdWriteDataToRow(unsigned char data, unsigned int row);
void lcdSetRowColumn(unsigned int row, unsigned int column);
void lcdEnableClearAtColumnMax(unsigned int max);

#endif
