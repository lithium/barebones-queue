#ifndef BAREBONES_VGA_H
#define BAREBONES_VGA_H

#include <inttypes.h>


#define VGA_NUM_COLS 80
#define VGA_NUM_ROWS 25


int Printxy(int x, int y, char *str, uint8_t colors);
int Print(char *str, uint8_t colors);
int Println(char *str, uint8_t colors);
void ScrollUp(int numLines, uint8_t clearColor);

void CursorDisable();
void CursorEnable(uint8_t scanlineStart, uint8_t scanlineEnd);
void CursorMoveto(int x, int y);



#endif