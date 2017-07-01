#include "monitor.h"
#include "system.h"

uint16 *fb = (uint16 *) 0x000B8000;
uint8 cursor_x = 0;
uint8 cursor_y = 0;

// Blank character
static const uint16 BLANK = 0x20 /*space*/ | (((FBCOLOR_BLACK << 4) | (FBCOLOR_WHITE & 0x0F)) << 8);

// Update the hardware framebuffer cursor
static void update_cursor()
{
	uint16 pos = cursor_y * FB_WIDTH + cursor_x;

	// Write the high bits
	outb(0x3D4, 14);
	outb(0x3D5, ((pos >> 8) & 0x00FF));

	// Write the low bits
	outb(0x3D4, 15);
	outb(0x3D5, pos & 0x00FF);
}

// Scroll the screen up by one line
static void scroll()
{
	const uint16 last_line_pos = (FB_HEIGHT - 1) * FB_WIDTH;

	// Copy line_i to line_(i-1)
	for (uint16 i = 0; i < last_line_pos; i++)
	{
		fb[i] = fb[i + FB_WIDTH];
	}

	// Clear last line
	for (uint16 i = last_line_pos; i < FB_HEIGHT * FB_WIDTH; i++)
	{
		fb[i] = BLANK;
	}

	// Set cursor to last line
	cursor_y = FB_HEIGHT - 1;
}

void monitor_put(char c, FBCOLOR fg, FBCOLOR bg)
{
	// Backspace => move back one
	if (c == 0x08 && cursor_x > 0)
	{
		cursor_x--;
	}
	// Tab => move forward TAB_SIZE
	else if (c == '\t')
	{
		cursor_x = (cursor_x + TAB_SIZE) & ~(TAB_SIZE - 1);
	}
	// Carriage return => move cursor to start of line
	else if (c == '\r')
	{
		cursor_x = 0;
	}
	// Line feed => move to next row
	else if (c == '\n')
	{
		cursor_y++;
	}
	// All other printable characters
	else if (c >= ' ')
	{
		// Calculate the position and colour bits
		uint8 color = (bg << 4) | (fg & 0x0F);
		uint16 pos = cursor_y * FB_WIDTH + cursor_x;

		// Write character to framebuffer
		fb[pos] = c | (color << 8);

		// Advance cursor
		cursor_x++;
	}

	// Wrap to a new line if we hit the end of the row
	if (cursor_x >= FB_WIDTH)
	{
		cursor_x = 0;
		cursor_y++;
	}

	// Scroll the screen if needed
	if (cursor_y >= 25)
	{
		scroll();
	}

	// Update the cursor location
	update_cursor();
}

void monitor_write(char *str, FBCOLOR fg, FBCOLOR bg)
{
	// TODO: handle str longer than 32-bits
	//       and/or a non-null terminated string
	uint32 i = 0;
	while(str[i])
	{
		monitor_put(str[i++], fg, bg);
	}
}

void monitor_clear()
{
	// Fill the entire screen with the 'blank' character
	for (int i = 0; i < FB_HEIGHT * FB_WIDTH; i++)
	{
		fb[i] = BLANK;
	}

	// Reset cursor to (0, 0)
	cursor_x = 0;
	cursor_y = 0;
	update_cursor();
}
