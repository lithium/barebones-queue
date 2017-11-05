#include "vga/vga.h"
#include "arch/x86.h"


static uint16_t *_VGAMEM = (uint16_t*)0xB8000;


int Printxy(int x, int y, char *str, uint8_t colors)
{
	uint16_t pos = (y * VGA_NUM_COLS + x);
	int ret = 0;

	for (; *str != 0; str++) {
		_VGAMEM[pos++] = *str | (colors<<8);
		ret++;
	}
	return ret;
}


static uint8_t _cursorRow = 0;
static uint8_t _cursorCol = 0;
static uint8_t _currentColor = 7;

int PrintColor(uint8_t color) {
	_currentColor = color;
}

int Print(char *str)
{
	int ret = Printxy(_cursorCol, _cursorRow, str, _currentColor);
	_cursorCol = (_cursorCol + ret) % VGA_NUM_COLS;

	CursorMoveto(_cursorCol, _cursorRow);
	return ret;
}
int Println(char *str)
{
	int ret = Print(str);

	_cursorCol = 0;
	if (_cursorRow < VGA_NUM_ROWS-1) {
		CursorMoveto(_cursorCol, ++_cursorRow);
	} else {
		ScrollUp(1, 7);
	}

	return ret;
}
void ScrollUp(int numRows, uint8_t clearColor)
{
	int ofs = numRows*VGA_NUM_COLS;
	for (int i=ofs; i < VGA_NUM_ROWS*VGA_NUM_COLS; i++) {
		_VGAMEM[i - ofs] = _VGAMEM[i];
	}
	for (int i = VGA_NUM_ROWS*VGA_NUM_COLS-ofs; i < VGA_NUM_ROWS*VGA_NUM_COLS; i++) {
		_VGAMEM[i] = clearColor<<8;
	}
}



void CursorDisable()
{
	OUTB(0x3d4, 0x0A);
	OUTB(0x3d5, 0x20);
}

void CursorEnable(uint8_t scanlineStart, uint8_t scanlineEnd)
{
	OUTB(0x3d4, 0x0A);
	OUTB(0x3d5, (INB(0x3d5) & 0xC0) | scanlineStart);

	OUTB(0x3d4, 0x0B);
	OUTB(0x3e0, (INB(0x3e0) & 0xE0) | scanlineEnd);
}

void CursorMoveto(int x, int y)
{
	uint16_t pos = y * VGA_NUM_COLS + x;

	OUTB(0x3d4, 0x0F);
	OUTB(0x3d5, pos & 0xFF);
	OUTB(0x3d4, 0x0E);
	OUTB(0x3d5, (pos>>8) & 0xFF);

}

