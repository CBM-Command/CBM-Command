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

//#include <conio.h>
//#include <stdbool.h>
//#include <stddef.h>				// for NULL
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

//#include "constants.h"
//#include "drives.h"
//#include "globals.h"
//#include "input.h"
//#include "menus.h"
//#include "screen.h"


//const char* COMMAND_MENU_LABELS[2];
//unsigned char COMMAND_MENU_KEYS[2];
//
//void initCommandMenu(void)
//{
//	static bool isInitialized = false;
//
//	if(!isInitialized)
//	{
//		COMMAND_MENU_LABELS[0] = COMMAND_MENU_SWAP_PANELS;
//		COMMAND_MENU_LABELS[1] = COMMAND_MENU_PANELS_ON_OFF;
//
//		COMMAND_MENU_KEYS[0] = COMMAND_MENU_SWAP_PANELS_KEY;
//		COMMAND_MENU_KEYS[1] = COMMAND_MENU_PANELS_ON_OFF_KEY;
//
//		isInitialized = true;
//	}
//}
//
//void handleCommandMenu(void)
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
//			break;
//
//		case CH_CURS_UP:
//			if(currentMenuLine == 0) currentMenuLine = COMMAND_MENU_COUNT;
//			--currentMenuLine;
//			retrieveScreen();
//			drawCommandMenu(currentMenuX);
//			continue;
//
//		case CH_CURS_DOWN:
//			if(++currentMenuLine == COMMAND_MENU_COUNT) currentMenuLine = 0;
//			retrieveScreen();
//			drawCommandMenu(currentMenuX);
//			continue;
//
//		case CH_CURS_LEFT:
//			retrieveScreen();
//			writeMenu(options);
//			return;
//
//		case CH_CURS_RIGHT:
//			retrieveScreen();
//			writeMenu(right);
//			return;
//
//		case CH_ENTER:
//			key = COMMAND_MENU_KEYS[currentMenuLine];
//
//		default:
//			if(key == COMMAND_MENU_SWAP_PANELS_KEY)
//			{
//				swapPanels();
//			}
//			else if(key == COMMAND_MENU_PANELS_ON_OFF_KEY)
//			{
//				togglePanels();
//			}
//			else
//			{
//				continue;
//			}
//		}
//		break;
//	}
//}

//void togglePanels(void)
//{
//	if(arePanelsOn)
//	{
//		setupScreen();
//		writeMenuBar();
//		arePanelsOn = false;
//	}
//	else
//	{
//		//retrieveScreen();
//		displayPanels();
//		arePanelsOn = true;
//	}
//}

//static void displayPanels(void)
//{
//	if(leftPanelDrive.visible)
//	{
//		rereadDrivePanel(left);
//	}
//	if(rightPanelDrive.visible)
//	{
//		rereadDrivePanel(right);
//	}
//}
