/***************************************************************
Copyright (c) 2013 Payton Byrd
All rights reserved.

Redistribution and use in source and binary forms, with or
without modification, are permitted provided that the following
conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the following
  disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the following
  disclaimer in the documentation and/or other materials
  provided with the distribution.

* Neither the name of Payton Byrd nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior written
  permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***************************************************************/

#define __fastcall __fastcall__
#include <conio.h>
#include <device.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#ifdef __C128__
#include <c128.h>
#endif

#include "ConfigurationManager.h"
#include "Configuration.h"
#include "constants.h"
#include "globalInput.h"
#include "globals.h"
#include "screen.h"

static unsigned char defaultDisk[2];

static void refreshScreen(void)
{
	setupScreen();

	writeFunctionKeys();
	writeMenu();
}

void main(void)
{
#ifdef __C128__
	videomode(VIDEOMODE_80COL);
	fast();
#endif

	initialize();
	defaultDisk[0] = atoi(defaultLeftDisk);
	defaultDisk[1] = atoi(defaultRightDisk);

	refreshScreen();

	while(true)
	{
		readKeyboard();
	}
}

static void __fastcall spin(unsigned char row) {
	unsigned char oldDisk = defaultDisk[row];

	revers(true);
	for (;;)
	{
		gotoxy(
#if size_x > 22
			32
#else
			18
#endif
			, row + 5);
		cprintf("%-3u", defaultDisk[row]);
		switch (getKey())
		{
		case CH_CURS_DOWN:
		case CH_CURS_LEFT:
			if (--defaultDisk[row] == 255) {
				defaultDisk[row] = 254;
			}
			break;
		case CH_CURS_RIGHT:
		case CH_CURS_UP:
			if (++defaultDisk[row] == 255) {
				defaultDisk[row] = 0;
			}
			break;
		case CH_STOP:
			defaultDisk[row] = oldDisk;
		case CH_ENTER:
			revers(false);
			refreshScreen();
			return;
		}
	}
}

static void readKeyboard(void)
{
	char key = cgetc();

	switch(key)
	{
	case CH_F1:
		help();
		break;
	case CH_F2:
		quit();
		break;
	case CH_F3:
		save();
		break;
	case CH_F4:
		keys();
		refreshScreen();
		break;
#ifdef COLOR_RED
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		changeColor(key);
		refreshScreen();
		break;
#endif
	case 'l':
		if(++defaultLeftDrive > 16) defaultLeftDrive = 8;
		refreshScreen();
		break;
	case 'r':
		if(++defaultRightDrive > 16) defaultRightDrive = 8;
		refreshScreen();
		break;
	case 'p':
		spin(0);
		break;
	case 'd':
		spin(1);
		break;
	}
}

#ifdef COLOR_RED
static void __fastcall changeColor(char key)
{
	switch(key)
	{
	case '1':
		pickColor(&color_background);
		break;
	case '2':
		pickColor(&color_border);
		break;
	case '3':
		pickColor(&color_selector);
		break;
	case '4':
		pickColor(&color_text_borders);
		break;
	case '5':
		pickColor(&color_text_menus);
		break;
	case '6':
		pickColor(&color_text_files);
		break;
	case '7':
		pickColor(&color_text_status);
		break;
	case '8':
		pickColor(&color_text_highlight);
		break;
	case '9':
		pickColor(&color_text_other);
		break;
	}
}

static void __fastcall displayColor(unsigned char color)
{
	gotox(14);
	cputc(':');
	(void)textcolor(color);
	revers(true);
	cputc(' ');
	revers(false);
	(void)textcolor(color_text_menus);
}
#endif

static void help(void)
{
	static const char *const help_message[] =
	{
		{ "Please visit:" },
#if size_x < 40
		{ "cbmcommand.codeplex" },
		{ " .com/documentation" }
#else
		{ "http://cbmcommand.codeplex.com/" },
		{ "        /documentation" }
#endif
	};

	//saveScreen();

	drawDialog(help_message, A_SIZE(help_message), "Help", OK);

	//retrieveScreen();
}

