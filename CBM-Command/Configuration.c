/***************************************************************
Copyright (c) 2012, Payton Byrd
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

//#include <stdlib.h>
//#include <stdio.h>
#include <string.h>
//#include <conio.h>
//#include <errno.h>
#include <device.h>
#ifdef __CBM__
#include <cbm.h>
#include "Configuration-CBM.h"
#endif

#include "Configuration.h"
#include "globalInput.h"
#include "globals.h"
#include "screen.h"

/* Drive Configuration */
unsigned char defaultLeftDrive;
unsigned char defaultRightDrive;
char defaultLeftDisk[4]  = "0";
char defaultRightDisk[4] = "0";

/* Keys Configuration */
char keyMap[] =
{
	HK_COPY,			// 0
	HK_CREATE_D64,		// 1
	HK_WRITE_D64,		// 2
	HK_DELETE,			// 3
	HK_DRIVE_LEFT,		// 4
	HK_DRIVE_RIGHT,		// 5
	HK_DRIVE_CURRENT,	// 6
	HK_DRIVE_COMMAND,	// 7
	HK_HELP,			// 8
	HK_MAKE_DIRECTORY,	// 9
	HK_ENTER_DIRECTORY, //10
	HK_LEAVE_DIRECTORY, //11
	HK_PAGE_UP,			//12
	HK_PAGE_DOWN,		//13
	HK_QUIT,			//14
	HK_RENAME,			//15
	HK_REREAD_LEFT,		//16
	HK_REREAD_RIGHT,	//17
	HK_REREAD_SELECTED,	//18
	HK_SELECT,			//19
	HK_SELECT_ALL,		//20
	HK_DESELECT_ALL,	//21
	HK_TO_TOP,			//22
	HK_TO_BOTTOM,		//23
	HK_EXECUTE_SELECTED,//24
	HK_COPY_DISK,		//25
	HK_BATCH_CREATE_D64,//26
	HK_PATH,			//27
	HK_ORIENTATION		//28
};


/* Color Configuration */

// Choose useful default colors for each model.
#if defined(__C128__)
unsigned char color_background	= COLOR_BLACK;
unsigned char color_border		= COLOR_BLACK;
unsigned char color_selector	= COLOR_WHITE;
unsigned char color_text_borders= COLOR_GRAY3;
unsigned char color_text_menus	= COLOR_WHITE;
unsigned char color_text_files	= COLOR_YELLOW;
unsigned char color_text_status = COLOR_GRAY3;
unsigned char color_text_other	= COLOR_WHITE;
unsigned char color_text_highlight = COLOR_YELLOW;
#elif defined(__C64__) || defined(__CBM510__)
unsigned char color_background	= COLOR_BLUE;
unsigned char color_border		= COLOR_BLUE;
unsigned char color_selector	= COLOR_WHITE;
unsigned char color_text_borders= COLOR_GRAY2;
unsigned char color_text_menus	= COLOR_GRAY3;
unsigned char color_text_files	= COLOR_GRAY3;
unsigned char color_text_status	= COLOR_GRAY2;
unsigned char color_text_other	= COLOR_GRAY3;
unsigned char color_text_highlight = COLOR_WHITE;
#elif defined(__VIC20__)
unsigned char color_background	= COLOR_WHITE;
unsigned char color_border		= COLOR_CYAN;
unsigned char color_selector	= COLOR_RED;
unsigned char color_text_borders= COLOR_CYAN;
unsigned char color_text_menus	= COLOR_BLUE;
unsigned char color_text_files	= COLOR_BLUE;
unsigned char color_text_status	= COLOR_BLUE;
unsigned char color_text_other	= COLOR_BLACK;
unsigned char color_text_highlight = COLOR_YELLOW;
#elif defined(__PLUS4__)
unsigned char color_background	= COLOR_WHITE;
unsigned char color_border		= (BCOLOR_BLUE | CATTR_LUMA6);
unsigned char color_selector	= BCOLOR_RED;
unsigned char color_text_borders= (BCOLOR_BLUE | CATTR_LUMA6);
unsigned char color_text_menus	= BCOLOR_BLUE;
unsigned char color_text_files	= BCOLOR_BLUE;
unsigned char color_text_status	= BCOLOR_BLUE;
unsigned char color_text_other	= BCOLOR_BLACK;
unsigned char color_text_highlight = BCOLOR_YELLOW;
#else
// We don't need to define most of them on non-color systems
// because nothing on those systems will touch them.
unsigned char color_text_borders;
#endif

/* Load configuration
 * --------------------------
 * - Payton Byrd
 * --------------------------
 * Loads the configuration
 * from disk.
 */
void load(void)
{
#if defined(__CBM__)
	loadCBM();
#endif
}

#if defined(__CBM__)
void loadCBM(void)
{
	//cbm_open(15, getcurrentdevice(), 15, "");	// open the status channel
	if (cbm_open(1,
		// Get the drive from which the program was loaded.
		defaultLeftDrive = defaultRightDrive = getcurrentdevice(), 2,
#ifdef __PET__
		// We assume that IEEE systems have dual-drives.
		"cbmcmd22cfg."
#else
		// We assume that IEC systems have single-drives.
		":cbmcmd22cfg."
#endif

	// We use different filenames for the various
	// models, so that different versions can be
	// on the same disk, and not have to share
	// the same configuration.
#ifdef __C64__
		"c64"
#endif
#ifdef __C128__
		"c128"
#endif
#ifdef __PET__
		"pet"
#endif
#ifdef __VIC20__
		"vc20"
#endif
#ifdef __PLUS4__
		"plus4"
#endif
		) == 0)
	{
		// Change from the default settings only
		// if the configuration file can be read.
		if (cbm_read(1, buffer,
#ifdef COLOR_RED
			// expecting drives, keys, and colors on color systems
			19 + sizeof keyMap) == 19 + sizeof keyMap
#else
			// expecting only drives and keys on monochrome systems
			10 + sizeof keyMap) == 10 + sizeof keyMap
#endif
		   )
		{
			// Set the values from the configuration bytes.

			/* Drive Settings */
			defaultLeftDrive	= buffer[0];
			strcpy(defaultLeftDisk, &buffer[1]);
			defaultRightDrive	= buffer[5];
			strcpy(defaultRightDisk, &buffer[6]);

// The CBM/PET clan (except the CBM510) doesn't support color.
// Don't change colors if only black and white are defined.
#ifdef COLOR_RED
			/* Color settings */
			color_background	= buffer[10];
			color_border		= buffer[11];
			color_selector		= buffer[12];
			color_text_borders	= buffer[13];
			color_text_menus	= buffer[14];
			color_text_files	= buffer[15];
			color_text_status	= buffer[16];
			color_text_other	= buffer[17];
			color_text_highlight= buffer[18];

			memcpy(keyMap, buffer + 19, sizeof keyMap);
#else
			memcpy(keyMap, buffer + 10, sizeof keyMap);
#endif
		}
	}
	else
	{
		// The configuration drive couldn't be reached;
		// tell the user about that unusual condition.
		writeStatusBar("Config. drive failed");
		waitForEnterEsc();
	}

	// Close the channels
	cbm_close(1);
	//cbm_close(15);
}
#endif
