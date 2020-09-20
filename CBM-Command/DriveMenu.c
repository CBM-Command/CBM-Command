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
#include "drives.h"
//#include "globals.h"
#include "menus.h"
#include "screen.h"

//const char* DRIVE_MENU_LABELS[2];
//unsigned char DRIVE_MENU_KEYS[2];
//
//void initDriveMenu(void)
//{
//	static bool isInitialized = false;
//
//	if(!isInitialized)
//	{
//		//DRIVE_MENU_LABELS[0] = DRIVE_MENU_ON_OFF;
//		DRIVE_MENU_LABELS[0] = DRIVE_MENU_REREAD;
//		DRIVE_MENU_LABELS[1] = DRIVE_MENU_DRIVE;
//
//		//DRIVE_MENU_KEYS[0] = DRIVE_MENU_ON_OFF_KEY;
//		DRIVE_MENU_KEYS[0] = DRIVE_MENU_REREAD_KEY;
//		DRIVE_MENU_KEYS[1] = DRIVE_MENU_DRIVE_KEY;
//
//		isInitialized = true;
//	}
//}
//
//void __fastcall handleDriveMenu(enum menus menu)
//{
//	bool finalRetrieve = true;
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
//			if(currentMenuLine == 0) currentMenuLine = DRIVE_MENU_COUNT;
//			--currentMenuLine;
//			retrieveScreen();
//			drawDriveMenu(currentMenuX);
//			continue;
//
//		case CH_CURS_DOWN:
//			if(++currentMenuLine == DRIVE_MENU_COUNT) currentMenuLine = 0;
//			retrieveScreen();
//			drawDriveMenu(currentMenuX);
//			continue;
//
//		case CH_CURS_LEFT:
//			retrieveScreen();
//			currentMenuLine = 0;
//			if(menu == right)
//			{
//				writeMenu(command);
//			}
//			else
//			{
//				writeMenu(right);
//			}
//			return;
//
//		case CH_CURS_RIGHT:
//			retrieveScreen();
//			currentMenuLine = 0;
//			if(menu == left)
//			{
//				writeMenu(file);
//			}
//			else
//			{
//				writeMenu(left);
//			}
//			return;
//
//		case CH_ENTER:
//			key = DRIVE_MENU_KEYS[currentMenuLine];
//
//		default:
//			//if(key == DRIVE_MENU_ON_OFF_KEY)
//			//{
//			//	retrieveScreen();
//			//	notImplemented();
//			//	finalRetrieve = false;
//			//}
//			//else
//			if(key == DRIVE_MENU_DRIVE_KEY)
//			{
//				retrieveScreen();
//				writeDriveSelectionPanel(menu);
//				finalRetrieve = false;
//			}
//			else if(key == DRIVE_MENU_REREAD_KEY)
//			{
//				retrieveScreen();
//				rereadDrivePanel(menu);
//				finalRetrieve = false;
//			}
//			else
//			{
//			continue;
//			}
//		}
//		break;
//	}
//
//	if(finalRetrieve) retrieveScreen();
//}

void __fastcall rereadDrivePanel(enum menus menu)
{
	selectedPanel = (menu == left) ? &leftPanelDrive : &rightPanelDrive;
	rereadSelectedPanel();
}

void rereadSelectedPanel(void)
{
	//if(selectedPanel == NULL)
	//{
	//	selectedPanel = &leftPanelDrive;
	//}

	getDirectory(selectedPanel,
		selectedPanel->currentIndex =
		selectedPanel->displayStartAt = 0);
	resetSelectedFiles(selectedPanel);
	displayDirectory(selectedPanel);

	writeSelectorPosition(
		selectedPanel == &leftPanelDrive ? &rightPanelDrive : &leftPanelDrive,
		' ');
	writeSelectorPosition(selectedPanel, '>');

	writeCurrentFilename(selectedPanel);
}

void __fastcall writeDriveSelectionPanel(enum menus menu)
{
	listDrives(menu);
	retrieveScreen();

	rereadDrivePanel(menu);
}

void selectCurrentPanelDrive(void)
{
	writeDriveSelectionPanel(selectedPanel == &leftPanelDrive ? left : right);
}