static void quit(void)
{
	static const char *const quit_message[] =
	{
		{ "Are you sure" },
		{ "you want to quit?" }
	};

	//saveScreen();

	if(writeYesNo("Confirm", quit_message, A_SIZE(quit_message)))
	{
		clrscr();
		writeStatusBar("Goodbye!");
		(void)bordercolor(outsideFrame);
		(void)bgcolor(outsideScreen);
		(void)textcolor(outsideText);
		exit(EXIT_SUCCESS);
	}

	//retrieveScreen();
}

static void writeMenu(void)
{
	writePanel(true, false, color_text_borders,
		0, 1, size_y - 4, size_x - 1,
#if size_x < 40
		"Config. Manager",
#else
		"CBM-Command Configuration Manager",
#endif
		NULL, NULL);

	(void)textcolor(color_text_menus);
	cputsxy(1, 4, "Drives:");
#if size_x > 22
	gotox(2); cprintf("\nL;P - Default  Left Drive:%3u;%u", defaultLeftDrive, defaultDisk[0]);
	gotox(2); cprintf("\nR;D - Default Right Drive:%3u;%u", defaultRightDrive, defaultDisk[1]);
#else
	gotox(1); cprintf("\nL;P -  Lft Dr:%2u;%u", defaultLeftDrive, defaultDisk[0]);
	gotox(1); cprintf("\nR;D - Rght Dr:%2u;%u", defaultRightDrive, defaultDisk[1]);
#endif

#ifdef COLOR_RED
	cputsxy(1, 8, "Colors:");
	gotox(2); cputs("\n1 Background"); displayColor(color_background);
	gotox(2); cputs("\n2 Border"); displayColor(color_border);
	gotox(2); cputs("\n3 Selector"); displayColor(color_selector);
	gotox(2); cputs("\n4 Box Border"); displayColor(color_text_borders);
	gotox(2); cputs("\n5 Menues"); displayColor(color_text_menus);
	gotox(2); cputs("\n6 Filenames"); displayColor(color_text_files);
	gotox(2); cputs("\n7 Status Bar"); displayColor(color_text_status);
	gotox(2); cputs("\n8 Highlight"); displayColor(color_text_highlight);
	gotox(2); cputs("\n9 Other"); displayColor(color_text_other);
#endif
}

static void writeFunctionKeys(void)
{
#if size_x > 22u
#define bottom size_y - 1
#else
#define bottom size_y - 2
#endif

	(void)textcolor(color_text_other);
	//cclearxy(0, bottom, size_x);
#if size_x > 22u
	cputsxy(2, bottom, "HELP    QUIT    SAVE    KEYS");
#else
	cputsxy(2, bottom,		"HELP    SAVE");
	cputsxy(2, bottom + 1u, "QUIT    KEYS");
#endif

	revers(true);
#ifdef __PET__
	cputcxy(1, bottom, '1');
	gotox(9); cputc('2');
	gotox(17); cputc('3');
	gotox(25); cputc('4');
#else
	cputsxy(0, bottom, "F1");
#if size_y > 23u
	gotox(8); cputs("F2");
	gotox(16); cputs("F3");
	gotox(24); cputs("F4");
#else
	gotox(8); cputs("F3");
	cputsxy(0, bottom + 1u, "F2");
	gotox(8); cputs("F4");
#endif
#endif
	revers(false);
}

