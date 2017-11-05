#ifndef BAREBONES_VGA_H
#define BAREBONES_VGA_H

#include <inttypes.h>


#define VGA_NUM_COLS 80
#define VGA_NUM_ROWS 25


int Printxy(int x, int y, char *str, int len, uint8_t colors);
int PrintColor(uint8_t color);
int Printn(char *str, int len);
int Printnln(char *str, int len);
void ScrollUp(int numLines, uint8_t clearColor);

void CursorDisable();
void CursorEnable(uint8_t scanlineStart, uint8_t scanlineEnd);
void CursorMoveto(int x, int y);


#define Print(s)  Printn((s), -1)
#define Println(s)  Printnln((s), -1)

#endif