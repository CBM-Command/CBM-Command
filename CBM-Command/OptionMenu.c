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

#ifdef __CBM__
#include <cbm.h>
#endif
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "constants.h"
#include "globals.h"
#include "menus.h"
#include "screen.h"

//const char* OPTION_MENU_LABELS[2];
//unsigned char OPTION_MENU_KEYS[2];
//
//void initOptionMenu(void)
//{
//	static bool isInitialized = false;
//
//	if(!isInitialized)
//	{
//		OPTION_MENU_LABELS[0] = OPTION_MENU_OPTIONS;
//		OPTION_MENU_LABELS[1] = OPTION_MENU_SAVE;
//
//		OPTION_MENU_KEYS[0] = OPTION_MENU_OPTIONS_KEY;
//		OPTION_MENU_KEYS[1] = OPTION_MENU_SAVE_KEY;
//
//		isInitialized = true;
//	}
//}
//
//void handleOptionMenu(void)
//{
//	char key;
//
//	for (;;)
//	{
//		key = getKey();
//		switch(key)
//		{
//		case CH_STOP:
//			retrieveScreen();
//			writeStatusBar("Erased menu.");
//			return;
//
//		case CH_CURS_UP:
//			if(currentMenuLine == 0) currentMenuLine = OPTION_MENU_COUNT;
//			--currentMenuLine;
//			retrieveScreen();
//			drawOptionsMenu(currentMenuX);
//			continue;
//
//		case CH_CURS_DOWN:
//			if(++currentMenuLine == OPTION_MENU_COUNT) currentMenuLine = 0;
//			retrieveScreen();
//			drawOptionsMenu(currentMenuX);
//			continue;
//
//		case CH_CURS_LEFT:
//			retrieveScreen();
//			writeMenu(file);
//			return;
//
//		case CH_CURS_RIGHT:
//			retrieveScreen();
//			writeMenu(command);
//			return;
//
//		case CH_ENTER:
//			key = OPTION_MENU_KEYS[currentMenuLine];
//
//		default:
//			if(key == OPTION_MENU_OPTIONS_KEY)
//			{
//				writeOptionsPanel();
//			}
//			else if(key == OPTION_MENU_SAVE_KEY)
//			{
//				saveOptions();
//			}
//			else
//			{
//				continue;
//			}
//		}
//		break;
//	}
//}
//
//static void saveOptions(void)
//{
//	retrieveScreen();
//	notImplemented();
//}

//static void writeOptionsPanel(void)
//{
//	retrieveScreen();
//	notImplemented();
//}