static void save(void)
{
#ifdef __CBM__
	unsigned char d;
	signed char r;

	cbm_open(15,getcurrentdevice(),15,"");	// open the status channel
	d = cbm_open(1, getcurrentdevice(), 3, "@0:cbmcmd22cfg."
#ifdef __C64__
		"c64"
#endif
#ifdef __C128__
		"c128"
#endif
#ifdef __VIC20__
		"vc20"
#endif
#ifdef __PET__
		"pet"
#endif
#ifdef __PLUS4__
		"plus4"
#endif
		",s,w");
	if(d == 0)			// XXX: success means only that the drive exists
	{
		sprintf(defaultLeftDisk, "%u", defaultDisk[0]);
		sprintf(defaultRightDisk,"%u", defaultDisk[1]);

		cbm_write(1, &defaultLeftDrive, 1);
		cbm_write(1,  defaultLeftDisk, sizeof defaultLeftDisk);
		cbm_write(1, &defaultRightDrive, 1);
		cbm_write(1,  defaultRightDisk, sizeof defaultRightDisk);

// The CBM/Pet clan (except the CBM510) doesn't support color.
// Don't save colors if only black and white are defined.
#ifdef COLOR_RED
		cbm_write(1, &color_background, 1);
		cbm_write(1, &color_border, 1);
		cbm_write(1, &color_selector, 1);
		cbm_write(1, &color_text_borders, 1);
		cbm_write(1, &color_text_menus, 1);
		cbm_write(1, &color_text_files, 1);
		cbm_write(1, &color_text_status, 1);
		cbm_write(1, &color_text_other, 1);
		cbm_write(1, &color_text_highlight, 1);
#endif

		cbm_write(1u, keyMap, sizeof keyMap);

		r = cbm_read(15, buffer, (sizeof buffer) - 1);
		buffer[r < 0 ? 0 : r] = '\0';
		if (buffer[0] != '0')
		{
			writeStatusBar(buffer);
			waitForEnterEsc();
		}
		else
		{
			writeStatusBar("Wrote configuration.");
		}
	}
	else
	{
		waitForEnterEscf("Error %u openning cfg", d);
		r = cbm_read(15,buffer,(sizeof buffer) - 1);
		buffer[r < 0 ? 0 : r] = '\0';
		writeStatusBar(buffer);
		waitForEnterEsc();
	}

	cbm_close(1);
	cbm_close(15);
#endif
}

#ifdef COLOR_RED
static void __fastcall pickColor(unsigned char *color)
{
	unsigned char k, j, i;
#if defined(__VIC20__)
#define NUM_COLORS 8
#elif defined(__C64__) || defined(__C128__) || defined(__CBM510__)
#define NUM_COLORS 16
#elif defined(__PLUS4__)
#define NUM_COLORS 128
#endif

	writePanel(true, false, color_text_borders, 0, 0, size_y-1, size_x-1,
		"Pick a Color", "Cancel", "OK");

	revers(true);
	for(i=0; i<NUM_COLORS; i += 16)
	{
		for(j=0; j<16 && (unsigned char)(i + j) < NUM_COLORS; ++j)
		{
			textcolor(j + i);
			cputcxy(2+j*2, 2+i/(16/2), ' ');
		}
	}
	revers(false);
	textcolor(color_selector);

	i = *color / 16;
	j = *color % 16;

	for (;;)
	{
		cputcxy(2+j*2, 3+i*2, '^');
		k=getKey();
		cputcxy(2+j*2, 3+i*2, ' ');
		switch(k)
		{
		case CH_CURS_DOWN:
			if((i + 1) * 16 < NUM_COLORS) ++i;
			break;

		case CH_CURS_UP:
			if(i > 0) --i;
			break;

		case CH_CURS_LEFT:
			if(j > 0) --j;
			break;

		case CH_CURS_RIGHT:
			if(j < 15 && j +1 < NUM_COLORS) ++j;
			break;

		case CH_ENTER:
			*color = j + i*16;
		case CH_STOP:
			return;
		}
	}
}
#endif

static void __fastcall drawCursor(
	const unsigned char x,
	const unsigned char y,
	const bool draw)
{
	(void)textcolor(color_text_highlight);
	cputcxy(x, y, draw ? '>' : ' ');
}

