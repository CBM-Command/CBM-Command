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

//#include <stdio.h>
//#include <stdlib.h>
//#include <stdbool.h>
#include <conio.h>

#include "Configuration.h"
//#include "constants.h"
#include "drives.h"
#include "globals.h"
#include "input.h"
#include "menus.h"
#include "globalInput.h"
#include "screen.h"

void readKeyboard(void)
{
	//char buffer[39];
	char key = cgetc();
	signed char k = KM_KEYMAP_SIZE;

	// The key might be configured; look for which one.
	while (--k > KM_NONE && keyMap[k] != key)
		;
	switch (k)
	{
	case KM_REREAD_SELECTED:
		key = CH_F4;
		break;
#ifdef __CBM__
	case KM_EXECUTE_SELECTED:
		executeSelectedFile();
		return;
#endif
	case KM_DRIVE_CURRENT:
		key = CH_F3;
		break;
	case KM_SELECT:
		selectCurrentFile();
		key = CH_CURS_DOWN;
		break;
	case KM_ENTER_DIRECTORY:
		enterDirectory(selectedPanel);
		return;
	case KM_LEAVE_DIRECTORY:
		leaveDirectory(selectedPanel);
		return;
	case KM_REREAD_LEFT:
		rereadDrivePanel(left);
		return;
	case KM_REREAD_RIGHT:
		rereadDrivePanel(right);
		return;
	case KM_DRIVE_LEFT:
		writeDriveSelectionPanel(left);
		return;
	case KM_DRIVE_RIGHT:
		writeDriveSelectionPanel(right);
		return;
	case KM_SELECT_ALL:
		selectAllFiles(selectedPanel, 0xFF);
		return;
	case KM_DESELECT_ALL:
		selectAllFiles(selectedPanel, 0x00);
		return;
	case KM_HELP:
		key = CH_F1;
		break;
	case KM_QUIT:
		key = CH_F2;
		break;
	case KM_COPY:
		key = CH_F5;
		break;
	case KM_RENAME:
		key = CH_F6;
		break;
	case KM_DELETE:
		key = CH_F8;
		break;
	case KM_DRIVE_COMMAND:
		inputCommand();
		return;
	case KM_PATH:
		setPath();
		return;
	case KM_MAKE_DIRECTORY:
		key = CH_F7;
		break;
	case KM_TO_TOP:
		moveTop(selectedPanel);
		return;
	case KM_TO_BOTTOM:
		moveBottom(selectedPanel);
		return;
	case KM_PAGE_UP:
		movePageUp(selectedPanel);
		return;
	case KM_PAGE_DOWN:
		movePageDown(selectedPanel);
		return;
#if defined(__CBM__)
	case KM_WRITE_D64:
		writeDiskImage();
		return;
	case KM_CREATE_D64:
		createDiskImage(NULL);
		return;
	case KM_BATCH_CREATE_D64:
		batchCreateDiskImage();
		return;
#endif
	case KM_COPY_DISK:
		copyDisk();
		return;
#if size_x == 40
	case KM_ORIENTATION:
		swapOrientation();

		displayDirectory(&leftPanelDrive);
		leftPanelDrive.currentIndex = leftPanelDrive.displayStartAt;

		if(rightPanelDrive.visible)
		{
			displayDirectory(&rightPanelDrive);
			rightPanelDrive.currentIndex = rightPanelDrive.displayStartAt;
		}

		writeSelectorPosition(selectedPanel, '>');
		return;
#endif
	}

	// Either the key isn't configured, or it has a non-configurable twin.  We
	// use the twins, so that there is only one call to the command functions.
	switch (key)
	{
	case CH_CURS_DOWN:
		moveSelectorDown(selectedPanel);
		break;
	case CH_CURS_UP:
		moveSelectorUp(selectedPanel);
		break;
	case CH_CURS_LEFT:
		if(selectedPanel == &rightPanelDrive
			&& leftPanelDrive.visible
			/*&& arePanelsOn*/)
		{
			selectedPanel = &leftPanelDrive;
#if size_x < 40
			displayDirectory(selectedPanel);
#endif
			writeCurrentFilename(selectedPanel);
			writeSelectorPosition(& leftPanelDrive, '>');
#if size_x > 22
			writeSelectorPosition(&rightPanelDrive, ' ');
#endif
		}
		break;
	case CH_CURS_RIGHT:
		if(selectedPanel == &leftPanelDrive
#if size_x > 22
			&& rightPanelDrive.visible
#endif
			/*&& arePanelsOn*/)
		{
			selectedPanel = &rightPanelDrive;
#if size_x < 40
			displayDirectory(selectedPanel);
#endif
			writeCurrentFilename(selectedPanel);
			writeSelectorPosition(&rightPanelDrive, '>');
#if size_x > 22
			writeSelectorPosition(& leftPanelDrive, ' ');
#endif
		}
		break;
	case CH_F4:
		rereadSelectedPanel();
		break;
	case CH_F3:
		selectCurrentPanelDrive();
		break;
#ifdef __C128__
	case HK_HELP_128:
#endif
	case CH_F1:
		writeHelpPanel();
		break;
	case CH_F2:
		quit();
		break;
	case CH_F5:
		copyFiles();
		break;
	case CH_F6:
		renameFile();
		break;
	case CH_F8:
		deleteFiles();
		break;
	case CH_F7:
		makeDirectory();
		break;
#ifdef __C128__
	case HK_GO64:
		go64();
		break;
#endif
	// It isn't a non-configurable key, either; ignore it.
	}
}
