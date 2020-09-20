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

#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

/* Drive Configuration */
extern unsigned char defaultLeftDrive;		// Left Panel unit on start-up
extern unsigned char defaultRightDrive;		// Right Panel unit on start-up
extern char defaultLeftDisk[4];				// Left Panel drive/partition on start-up
extern char defaultRightDisk[4];			// Right Panel drive/partition on start-up

/* Color Configuration */
extern unsigned char color_background;		// Background color for application
extern unsigned char color_border;			// Border color (does nothing on C= 128)
extern unsigned char color_selector;		// Color of the drive-/file-selector glyph
extern unsigned char color_text_borders;	// Color of panel borders
extern unsigned char color_text_menus;		// Color of the function-key and other menues
extern unsigned char color_text_files;		// Color of the filenames in the directory
extern unsigned char color_text_status;		// Color of the status bar and its text
extern unsigned char color_text_highlight;	// Color of text that needs to stand out
extern unsigned char color_text_other;		// Color for areas not covered above

/* Keys Configuration */
// Key-Map Indices
enum keymap
{
	KM_NONE = -1,
	KM_COPY,
	KM_CREATE_D64,
	KM_WRITE_D64,
	KM_DELETE,
	KM_DRIVE_LEFT,
	KM_DRIVE_RIGHT,
	KM_DRIVE_CURRENT,
	KM_DRIVE_COMMAND,
	KM_HELP,
	KM_MAKE_DIRECTORY,
	KM_ENTER_DIRECTORY,
	KM_LEAVE_DIRECTORY,
	KM_PAGE_UP,
	KM_PAGE_DOWN,
	KM_QUIT,
	KM_RENAME,
	KM_REREAD_LEFT,
	KM_REREAD_RIGHT,
	KM_REREAD_SELECTED,
	KM_SELECT,
	KM_SELECT_ALL,
	KM_DESELECT_ALL,
	KM_TO_TOP,
	KM_TO_BOTTOM,
	KM_EXECUTE_SELECTED,
	KM_COPY_DISK,
	KM_BATCH_CREATE_D64,
	KM_PATH,
	KM_ORIENTATION,
	KM_KEYMAP_SIZE
};
extern char keyMap[KM_KEYMAP_SIZE];


/* Methods */
void load(void);				// Loads configuration
void save(void);				// Saves configuration

#endif
