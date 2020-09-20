/***************************************************************
Copyright (c) 2013, Payton Byrd
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

#ifdef __CBM__
#include <cbm.h>
#endif
#include <conio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <peekpoke.h>

#include "screen.h"
#include "Configuration.h"
#include "constants.h"
#include "globalInput.h"
#include "globals.h"
#include "PlatformSpecific.h"
#include "input.h"

#if defined(__PET__)
#define screenMemoryStart (void *)0x8000
static unsigned char SCREEN_BUFFER[size_x * size_y];
//static unsigned char COLOR_BUFFER[1000];
#endif

#if defined(__VIC20__)
#define screenMemoryStart (void *)0x1000
#define  colorMemoryStart (void *)0x9400
//static unsigned char SCREEN_BUFFER[size_x * size_y];
//static unsigned char  COLOR_BUFFER[size_x * size_y];
#endif

#if defined(__PLUS4__)
#define screenMemoryStart (void *)3072
#define  colorMemoryStart COLOR_RAM
static unsigned char SCREEN_BUFFER[size_x * size_y];
static unsigned char  COLOR_BUFFER[size_x * size_y];
#endif

bool isDoubleBuffered = false;
#ifdef __C64__
static unsigned char d018;
static unsigned char dd00;
//static unsigned char dd02;
#endif

#if size_x == 40
enum orientations screenOrientation =
	ORIENT_HORIZ;
#endif

bool copiedCharacters = false;
void beginDoubleBuffer(void)
{
#ifdef __C64__
	static bool double_buffer_initialized = false;

	if(!double_buffer_initialized)
	{
		--CIA1.cra;						// Turn off interrupt timer
		POKE(0x01, PEEK(0x01) & ~0x04);	// Switch in character ROM

		// Copy the lower-/upper-case font into VIC-II RAM bank 3.
		if(!copiedCharacters)
		{
			memcpy((void*)0xF800, (void*)0xD800, 0x0800);
			copiedCharacters = true;
		}

		POKE(0x01, PEEK(0x01) | 0x04);	// Switch out character ROM
		++CIA1.cra;						// Turn on interrupt timer

		double_buffer_initialized = true;

		// Save the original VIC-II banking values.
		d018 = PEEK(0xD018);
		dd00 = PEEK(0xDD00);
		//dd02 = PEEK(0xDD02);

		//POKE(0xDD02, PEEK(0xDD02) | 0x03);
	}

	saveScreen();

	POKE(0xDD00, (PEEK(0xDD00) & 0xFC) | 0x00);	// switch to VIC-II bank 3
	POKE(0xD018, 0xC0 | 0x0E);					// show the saved screen

	isDoubleBuffered = true;
#endif
}

void endDoubleBuffer(void)
{
#ifdef __C64__
	//POKE(0xDD02, dd02);
	POKE(0xDD00, dd00);					// switch back to VIC-II bank 0
	POKE(0xD018, d018);					// show the standard screen area

	isDoubleBuffered = false;
#endif
}

// Prepares the screen
void setupScreen(void)
{
	(void)bordercolor(color_border);
	(void)bgcolor(color_background);
	(void)textcolor(color_text_other);

	clrscr();
}

#ifndef __C64__
void saveScreen(void)
{
#ifdef __C128__
	copyVdcScreen(0x00, 0x10);
#elif defined(__PET__) /*|| defined(__VIC20__)*/ || defined(__PLUS4__)
	memcpy(SCREEN_BUFFER, screenMemoryStart, sizeof(SCREEN_BUFFER));

// Save colors if they are defined.
#ifdef COLOR_RED
	memcpy( COLOR_BUFFER,  colorMemoryStart, sizeof (COLOR_BUFFER));
#endif
#elif defined(__VIC20__)
	memcpy((void *) 0xB000, screenMemoryStart, size_x * size_y);
	memcpy((void *)(0xB000 + size_x * size_y), colorMemoryStart, size_x * size_y);
#endif
}

void retrieveScreen(void)
{
#ifdef __C128__
	copyVdcScreen(0x10, 0x00);
#elif defined(__PET__) /*|| defined(__VIC20__)*/ || defined(__PLUS4__)
	memcpy(screenMemoryStart, SCREEN_BUFFER, sizeof(SCREEN_BUFFER));
#ifdef COLOR_RED
	memcpy( colorMemoryStart,  COLOR_BUFFER, sizeof (COLOR_BUFFER));
#endif
#elif defined(__VIC20__)
	memcpy(screenMemoryStart, (void *) 0xB000, size_x * size_y);
	memcpy( colorMemoryStart, (void *)(0xB000+ size_x * size_y), size_x * size_y);
#endif
}
#endif