static void __fastcall setHotKey(const unsigned char index)
{
	char key, newKey;
	const unsigned char
		x = getCenterX(15u),
		y = getCenterY(6u);

	writeStatusBarf("index:%3u", index);

	writePanel(
		true, false,
		color_text_borders,
		x, y, 6u, 15u,
		"Set Key", NULL, "OK");

	(void)textcolor(color_text_other);
	newKey = keyMap[index];
	gotoxy(x+1, y+2);
	cprintf("Current: $%02X %c", newKey, newKey);

	for(;;)
	{
		gotoxy(x+1, y+3);
		cprintf("New    : $%02X %c", newKey, newKey);

		key = getKey();
		switch(key)
		{
		case CH_ENTER:
			keyMap[index] = newKey;
			// Fall through.
		case CH_STOP:
			retrieveScreen();
			return;

		// Ignore keys that mustn't be reconfigured.
		case CH_F1:
		case CH_F2:
		case CH_F3:
		case CH_F4:
		case CH_F5:
		case CH_F6:
		case CH_F7:
		case CH_F8:
		case CH_CURS_DOWN:
		case CH_CURS_LEFT:
		case CH_CURS_RIGHT:
		case CH_CURS_UP:
			break;

		default:
			newKey = key;
			break;
		}
	}
}

#define KEYS_HELP "Select Function"
static void keys(void)
{
	unsigned char index = 0u, x = 1u, y = 2u;

	writePanel(true, false, color_text_borders, 0, 0, size_y-1, size_x-1,
		"Change Keys", NULL, NULL);

	(void)textcolor(color_text_files);
// These names must be column-sorted in the same order as the enumeration.
// Left column:
	cputsxy(2u, 2u, "Copy");
	cputsxy(2u, 3u, "Mk Image");
	cputsxy(2u, 4u, "Wr Image");
	cputsxy(2u, 5u, "Delete");
	cputsxy(2u, 6u, "Dr Left");
	cputsxy(2u, 7u, "Dr Right");
	cputsxy(2u, 8u, "Dr Curr");
	cputsxy(2u, 9u, "Dr Cmd");
	cputsxy(2u, 10u, "Help");
	cputsxy(2u, 11u, "Mk Dir");
	cputsxy(2u, 12u, "En Dir");
	cputsxy(2u, 13u, "Lv Dir");
	cputsxy(2u, 14u, "Pg Up");
	cputsxy(2u, 15u, "Pg Down");
	cputsxy(2u, 16u, "Quit");

// Right column:
	cputsxy(12u, 2u, "Rename");
	cputsxy(12u, 3u, "Read Lft");
	cputsxy(12u, 4u, "Read Rgt");
	cputsxy(12u, 5u, "Refresh");
	cputsxy(12u, 6u, "Select");
	cputsxy(12u, 7u, "S. All");
	cputsxy(12u, 8u, "S. None");
	cputsxy(12u, 9u, "To Top");
	cputsxy(12u, 10u, "To Bottom");
	cputsxy(12u, 11u, "Exc/Read");
	cputsxy(12u, 12u, "Copy Disk");
	cputsxy(12u, 13u, "Batch Img");
	cputsxy(12u, 14u, "Set Path");
	cputsxy(12u, 15u, "Orientation");

	(void)textcolor(color_text_highlight);
	cputsxy(getCenterX((unsigned char)(sizeof KEYS_HELP - 1)), 17u, KEYS_HELP);

	for(;;)
	{
		drawCursor(x, y, true);

		switch (getKey())
		{
		case CH_ENTER:
			//saveScreen();
			//setHotKey((x == 1u ? 0u : 14u) + y - 2u);
			setHotKey(y + (x == 1u ? 0u - 2u : 15 - 2));	// XXX: examine asm output
			//retrieveScreen();
			break;

		case CH_STOP:
			return;

		case CH_CURS_DOWN:
			if(y < (x == 1u ? 16u : 15u))
			{
				drawCursor(x, y, false);
				++y;
			}
			break;

		case CH_CURS_UP:
			if(y > 2u)
			{
				drawCursor(x, y, false);
				--y;
			}
			break;

		case CH_CURS_RIGHT:
			if(x != 11u)
			{
				drawCursor(x, y, false);
				if(y > 15u) y = 15u;
				x = 11u;
			}
			break;

		case CH_CURS_LEFT:
			if(x != 1u)
			{
				drawCursor(x, y, false);
				x = 1u;
			}
			break;
		}
	}
}
