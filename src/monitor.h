#pragma once
#include "types.h"

// Framebuffer size
#define FB_WIDTH 80
#define FB_HEIGHT 25
#define TAB_SIZE 4

typedef enum
{
	MONCOLOR_BLACK    = 0,
	MONCOLOR_BLUE     = 1,
	MONCOLOR_GREEN    = 2,
	MONCOLOR_CYAN     = 3,
	MONCOLOR_RED      = 4,
	MONCOLOR_MAGENTA  = 5,
	MONCOLOR_BROWN    = 6,
	MONCOLOR_LGREY    = 7,
	MONCOLOR_DGREY    = 8,
	MONCOLOR_LBLUE    = 9,
	MONCOLOR_LGREEN   = 10,
	MONCOLOR_LCYAN    = 11,
	MONCOLOR_LRED     = 12,
	MONCOLOR_LMAGENTA = 13,
	MONCOLOR_LBROWN   = 14,
	MONCOLOR_WHITE    = 15
} MONCOLOR;

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
void monitor_MONCOLOR_set(MONCOLOR fg, MONCOLOR bg);

// Reset the forground and background colour
void monitor_MONCOLOR_reset();