void __fastcall writeStatusBar(
	const char message[])
{
	unsigned char oldX = wherex(), oldY = wherey();

	(void)textcolor(color_text_status);
	revers(true);

	cclearxy(0, 0, size_x);
	cputsxy(0, 0, message);

	revers(false);
	gotoxy(oldX, oldY);			// XXX: is this needed?
}

// The "color" parameter isn't used by the Pet library.
//#pragma warn(unused-param, push, off)

void __fastcall drawBox(
	unsigned char x,
	unsigned char y,
	unsigned char w,
	unsigned char h,
	unsigned char color __attribute__((unused)),
	bool reverse)
{
	unsigned char i = y + h;

	(void)textcolor(color);
	revers(reverse);

	// draw body
	while (--i > y)
	{
		cclearxy(x, i, w);
	}

	// draw the sides
	cvlinexy(x, y, h);
	cvlinexy(x+w, y, h);

	// draw top line
	chlinexy(x, y, w);
	cputcxy(x, y, CH_ULCORNER);
	cputcxy(x+w, y, CH_URCORNER);

	// draw bottom line
	chlinexy(x, y+h, w);
	cputcxy(x, y+h, CH_LLCORNER);
	cputcxy(x+w, y+h, CH_LRCORNER);
}
//#pragma warn(unused-param, pop)

unsigned char __fastcall getCenterX(unsigned char w)
{
	return (size_x - w) / 2u /*- 1u*/;
}

unsigned char __fastcall getCenterY(unsigned char h)
{
	return (size_y - h) / 2u /*- 1u*/;
}

void __fastcall writePanel(
	bool drawBorder,
	bool reverse,
	unsigned char color,
	unsigned char x, unsigned char y,
	unsigned char h, unsigned char w,
	const char *title,
	const char *cancel,
	const char *ok)
{
	unsigned char i = y + h, okLeft = x + w - 2;

	saveScreen();

	if(drawBorder)
	{
		drawBox(x, y, w, h, color, reverse);
	}
	else
	{
		(void)textcolor(color);
		revers(reverse);

		while (--i >= y)
		{
			cclearxy(x,i,w);
		}
	}

	if(title != NULL)
	{
#ifdef __C128__
		// Works around a bug in CC65's CONIO
		// library on the VDC.
		textcolor(color);
		revers(reverse);
#endif
		gotoxy(x+1, y); cprintf("[%s]", title);
	}

	revers(false);

	if(ok != NULL)
	{
#ifdef __C128__
		// Works around a bug in CC65's CONIO
		// library on the VDC.
		textcolor(color);
		revers(reverse);
#endif
		//okLeft -= sprintf(buffer, "[%s]", ok);
		//cputsxy(okLeft, y + h - 1, buffer);
		okLeft -= strlen(ok) + 2;
		gotoxy(okLeft, y + h - 1); cprintf("[%s]", ok);
	}

	if(cancel != NULL)
	{
#ifdef __C128__
		// Works around a bug in CC65's CONIO
		// library on the VDC.
		textcolor(color);
		revers(reverse);
#endif
		//cputsxy(okLeft - sprintf(buffer, "[%s]", cancel) - 2,
		//	y + h - 1, buffer);
		gotoxy(okLeft - strlen(cancel) - 4, y + h - 1);
		cprintf("[%s]", cancel);
	}
}

//void notImplemented(void)
//{
//	saveScreen();
//	writeStatusBar("Not implemented");
//	waitForEnterEsc();
//	retrieveScreen();
//}

enum results __fastcall drawDialog(
	const char* const message[],
	unsigned char lineCount,
	const char* title,
	enum buttons button)
{
	unsigned char w = 20, h = lineCount + 5;
	unsigned char y = getCenterY(h);
	const char *okButton = NULL, *cancelButton = NULL;
	unsigned char x, i, key;

	for(i=0; i<lineCount; ++i)
	{
		if(strlen(message[i]) + 3 > w) w = strlen(message[i]) + 3;
	}

	x = getCenterX(w);

	if(button & OK)
	{
		okButton = "OK";
	}
	if(button & YES)
	{
		okButton = "Yes";
	}

	if(button & CANCEL)
	{
		cancelButton = "Cancel";
	}
	if(button & NO)
	{
		cancelButton = "No";
	}

	writePanel(
		true, false, color_text_borders,
		x, y, h, w,
		title, cancelButton, okButton);

	for(i=0; i<lineCount; ++i)
	{
		(void)textcolor(color_text_other);
		cputsxy(x+2, i+2+y,message[i]);
	}

	while(true)
	{
		key = getKey();

		switch (key)
		{
		case CH_ENTER:
		case CH_STOP:
			break;
		case 'o':
			if (button & OK)
				break;
			continue;
		case 'y':
			if (button & YES)
				break;
			continue;
		case 'c':
			if (button & CANCEL)
				break;
			continue;
		case 'n':
			if (button & NO)
				break;
		default:
			continue;
		}
		break;
	}

