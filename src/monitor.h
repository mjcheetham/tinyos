#pragma once
#include "types.h"

// Framebuffer size
#define FB_WIDTH 80
#define FB_HEIGHT 25
#define TAB_SIZE 4

typedef enum
{
	FBCOLOR_BLACK    = 0,
	FBCOLOR_BLUE     = 1,
	FBCOLOR_GREEN    = 2,
	FBCOLOR_CYAN     = 3,
	FBCOLOR_RED      = 4,
	FBCOLOR_MAGENTA  = 5,
	FBCOLOR_BROWN    = 6,
	FBCOLOR_LGREY    = 7,
	FBCOLOR_DGREY    = 8,
	FBCOLOR_LBLUE    = 9,
	FBCOLOR_LGREEN   = 10,
	FBCOLOR_LCYAN    = 11,
	FBCOLOR_LRED     = 12,
	FBCOLOR_LMAGENTA = 13,
	FBCOLOR_LBROWN   = 14,
	FBCOLOR_WHITE    = 15
} FBCOLOR;

// Write a single character to the monitor
void monitor_put(char c);

// Write a null-terminated string to the monitor
void monitor_write(char *str);

// Write a null-terminated string to the monitor followed by a new line
void monitor_writeline(char *str);

// Write an integer as a hexadecimal number to the monitor
void monitor_write_hex(uint64 i);

// Clear the monitor
void monitor_clear();

// Set the foreground and background colours
void monitor_color_set(FBCOLOR fg, FBCOLOR bg);

// Reset the forground and background colour
void monitor_color_reset();
