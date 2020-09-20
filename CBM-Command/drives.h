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

#ifndef _DRIVES_H
#define _DRIVES_H

#include <stdbool.h>
#ifdef __CBM__
#include <cbm.h>
#endif

#include "constants.h"
#include "menus.h"

/* STRUCTS */

/* Drive Status
 *-----------------------------
 *- Payton Byrd
 *-----------------------------
 * Used to store the drive number
 * and drive init. message for
 * each device.
 */
struct drive_status
{
	const unsigned char drive;
	char message[40];
};

/* Directory Node
 *-----------------------------
 *- Payton Byrd
 *-----------------------------
 * Stores the name, type, size,
 * and index of a section of the
 * sliding directory window.
 */
struct dir_node
{
	char name[17];
	unsigned char type;
	unsigned int size;
	int index;
};

/* Panel Drive
 *-----------------------------
 *- Payton Byrd
 *-----------------------------
 * Contains the "moving parts"
 * of the application.  There are
 * two Panels declared, each with
 * it's own data for selected
 * entries, the drive that is
 * associated with the panel,
 * the directory header, the 30
 * directory nodes for the sliding
 * window, the total length of the
 * directory, where the directory
 * display starts, where the
 * sliding window starts, and
 * the position (left or right) of
 * the panel.
 */
struct panel_drive
{
	bool visible;
	unsigned char* selectedEntries;
	struct drive_status *drive;
	char dollar[1];				// '$'
	char path[3 + 1];			// "123", "\0"
#ifdef __CBM__
	struct cbm_dirent header;
#else
	struct dir_node header;
#endif
	struct dir_node slidingWindow[SLIDING_WINDOW_SIZE];
	int length;
	int currentIndex;
	int displayStartAt;
	int slidingWindowStartAt;
	//enum menus position;		// XXX: not used, anymore
};

/* GLOBAL VARIABLES */
//extern struct drive_status drives[9];		// The collection of drive statuses
extern struct panel_drive leftPanelDrive;	// The left panel
extern struct panel_drive rightPanelDrive;	// The right panel
extern struct panel_drive *selectedPanel;	// The current panel

extern unsigned char displayHeight;

/* METHODS */
void initializeDrives(void);

static int __fastcall getDriveStatus(
	struct drive_status *drive);

void __fastcall__ listDrives(const enum menus menu);

//#ifndef __PLUS4__
extern unsigned char __fastcall__ checkDrive(unsigned char drive);
//#else
//static unsigned char __fastcall checkDrivePlus4(unsigned char drive);
//#endif

unsigned int __fastcall getDirectory(
	struct panel_drive *drive,
	int slidingWindowStartAt);

void __fastcall displayDirectory(
	struct panel_drive *drive);

char __fastcall getFileType(
	unsigned char type);

static void __fastcall shortenSize(
	char* buffer,
	unsigned int value);

static char* __fastcall shortenString(
	char* source);

void __fastcall writeSelectorPosition(
	struct panel_drive *panel,
	char character);

void __fastcall moveSelectorUp(
	struct panel_drive *panel);

void __fastcall moveSelectorDown(
	struct panel_drive *panel);

void selectCurrentFile(void);

void __fastcall writeCurrentFilename(
	struct panel_drive *panel);

void __fastcall enterDirectory(
	struct panel_drive *panel);

void __fastcall leaveDirectory(
	struct panel_drive *panel);

static bool __fastcall__ isDiskImage(
	struct panel_drive *panel);

static bool __fastcall__ isDirectory(
	struct panel_drive *panel);

struct dir_node* __fastcall__ getSelectedNode(
	struct panel_drive *panel);

struct dir_node* __fastcall__ getSpecificNode(
	struct panel_drive *panel, int index);

signed char __fastcall__ sendCommand(
	const struct panel_drive *panel,
	const char *command);

void __fastcall__ resetSelectedFiles(
	struct panel_drive *panel);

void __fastcall__ selectAllFiles(
	struct panel_drive *panel,
	unsigned char select);

void __fastcall__ moveTop(
	struct panel_drive *panel);

void __fastcall__ movePageUp(
	struct panel_drive *panel);

void __fastcall__ movePageDown(
	struct panel_drive *panel);

void __fastcall__ moveBottom(
	struct panel_drive *panel);

//bool __fastcall__ getDriveError(
//	unsigned char file,
//	char* message,
//	unsigned size);

// Use this function, instead of cbm_open(), to open files.  It checks the DOS
// status message.  It returns a value greater than zero if the device cannot
// be found.  It returns a negative value if the DOS reports an error;
// then, the buffer array holds the error string.
signed char __fastcall__ cbmOpen(unsigned char lfn, unsigned char device,
	unsigned char sec_addr, const char* path, const char* name,
	unsigned char errf);

#endif