	retrieveScreen();

	switch(key)
	{
	case CH_STOP:
	case 'n':
	case 'c':
		if(button & NO) return NO_RESULT;
		//if(button & CANCEL) return CANCEL_RESULT;
		break;

	case CH_ENTER:
	case 'y':
	case 'o':
		if(button & YES) return YES_RESULT;
		if(button & OK) return OK_RESULT;
		break;
	}

	return CANCEL_RESULT;
}

enum results __fastcall drawInputDialog(
	unsigned char lineCount,
	unsigned char length,
	const char *const message[],
	const char *title,
	char *resultText)
{
	unsigned char h = lineCount + 6, w = length + 3;
	unsigned char x = getCenterX(w) + 2, y = getCenterY(h);
	unsigned char count = strlen(resultText), i, key;

	writePanel(
		true, false, color_text_borders,
		x - 2, y, h, w,
		title,
		"Cancel",
		"Done");

	(void)textcolor(color_text_other);
	for(i=0; i<lineCount; ++i)
	{
		cputsxy(x, i+2+y, message[i]);
	}
	++i;						// i = lineCount + 1;
	// XXX: factor out i+2+y

	revers(true);
	//(void)textcolor(color_text_other);
	cclearxy(x, i+2+y, length + 1);

	cputsxy(x, i+2+y, resultText);
	do
	{
		cputcxy(count+x, i+2+y, '<');
		key = getKey();

		if (count < length &&
			(
				(key >= ' ' && key <= '^'+1) ||
				(key >= 'A' && key <= 'Z')
			)
		   )
		{
			cputcxy(count+x, i+2+y, resultText[count] = key);
			++count;
		}
		else if (key ==
#ifdef CH_DEL
			CH_DEL
#else
			'\x7F'				// CH_RUBOUT
#endif
			&& count > 0)
		{
			cputcxy(count+x, i+2+y, ' ');
			--count;
		}
		resultText[count] = '\0';
	}
	while (key != CH_ENTER && key != CH_STOP);

	revers(false);
	retrieveScreen();

	if (key == CH_ENTER && count != 0)
	{
		return OK_RESULT;
	}
	return CANCEL_RESULT;
}

bool __fastcall writeYesNo(
	const char *title,
	const char *const message[],
	unsigned char lineCount)
{
	//return drawDialog(message, lineCount, title, YES | NO) == YES_RESULT;
	// The below code makes smaller programs (because it will be optimized,
	// while the above line would link to a boolean-test conversion module).
	return ((unsigned char)drawDialog(message, lineCount, title, YES | NO)
		== YES_RESULT) ? true : false;
}

void __fastcall vwriteStatusBarf(const char format[], va_list ap)
{
	char buffer[81];

	vsprintf(buffer, format, ap);
	writeStatusBar(buffer);
}

void writeStatusBarf(const char format[], ...)
{
	va_list ap;

	va_start(ap,  format);
	vwriteStatusBarf(format, ap);
	va_end(ap);
}

void __fastcall drawProgressBar(
	const char* message,
	unsigned int currentValue,
	unsigned int maxValue)
{
	unsigned char result;
	//unsigned char solid, remainder, i;
	div_t bar;
	unsigned char x = getCenterX(20 - 2), y = getCenterY(1);
#ifdef __CBM__
	static const char barSegments[] =
		{ ' ', 165, 165, 180, 181, 161, 182, 182, 170, 167 };
#endif

	(void)textcolor(color_text_files);
	cputsxy(x, y, message);
	//++y;

	if(maxValue < currentValue) maxValue = currentValue;

	// result's range is from 0.00 up to 100.00
	result = (unsigned char)((((unsigned long)currentValue * 10000uL)
		/ (unsigned long)maxValue) / 100uL);

	//solid = result / sizeof barSegments;
	//remainder = result % sizeof barSegments;
	bar = div(result, sizeof barSegments);

	(void)textcolor(color_text_highlight);
	cputcxy(x, y + 1, 179);
	revers(true);
	////for(i=0; i<solid; ++i)
	////{
	////	cputcxy(x + i + 1, y, ' ');
	////}
	//cclearxy(x, y, solid);
	//if(remainder < 6) { revers(false); }
	//cputcxy(x + solid, y, barSegments[remainder]);
	cclear(bar.quot);
	if(bar.rem < 6) { revers(false); }
	cputc(barSegments[bar.rem]);
	revers(false);

	gotox(x + 11); cputc(171);
	gotox(x + 14); cprintf("%3u%%", (unsigned int)result);
}

#if size_x == 40
void swapOrientation(void)
{
	unsigned char y;

	for(y = 1; y < 24; ++y)
	{
		cclearxy(0, y, 40);
	}

	screenOrientation ^= ORIENT_VERT ^ ORIENT_HORIZ;
}
#endif
