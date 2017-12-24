#include "monitor.h"
#include "system.h"

static uint16 *fb = (uint16 *) 0x000B8000;
static uint8 cursor_x = 0;
static uint8 cursor_y = 0;
static MONCOLOR color_fg = MONCOLOR_LGREY;
static MONCOLOR color_bg = MONCOLOR_BLACK;

// Blank character
static const uint16 BLANK = 0x20 /*space*/ | (((MONCOLOR_BLACK << 4) | (MONCOLOR_WHITE & 0x0F)) << 8);

// Update the hardware framebuffer cursor
static void update_cursor(void)
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
static void scroll(void)
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

void monitor_put(char c)
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
		uint8 color = (color_bg << 4) | (color_fg & 0x0F);
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

void monitor_write(char *str)
{
	// TODO: handle str longer than 32-bits
	//       and/or a non-null terminated string
	uint32 i = 0;
	while(str[i])
	{
		monitor_put(str[i++]);
	}
}

void monitor_writeline(char *str)
{
	monitor_write(str);
	monitor_write("\r\n");
}

void monitor_write_hex(uint64 i)
{
	// Display 8 chars if only 32-bit value or less
	uint8 len = 7;
	if (i > 0xFFFFFFFF)
	{
		len = 15;
	}

	monitor_write("0x");
	for (int8 j = len; j >= 0; j--)
	{
		uint32 digit = (i >> j*4) & 0xF;
		if (digit < 0xA)
		{
			monitor_put(digit + '0');
		}
		else
		{
			monitor_put(digit - 0xA + 'a');
		}

		// Add separator between DWORDs
		if (j == 8)
		{
			monitor_put('_');
		}
	}
}

// TODO: implement 64-bit version (need to link to libgcc)
void monitor_write_dec(uint32 i)
{
	if (i == 0)
	{
		monitor_put('0');
		return;
	}

	int32 acc = i;
	int32 j;
	char c[64];
	for (j = 0; acc > 0; j++)
	{
		c[j] = '0' + acc % 10;
		acc /= 10;
	}
	c[j] = 0;

	int32 k = 0;
	char c2[64];
	c2[j--] = 0;
	while (j >= 0)
	{
		c2[j--] = c[k++];
	}
	monitor_write(c2);
}

void monitor_clear(void)
{
	// Fill the entire screen with the 'blank' character
	for (uint64 i = 0; i < FB_HEIGHT * FB_WIDTH; i++)
	{
		fb[i] = BLANK;
	}

	// Reset cursor to (0, 0)
	cursor_x = 0;
	cursor_y = 0;
	update_cursor();
}

void monitor_color_set(MONCOLOR fg, MONCOLOR bg)
{
	color_fg = fg;
	color_bg = bg;
}

void monitor_color_reset(void)
{
	monitor_color_set(MONCOLOR_LGREY, MONCOLOR_BLACK);
}
