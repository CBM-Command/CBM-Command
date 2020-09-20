/***************************************************************
Copyright (c) 2010, Payton Byrd
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

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

#include "Configuration.h"
#include "constants.h"
#include "globals.h"
#include "input.h"
#include "menus.h"
#include "screen.h"

//unsigned char currentMenuX;
//unsigned char currentMenuLine;

//void __fastcall writeMenu(enum menus menu)
//{
//	currentMenuLine = 0;
//	switch (menu)
//	{
//	  case left:
//		drawDriveMenu(LEFT_MENU_X * size_x / 100);
//		handleDriveMenu(left);
//		break;
//	  case right:
//		drawDriveMenu(size_x * RIGHT_MENU_X / 100);
//		handleDriveMenu(right);
//		break;
//	  case file:
//		drawFileMenu(size_x * FILE_MENU_X / 100);
//		handleFileMenu();
//		break;
//	  case command:
//		drawCommandMenu(size_x * COMMAND_MENU_X / 100);
//		handleCommandMenu();
//		break;
//	  case options:
//		drawOptionsMenu(size_x * OPTIONS_MENU_X / 100);
//		handleOptionMenu();
//	}
//}
//
//void __fastcall drawFileMenu(unsigned char x)
//{
//	//unsigned char i = 0;
//	//unsigned char buffer[79];
//
//	initFileMenu();
//
//	drawMenu(x,
//		FILE_MENU_WIDTH,
//		FILE_MENU_COUNT,
//		FILE_MENU_LABELS,
//		FILE_MENU_KEYS);
//}
//
//void __fastcall drawDriveMenu(unsigned char x)
//{
//	//unsigned char i = 0;
//	//unsigned char buffer[79];
//
//	initDriveMenu();
//
//	drawMenu(x,
//		DRIVE_MENU_WIDTH,
//		DRIVE_MENU_COUNT,
//		DRIVE_MENU_LABELS,
//		DRIVE_MENU_KEYS);
//}
//
//
//void __fastcall drawCommandMenu(unsigned char x)
//{
//	//unsigned char i = 0;
//	//unsigned char buffer[79];
//
//	initCommandMenu();
//
//	drawMenu(x,
//		COMMAND_MENU_WIDTH,
//		COMMAND_MENU_COUNT,
//		COMMAND_MENU_LABELS,
//		COMMAND_MENU_KEYS);
//}
//
//void __fastcall drawOptionsMenu(unsigned char x)
//{
//	//unsigned char i = 0;
//	//unsigned char buffer[79];
//
//	initOptionMenu();
//
//	drawMenu(x,
//		OPTION_MENU_WIDTH,
//		OPTION_MENU_COUNT,
//		OPTION_MENU_LABELS,
//		OPTION_MENU_KEYS);
//}
//
//static void __fastcall drawMenu(
//	unsigned char x,
//	unsigned char width,
//	unsigned char count,
//	const char* const labels[],
//	const char keys[])
//{
//	unsigned char i;
//	//static const unsigned char offset = 0x80;
//	//bool test;
//	unsigned char oldColor = textcolor(color_text_menus);
//
//	//revers(true);
//
//	currentMenuX = x;
//	//currentMenuLine = 0;
//
//	saveScreen();
//
//	drawBox(x, 1, width + 1,
//		count + 1, color_text_borders, false);
//
//	for(i=0; i < count; i++)
//	{
//		drawMenuLine(
//			(i==currentMenuLine),
//			keys[i], labels[i],
//			x + 1, i + 2);
//	}
//
//	//revers(false);
//	textcolor(oldColor);
//}
//
//static void __fastcall drawMenuLine(
//	bool reverse,
//	char key,
//	const char* label,
//	unsigned char x,
//	unsigned char y)
//{
//	unsigned char j;
//	bool test;
//	unsigned char originalReverse = revers(reverse);
//
//	for(j=0; j<strlen(label); j++)
//	{
//		test = ((char)(key ^ ('a' ^ 'A')) == label[j]);
//		if(test)
//		{
//			textcolor(color_text_highlight);
//		}
//
//		cputcxy(x + j, y, label[j]);
//
//		if(test)
//		{
//			textcolor(color_text_menus);
//		}
//	}
//
//	revers(originalReverse);
//}

// Writes the function menu bar at the bottom of the screen;
// it is scaled to the screen size.
void writeMenuBar(void)
{
	//static const unsigned char bottom = size_y - 1;

	(void)textcolor(color_text_menus);
	//revers(true);

	//cclearxy(0, 0, size_x);

	//cputsxy(   LEFT_MENU_X * size_x / 100, 0, LEFT_MENU);
	//cputsxy(   FILE_MENU_X * size_x / 100, 0, FILE_MENU);
	//cputsxy(COMMAND_MENU_X * size_x / 100, 0, COMMAND_MENU);
	//cputsxy(OPTIONS_MENU_X * size_x / 100, 0, OPTIONS_MENU);
	//cputsxy(  RIGHT_MENU_X * size_x / 100, 0, RIGHT_MENU);

	//revers(false);
#if size_x == 22
	cputsxy(1, size_y - 2, "HELP  DRIV  CPY  MKD\r\n"
		" QUIT  REFR  REN  DEL");

	revers(true);
	//textcolor(color_text_highlight);
	cputcxy(0, size_y - 2, '1');
	gotox(6); cputc('3');
	gotox(12); cputc('5');
	gotox(17); cputc('7');
	cputcxy(0, size_y - 1, '2');
	gotox(6); cputc('4');
	gotox(12); cputc('6');
	gotox(17); cputc('8');
#endif
#if size_x == 40
	cputsxy(1, size_y - 1, "HLP  QUIT  DRV  REF  CPY  REN  MDR  DEL");

	revers(true);
	//(void)textcolor(color_text_highlight);
	cputcxy(0, size_y - 1, '1');
	gotox(5);  cputc('2');
	gotox(11); cputc('3');
	gotox(16); cputc('4');
	gotox(21); cputc('5');
	gotox(26); cputc('6');
	gotox(31); cputc('7');
	gotox(36); cputc('8');
#endif
#if size_x == 80
	cputsxy(2, size_y - 1,
		"HELP     QUIT    DRIVE    REFRESH   COPY    RENAME    MAKE DIRECTORY    DELETE");

	revers(true);
	//(void)textcolor(color_text_highlight);
#ifdef __PET__
	cputcxy(1, size_y - 1, '1');
	gotox(10); cputc('2');
	gotox(18); cputc('3');
	gotox(27); cputc('4');
	gotox(37); cputc('5');
	gotox(45); cputc('6');
	gotox(55); cputc('7');
	gotox(73); cputc('8');
#else
	cputsxy(0, size_y - 1, "F1");
	gotox(9);  cputs("F2");
	gotox(17); cputs("F3");
	gotox(26); cputs("F4");
	gotox(36); cputs("F5");
	gotox(44); cputs("F6");
	gotox(54); cputs("F7");
	gotox(72); cputs("F8");
#endif
#endif

	revers(false);
}
