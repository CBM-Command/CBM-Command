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

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <device.h>
#ifdef __C128__
#include <c128.h>
#endif
#ifdef __CBM__
#include <cbm.h>
#endif

#include "CBM-REL.h"
#include "Configuration.h"
#include "constants.h"
#include "globalInput.h"
#include "globals.h"
#include "drives.h"
#include "input.h"
#include "menus.h"
#include "screen.h"
#include "Viewer.h"

// Number of CBM blocks in disk images (without error blocks or extra tracks)
#define D64_SIZE  689
#define D71_SIZE 1377
#define D80_SIZE 2100
#define D81_SIZE 3226
#define D82_SIZE 4199

static const char* const quit_message[] =
{
	"Quit CBM-Command?"
};
#define A_SIZE_QUIT A_SIZE(quit_message)

// Locations of screen and keyboard variables
// that are used by Commodore Kernals
#if defined(__VIC20__) || defined(__C64__)
#define LINE_TABLE ((unsigned char[size_y])0xD9)
#define KB_COUNT (*(unsigned char*)198)
#define KB_QUEUE ((unsigned char[10])631)
#endif
#if defined(__C128__)
#define KB_COUNT (*(unsigned char*)0xD0)
#define KB_QUEUE ((unsigned char[10])0x034A)
#endif
#if defined(__PET__)
#define KB_COUNT (*(unsigned char*)0x9E)
#define KB_QUEUE ((unsigned char[10])0x026F)
#endif
#if defined(__PLUS4__)
#define KB_COUNT (*(unsigned char*)0xEF)
#define KB_QUEUE ((unsigned char[10])0x0527)
#endif

// Names of Unique Disk Formats
enum
{
	F_UNKNOWN,
	F_1541,						// single-sided
	F_1571,						// double-sided
	F_1581,
	F_2030,
	F_2040,
	F_8050,						// single-sided
	F_8250,						// double-sided
	F_D2M,
	F_DNP,
	F_FAT,
	F_IDE64
};


//#ifdef __C128__
//const char* FILE_MENU_LABELS[9];
//unsigned char FILE_MENU_KEYS[9];
//#else
//const char* FILE_MENU_LABELS[8];
//unsigned char FILE_MENU_KEYS[8];
//#endif
//
//void initFileMenu(void)
//{
//	static bool isInitialized = false;
//
//	if(!isInitialized)
//	{
//		FILE_MENU_LABELS[0] = FILE_MENU_ABOUT;
//		FILE_MENU_LABELS[1] = FILE_MENU_HELP;
//		FILE_MENU_LABELS[2] = FILE_MENU_COPY;
//		FILE_MENU_LABELS[3] = FILE_MENU_RENAME;
//		FILE_MENU_LABELS[4] = FILE_MENU_MAKE_DIR;
//		FILE_MENU_LABELS[5] = FILE_MENU_DELETE;
//		FILE_MENU_LABELS[6] = FILE_MENU_SEND_COMMAND;
//		FILE_MENU_LABELS[7] = FILE_MENU_QUIT;
//#ifdef __C128__
//		FILE_MENU_LABELS[8] = FILE_MENU_GO64;
//#endif
//
//		FILE_MENU_KEYS[0] = FILE_MENU_ABOUT_KEY;
//		FILE_MENU_KEYS[1] = FILE_MENU_HELP_KEY;
//		FILE_MENU_KEYS[2] = FILE_MENU_COPY_KEY;
//		FILE_MENU_KEYS[3] = FILE_MENU_RENAME_KEY;
//		FILE_MENU_KEYS[4] = FILE_MENU_MAKE_DIR_KEY;
//		FILE_MENU_KEYS[5] = FILE_MENU_DELETE_KEY;
//		FILE_MENU_KEYS[6] = FILE_MENU_SEND_COMMAND_KEY;
//		FILE_MENU_KEYS[7] = FILE_MENU_QUIT_KEY;
//#ifdef __C128__
//		FILE_MENU_KEYS[8] = FILE_MENU_GO64_KEY;
//#endif
//
//		isInitialized = true;
//	}
//}
//
//void handleFileMenu(void)
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
//			if(currentMenuLine == 0) currentMenuLine = FILE_MENU_COUNT;
//			--currentMenuLine;
//			retrieveScreen();
//			drawFileMenu(currentMenuX);
//			continue;
//
//		case CH_CURS_DOWN:
//			if(++currentMenuLine == FILE_MENU_COUNT) currentMenuLine = 0;
//			retrieveScreen();
//			drawFileMenu(currentMenuX);
//			continue;
//
//		case CH_CURS_LEFT:
//			retrieveScreen();
//			writeMenu(left);
//			return;
//
//		case CH_CURS_RIGHT:
//			retrieveScreen();
//			writeMenu(options);
//			return;
//
//		case CH_ENTER:
//			key = FILE_MENU_KEYS[currentMenuLine];
//
//		default:
//			if(key == FILE_MENU_ABOUT_KEY)
//			{
//				retrieveScreen();
//				writeAboutBox();
//			}
//			else if(key == FILE_MENU_HELP_KEY)
//			{
//				retrieveScreen();
//				writeHelpPanel();
//			}
//			else if(key == FILE_MENU_COPY_KEY)
//			{
//				retrieveScreen();
//				copyFiles();
//			}
//			else if(key == FILE_MENU_RENAME_KEY)
//			{
//				retrieveScreen();
//				renameFile();
//			}
//			else if(key == FILE_MENU_MAKE_DIR_KEY)
//			{
//				retrieveScreen();
//				makeDirectory();
//			}
//			else if(key == FILE_MENU_DELETE_KEY)
//			{
//				retrieveScreen();
//				deleteFiles();
//			}
//			else if(key == FILE_MENU_SEND_COMMAND_KEY)
//			{
//				retrieveScreen();
//				inputCommand();
//			}
//			else if(key == FILE_MENU_QUIT_KEY)
//			{
//				retrieveScreen();
//				quit();
//			}
//#ifdef __C128__
//			else if(key == FILE_MENU_GO64_KEY)
//			{
//				retrieveScreen();
//				go64();
//			}
//#endif
//			else
//			{
//				continue;
//			}
//		}
//		break;
//	}
//}

void writeHelpPanel(void)
{
	viewFile(getcurrentdevice(),
#if __PET__
		// Most IEEE units are dual-drives.
		// Look for the help file on both disks.
		""
#else
		// Most non-IEEE units are single-drives (or, they can have partitions).
		// Avoid a CBM-DOS bug by explicitly naming the one disk
		// (or, the current partition).
		"0"
#endif
		,"cbmcmd.help");
}

static bool kbStop(void)
{
	return (kbhit() && getKey() == CH_STOP);
}

unsigned char fileBuffer[COPY_BUFFER_SIZE];
static struct panel_drive *targetPanel, *tempPanel;

#ifdef __CBM__
// Detect the format of the disk in the named drive.
//
static unsigned char __fastcall getFormat(struct panel_drive *panel)
{
	int r;
	unsigned char drive = panel->drive->drive;
	unsigned char format = F_UNKNOWN;

	// Decode the drive's reset-message.
	cbm_open(15, drive, 15, "ui");
	if ((r = cbm_read(15, buffer, (sizeof buffer) - 1)) > 0)
	{
		buffer[r] = '\0';
		if (
#ifndef __PET__
			strstr(buffer,"1541") != NULL ||
			strstr(buffer,"1540") != NULL ||
			strstr(buffer,"1570") != NULL ||
#endif
#if defined(__PET__) || defined(__C64__) || defined(__C128__)
			strstr(buffer,"2031") != NULL ||
#endif
#ifdef __PLUS4__
			//strstr(buffer, "1551") != NULL ||
			strstr(buffer, "tdisk") != NULL ||
#endif
			false)
		{
			format = F_1541;
		}
#ifndef __PET__
		//else if(strstr(buffer,"1571") != NULL)	// ambiguous
		//{
		//	format = F_1571;
		//}
		else if(strstr(buffer,"1581") != NULL)
		{
			format = F_1581;
		}
#ifdef __C64__
		else if(strstr(buffer,"ide64") != NULL)
		{
			format = F_IDE64;
		}
#endif
#endif
		else
		{
			// Either the message doesn't tell the format, or it's ambiguous.
			// Look at the format-codes at the beginning of the raw directory
			// file.
			// (Actually, this method could be used instead of the
			// reset-message, for almost all formats.)
			cbm_open(2, drive, CBM_SEQ, panel->dollar);	// I hope drive numbers work, here.
			if ((r = cbm_read(2, buffer, 2)) > 0)
			{
				// Two bytes might be tested; zero the second one
				// if only one of them could be read.
				buffer[r] = '\0';
				switch (buffer[0])
				{
				case 'a':
					// See if it's double- or single-sided.
					format = ((signed char)buffer[1] < 0) ? F_1571 : F_1541;
					break;
#if defined(__PET__) || defined(__C64__) || defined(__C128__)
				case 'c':
					// Unfortunately, the CBM 8250 doesn't say
					// "double-sided" as directly as the 1571 does.
					// The CBM 8050 has two BAM blocks, while the CBM 8250 has
					// four BAM blocks.  We read to the third block; if it has
					// the proper format code, then it's a BAM block, and the
					// format is double-sided.  If not, then it's a directory
					// block, and the format is single-sided.
					format = ((r = cbm_read(2, fileBuffer, 254*3)) == 254*3
						&& *(unsigned*)(&fileBuffer[254*3-2]) == 'c')
						? F_8250 : F_8050;
					break;
#endif
#ifndef __PET__
				case 'd':
					format = F_1581;
					break;
				case 'h':
					format = F_DNP;
					break;
//#ifdef __C64__
//				case 'i':
//					format = F_IDE64;
//					break;
//#endif
#endif
				}
			}
			cbm_close(2);
		}
	}
	cbm_close(15);

	if (r <= 0)
	{
		writeStatusBar("Error getting format");
		//waitForEnterEsc();
	}

	return format;
}
#endif

void copyFiles(void)
{
	bool multipleSelected = false, RELOAD = false;
	unsigned numSelectors = (selectedPanel->length + (7 - 1)) / 8u;
	unsigned copyBlocks = COPY_BUFFER_SIZE/254u;
	unsigned long totalBytes = 0;
#ifndef __VIC20__
#ifdef __PET__
	clock_t timeStart;
#else
	time_t timeStart;
#endif
	time_t timeSpent;
#endif
	unsigned i, k, index;
	char targetFilename[3 + 2 + 16 + 2 + 2 + 1];
	const struct dir_node *currentNode;
	unsigned char j, sd, td, rel_size, rel_bytes;
	const char *sp, *tp;
	int r, bytes;
	static struct position_rel command = {'p', 96 + 2, 0, 1};
	//static struct rel_file* relativeFile;
	static struct rel_file_rec relativeRecord;

	targetPanel =
		(selectedPanel == &leftPanelDrive) ? &rightPanelDrive : &leftPanelDrive;
	if (!targetPanel->visible)
	{
		return;
	}

	if(((td =   targetPanel->drive->drive) ==
		(sd = selectedPanel->drive->drive)) &
		(strcmp(tp =   targetPanel->path,
				sp = selectedPanel->path) == 0))
	{
		//saveScreen();
		writeStatusBar(
#if size_x < 40
			"Can't copy on 1 drive"
#else
			"Can't copy to the same drive"
#endif
			);
		//waitForEnterEsc();
		//retrieveScreen();
		return;
	}

#if size_x > 40
	saveScreen();
#endif

	// XXX: This doesn't ignore the select-all-files overflow.  If you "select
	// all", then deselect every file one-by-one, this code still might think
	// that some files are chosen.  Fortunately, that event is rare; and,
	// "deselecting all" is the easy work-around.
	for(i=0; i<numSelectors; i++)
	{
		if(selectedPanel->selectedEntries[i] != 0x00) multipleSelected = true;
	}
	if(!multipleSelected)
	{
		selectCurrentFile();
		writeSelectorPosition(selectedPanel, '>');
	}

	if (getFormat(selectedPanel) == F_IDE64)
	{
		copyBlocks = COPY_BUFFER_SIZE/256u;
	}

#ifndef __VIC20__
#ifdef __PET__
	timeStart = clock();
#else
	timeStart = time(NULL);
#endif
#endif
	for(i=0; i<numSelectors; i++)
	{
		for(j=0; j<8; ++j)
		{
			if ((selectedPanel->selectedEntries[i] & (1 << j)) != 0x00
				&& (k = i*8+j) < selectedPanel->length - 1)
			{
				if ((currentNode = getSpecificNode(selectedPanel, k)) == NULL)
				{
					getDirectory(selectedPanel, k);
					currentNode = getSpecificNode(selectedPanel, k);
					//if(currentNode == NULL)
					//{
					//	waitForEnterEscf("Can't get file %u", k);
					//	return;
					//}
				}
#ifdef __CBM__
				// Copy only sequential and relative files.
				if(currentNode->type >= _CBM_T_SEQ && currentNode->type < _CBM_T_REL)
				{
					//cbm_open(14, sd, 15, "");
					if ((r = cbmOpen(1, sd, CBM_SEQ, sp, currentNode->name, 14))
						== 0)
					{
						// Target file will be replaced automatically.
						sprintf(targetFilename,"@%s:%s,%c,w",
							tp,currentNode->name,
							tolower(getFileType(currentNode->type)));
						//cbm_open(15,td,15,"");
						if ((r = cbmOpen(2, td, 3, "", targetFilename, 15)) == 0)
						{
							drawBox(
								getCenterX(20), getCenterY(3),
								19, 3,
								color_text_borders,
								false);

							for(index=0; index < currentNode->size; index+=copyBlocks)
							{
								//writeStatusBarf("%s %u/%u",
								//	currentNode->name, index,
								//	currentNode->size);
								drawProgressBar(currentNode->name,
									index, currentNode->size);

								if ((bytes = cbm_read(1, fileBuffer,
									COPY_BUFFER_SIZE)) < 0)	// Catch errors.
								{
									waitForEnterEscf(
#if size_x > 22
										"Problem (%u) reading %s",
#else
										"Can't (%u) read %s",
#endif
										_oserror,
										currentNode->name);
									bytes = cbm_read(14,
										buffer, (sizeof buffer) - 1);
									buffer[bytes < 0 ? 0 : bytes] = '\0';
									writeStatusBar(buffer);
									waitForEnterEsc();
									break;
								}
								if(bytes == 0)	// Catch end-of-file, too.
								{
									break;
								}

								if(kbStop())
								{
									cbm_close(2);
									cbm_close(1);
									cbm_close(15);
									cbm_close(14);

									reloadPanels();
									writeStatusBar("Aborted copy");
									return;
								}

								if (cbm_write(2, fileBuffer, bytes) != bytes)
								{
									waitForEnterEscf(
#if size_x > 22
										"Problem (%u) writing %s",
#else
										"Can't (%u) write %s",
#endif
										_oserror,
										currentNode->name);
									bytes = cbm_read(15,
										buffer, (sizeof buffer) - 1);
									buffer[bytes < 0 ? 0 : bytes] = '\0';
									writeStatusBar(buffer);
									waitForEnterEsc();
									break;
								}
#ifndef __VIC20__
#ifdef __PET__
								timeSpent = (clock() - timeStart) / CLOCKS_PER_SEC;
#else
								timeSpent = (time(NULL) - timeStart);
#endif
								writeStatusBarf("%u:%02u e.t. %d B/s",
									(unsigned)timeSpent/60u,
									(unsigned)timeSpent%60u,
									(unsigned)((totalBytes +=
										(unsigned long)bytes)/timeSpent));
#endif
							}
							//drawProgressBar("", index, currentNode->size);
							drawProgressBar("", 1, 1);	// 100%
							RELOAD = true;
						}
						else
						{
							waitForEnterEscf("Can't write %s (%d)",
								currentNode->name, r);
							if (r < 0)
							{
								writeStatusBar(buffer);
								waitForEnterEsc();
							}
						}
					}
					else
					{
						waitForEnterEscf("Can't read %s (%d)",
							currentNode->name, r);
						if (r < 0)
						{
							writeStatusBar(buffer);
							waitForEnterEsc();
						}
					}

					cbm_close(2);
					cbm_close(15);
					cbm_close(1);
					cbm_close(14);
				}
				else if(currentNode->type == _CBM_T_REL)
				{
					//cbm_open(127, sd, 15, "");

					// Get record size.
					if ((rel_size = getRecordSize(127, sd, sp, currentNode->name))
						== 0)
					{
						writeStatusBar(buffer);
						waitForEnterEsc();
					}
					else
					{
						// Make a read- and write-filename string.
						// Set the last byte to the size of the records.
						sprintf(targetFilename, "%s:%s,l,%c",
							tp, currentNode->name, rel_size);

						// Open the source file.
						cbm_open(2, sd, 2, targetFilename);

						//cbm_open(126, td, 15, "");

						// Open the target file.
						if (cbmOpen(3, td, 3, "", targetFilename, 126) != 0)
						{
							writeStatusBar(buffer);
							waitForEnterEsc();
						}
						else
						{
							// Close it to make sure the file was created.
							cbm_close(3);

							// Re-open it.
							cbm_open(3, td, 3, targetFilename);

							writeStatusBar("Copying file...");
							command.rec_number = 0;
							for (;;)
							{
								//relativeRecord.record_number =
								++command.rec_number;
								cbm_write(127u, &command, sizeof command);

								rel_bytes = cbm_read(2,
									relativeRecord.record_data, rel_size);

								// Get the status of the source file operation.
								cbm_read(127u, buffer, sizeof buffer);

								// Look at the first two bytes of the status.
								//buffer[2] = '\0';

								// Convert them to an integer.
								if ((r=atoi(buffer)) != 00)
								{
									if (r != 50)	// is it error or end of file?
									{
										waitForEnterEscf(
#if size_x > 22
											"Error - rec: %u - result: %u",
#else
											"rec: %u - res: %u",
#endif
											command.rec_number, r);
									}
									break;
								}

								writeStatusBarf(
#if size_x > 22
									"Writing "
#endif
									"rec. %u, len. %u",
									command.rec_number, rel_bytes);
								totalBytes += cbm_write(3,
									relativeRecord.record_data, rel_bytes);

								if (kbStop())
								{
									cbm_close(3); cbm_close(126);
									cbm_close(2); cbm_close(127);

									reloadPanels();
									writeStatusBar("Aborted copy");
									return;
								}
							}
							//waitForEnterEscf("Stopped at rec. %u, len. %u",
							//	command.rec_number, rel_bytes);

							RELOAD = true;
							writeStatusBar("Closing...");
						}

						cbm_close(3); cbm_close(126);
						cbm_close(2);
					}
					cbm_close(127);
				}
#endif
			}
		}
	}
	if(RELOAD)
	{
		reloadPanels();

#ifndef __VIC20__
#ifdef __PET__
		timeSpent = (clock() - timeStart) / CLOCKS_PER_SEC;
#else
		timeSpent = (time(NULL) - timeStart);
#endif
		writeStatusBarf("%u:%02u e.t. %d B/s",
			(unsigned)timeSpent/60u,
			(unsigned)timeSpent%60u,
			(unsigned)(totalBytes/timeSpent));
#endif
	}
}

void reloadPanels(void)
{
#if size_x > 40
	retrieveScreen();
#endif
	tempPanel = selectedPanel;
	selectedPanel = targetPanel;
	rereadSelectedPanel();
	selectedPanel = tempPanel;
	rereadSelectedPanel();
	//writeSelectorPosition(targetPanel, ' ');
	//writeSelectorPosition(selectedPanel, '>');
	//writeCurrentFilename(selectedPanel);
}

void renameFile(void)
{
	signed char commandResult;
	unsigned char dialogResult;
	const struct dir_node *selectedNode;
	char command[41];
	char filename[17];
	static const char* const dialogMessage[] =
	{
		{ "Enter new name" },
		{ "for file:" }
	};

	//if(selectedPanel != NULL)
	{
		if ((selectedNode = getSelectedNode(selectedPanel)) != NULL)
		{
			// Let the user edit the original name -- a little bit.
#if size_x > 22
			writeStatusBarf("Old name: %s",
				strcpy(filename, selectedNode->name));
#else
			writeStatusBar(
				strcpy(filename, selectedNode->name));
#endif

			//saveScreen();
			dialogResult = drawInputDialog(
				A_SIZE(dialogMessage), (sizeof filename) - 1,
				dialogMessage,
				"Rename",
				filename);
			//retrieveScreen();

			if(dialogResult == OK_RESULT)
			{
				sprintf(command, "r%s:%s=%s", selectedPanel->path,
					filename, selectedNode->name);
				commandResult = sendCommand(selectedPanel, command);

				// Keep a copy of the original name.
				strcpy(filename, selectedNode->name);

				// Show the new file-name (but, don't move the directory).
				//rereadSelectedPanel();
				getDirectory(selectedPanel, selectedPanel->slidingWindowStartAt);
				displayDirectory(selectedPanel);
				writeSelectorPosition(selectedPanel, '>');

				if (commandResult >= 0)
				{
					writeStatusBarf(
#if size_x > 22
						"Renamed from %s",
#else
						"From %s",
#endif
						filename);
				}
			}
		}
	}
}

void makeDirectory(void)
{
	unsigned char dialogResult;
	char command[3 + 3 + 16 + 1];
	static const char* const dialogMessage[] =
	{
		{ "Enter name for" },
		{ "new directory:" }
	};

	//if(selectedPanel != NULL)
	{
		sprintf(command, "md%3s:", selectedPanel->path);

		//saveScreen();
		dialogResult = drawInputDialog(
			A_SIZE(dialogMessage), (sizeof command) - 3 - 1,
			dialogMessage,
			"New Directory",
			&command[6]);
		//retrieveScreen();

		if(dialogResult == OK_RESULT)
		{
			sendCommand(selectedPanel, command);

			//getDirectory(selectedPanel,
			//	selectedPanel->slidingWindowStartAt);
			//displayDirectory(selectedPanel);
			rereadSelectedPanel();
		}
	}
}

static signed char __fastcall removeFile(const struct dir_node *selectedNode)
{
	char command[41];

	writeStatusBarf(
#if size_x > 22
		"Deleting %s",
#else
		"Dltng %s",
#endif
		selectedNode->name);
#ifdef __CBM__
	sprintf(command,
		(selectedNode->type == _CBM_T_DIR) ? "rd%s:%s" : "s%s:%s",
		 selectedPanel->path, selectedNode->name);
#else
	command[0] = '\0';
#endif
	return sendCommand(selectedPanel, command);
}

void deleteFiles(void)
{
	const struct dir_node *selectedNode;
	unsigned k;
	int i;
	signed char j;
	bool isBatch = false;
	static const char* const dialogMessage[] =
	{
		{ "Are you sure?" }
	};

	// Don't ask the security question if the directory is empty.
	if (selectedPanel->length == 1)
	{
		return;
	}

	//saveScreen();
	if(writeYesNo("Delete files", dialogMessage, A_SIZE(dialogMessage)))
	{
		//retrieveScreen();
		writeStatusBar("Deleting files...");
		for (i = (selectedPanel->length + (7 - 1)) / 8u; --i >= 0; )
		{
			for (j = 8; --j >= 0; )
			{
				if ((selectedPanel->selectedEntries[i] & (1 << j)) != 0x00
					&& (k = i*8+j) < selectedPanel->length - 1)
				{
					isBatch = true;

					if ((selectedNode = getSpecificNode(selectedPanel, k)) == NULL)
					{
						getDirectory(selectedPanel, k - (SLIDING_WINDOW_SIZE - 1));
						selectedNode = getSpecificNode(selectedPanel, k);
					}

					if (removeFile(selectedNode) < 0 ||
						// Allow us to change our minds,
						// and stop a batch delete.
						kbStop())
					{
						rereadSelectedPanel();
						return;
					}
				}
			}
		}

		if(!isBatch)
		{
			// No names are highlighted; delete the current file.
			if ((selectedNode = getSelectedNode(selectedPanel)) != NULL)
			{
				removeFile(selectedNode);
				rereadSelectedPanel();
			}
		}
		else
		{
			rereadSelectedPanel();
		}
	}
	else
	{
		//retrieveScreen();
	}
}

#ifdef __C128__
void go64(void)
{
	if (writeYesNo("Confirm GO64", quit_message, A_SIZE_QUIT))
	{
		writeStatusBar("Going to 64 mode. Bye!");
		c64mode();
	}

	//retrieveScreen();
}
#endif

void quit(void)
{
	if (writeYesNo("Confirm", quit_message, A_SIZE_QUIT))
	{
		clrscr();
		writeStatusBar("Bye!");

		(void)bordercolor(outsideFrame);
		(void)bgcolor(outsideScreen);
		(void)textcolor(outsideText);

		exit(EXIT_SUCCESS);
	}

	//retrieveScreen();
}

#if 0
void writeAboutBox(void)
{
	writeStatusBar("Thanks for using CBM-Command.");
}
#endif

void executeSelectedFile(void)
{
	const struct dir_node *currentNode;
	static const char* const message[] =
	{
		{ "Read as text?" }
	};

	//if(selectedPanel != NULL)
	{
		if ((currentNode = getSelectedNode(selectedPanel)) != NULL)
		{
			saveScreen();
			switch (currentNode->type)
			{
#ifdef __CBM__
			case _CBM_T_PRG:
				if(!writeYesNo(currentNode->name, message, A_SIZE(message)))
				{
					//retrieveScreen();
#ifdef KB_COUNT
					if(writeYesNo("Confirm execute", quit_message, A_SIZE_QUIT))
					{
						(void)bordercolor(outsideFrame);
						(void)bgcolor(outsideScreen);
						(void)textcolor(outsideText);
						clrscr();

// Long file-names will cause line wrap-around on the VIC-20.
// Therefore, "LOAD ... RUN" must be printed differently.
#if size_x < 40
						cprintf("lO\"%s\",%u,1",
							currentNode->name, selectedPanel->drive->drive);
						LINE_TABLE[1] = 0x10;	// link top two physical lines

						KB_QUEUE[0] = 0x13;	// HOME
						KB_QUEUE[1] = '\n';
						KB_QUEUE[2] = 'r';
						KB_QUEUE[3] = 'U';
						KB_QUEUE[4] = '\n';

						KB_COUNT = 5;
#else
						cprintf("lO\"%s\",%u,1\r\n\n\n\n\nrU",
							currentNode->name, selectedPanel->drive->drive);

						KB_QUEUE[0] = 0x13;
						KB_QUEUE[1] = '\n';
						KB_QUEUE[2] = '\n';

						KB_COUNT = 3;
#endif

						exit(EXIT_SUCCESS);
					}
					//retrieveScreen();
#endif
					break;
				}
				//retrieveScreen();
				// Fall through.
			case _CBM_T_SEQ:
			//case _CBM_T_USR:
			case _CBM_T_OTHER:
#endif
				retrieveScreen();
				viewFile(selectedPanel->drive->drive,
					selectedPanel->path,
					currentNode->name);
			}
		}
	}
}

static char __fastcall__ inputLine(char line[], unsigned char width)
{
	//unsigned char dialogResult;
	unsigned char key;
	unsigned char count = 0;
	//static const char* const dialogMessage[] =
	//{
	//	{ "Type drive-command" }
	//};

	//saveScreen();
	//dialogResult = drawInputDialog(
	//	A_SIZE(dialogMessage),
	//	size_x - 4,
	//	dialogMessage,
	//	"Command",
	//	command
	//	);
	//retrieveScreen();

	//if(dialogResult == OK_RESULT)
	//{
	//}

	(void)textcolor(color_text_other);
	gotoxy(0, size_y -
#if size_x < 40
		3
#else
		2
#endif
		);
	cprintf("%2u>", selectedPanel->drive->drive);

	revers(true);
	cclear(width);

	do
	{
		gotox(count + 3); cputc('_');
		key = getKey();

		if (count < width &&
			(
				(key >= ' ' && key <= '^'+1) ||
				(key >= 'A' && key <= 'Z')
			)
		   )
		{
			gotox(count + 3); cputc(line[count] = key);
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
			gotox(count + 3); cputc(' ');
			--count;
		}
	}
	while (key != CH_ENTER && key != CH_STOP);
	line[count] = '\0';

	revers(false);
	gotox(0); cclear(size_x);

	if (count == 0 && key == CH_ENTER)
	{
		return '\0';
	}
	else
	{
		return key;
	}
}

void inputCommand(void)
{
	char command[size_x - 4], key;

	if ((key = inputLine(command, sizeof command - 1)) != CH_STOP)
	{
		sendCommand(selectedPanel, command);

		// Merely checking the status doesn't change the directory;
		// therefore, don't reread it, in that case.
		if (key != '\0')
		{
			rereadSelectedPanel();
		}
	}
}

void setPath(void)
{
	char path[sizeof selectedPanel->path];

	if (inputLine(path, sizeof path - 1) != CH_STOP)
	{
		strcpy(selectedPanel->path, path);
		rereadSelectedPanel();
	}
}

#if defined(__CBM__) //&& !defined(__VIC20__)
//static unsigned char temp[256];
static const unsigned char l[] =	// sectors per track on 1541/1571 disks
// Note:  This table can handle both sides of a 1571 because the modulo
// operator effectively "unfolds" it into two copies.
{	21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,
	19,19,19,19,19,19,19,
	18,18,18,18,18,18,
	17,17,17,17,17
};

bool __fastcall createDiskImage(const char *filename)
{
	static const char* const message[] =
	{
		{ "Type a name for" },
		{ "the disk image" }
	};
	char name[16 + 1];
	unsigned char sd, td, i, j, sectorsThisTrack, tracks;
	const char *sp, *tp;
	//struct dir_node *currentNode;
	unsigned char result;
	int r;
#ifndef __VIC20__
#ifdef __PET__
	clock_t timeStart;
#else
	time_t timeStart;
#endif
	time_t timeSpent, timeLeft;
#endif
	bool isD64 = true, isD71 = false, created = false;
	unsigned int p = 0, size = D64_SIZE;

	//if(selectedPanel != NULL && selectedPanel->drive != NULL)
	{
		targetPanel = (selectedPanel == &leftPanelDrive) ?
			&rightPanelDrive : &leftPanelDrive;

		if((
			//targetPanel->drive != NULL &&
			(td =   targetPanel->drive->drive) !=
			(sd = selectedPanel->drive->drive)) |
			(strcmp(tp =   targetPanel->path,
					sp = selectedPanel->path) != 0))
		{
			switch (j = getFormat(selectedPanel))
			{
			case F_1541:
//				writeStatusBar("Making D64");
				//waitForEnterEsc();
				writeStatusBarf("Making D64. %u %u", size, isD64);
				break;
			case F_1571:
				isD64 = false;
				isD71 = true;
				size=D71_SIZE;
//				writeStatusBar("Making D71");
				//waitForEnterEsc();
				writeStatusBarf("Making D71. %u", size);
				break;
			case F_1581:
				isD64 = false;
				//isD71 = false;
				size=D81_SIZE;
//				writeStatusBar("Making D81");
				//waitForEnterEsc();
				writeStatusBarf("Making D81. %u", size);
				break;
			default:
				writeStatusBarf("Unsupported format: %u", j);
				//waitForEnterEsc();
				return created;
			}

			//currentNode = getSelectedNode(selectedPanel);
			if(filename == NULL)
			{
				name[0]='\0';
				//saveScreen();
				result = drawInputDialog(
					A_SIZE(message), (sizeof name) - 1,
					message, "Make Image",
					(char *)(filename = name));
				//retrieveScreen();
			}
			else
			{
				//sprintf(name, ":%s", filename);
				result = OK_RESULT;
			}

			if(result == OK_RESULT)
			{
				//strlower(name);
				//if(strstr(name,".d64") == NULL &&
				//	strstr(name,".d41") == NULL)
				//{
				//	strcat(name, ".d64");
				//}

				//cbm_open(15, sd, 15, "");
				if((r = cbmOpen(2, sd, 2, "", "#", 15)) == 0)
				{
#ifndef __VIC20__
#ifdef __PET__
					timeStart = clock();
#else
					timeStart = time(NULL);
#endif
#endif
					//cbm_open(14,td,15,"");
					if((signed char)(r = cbmOpen(3,td,CBM_WRITE,tp,filename,14)) == 0)
					{
						//saveScreen();
						drawBox(
							getCenterX(20), getCenterY(3),
							19, 3,
							color_text_borders,
							false);

						tracks = isD64 ? 35 : (isD71 ? 35*2 : 80);
						for(i=0;i<tracks;++i)
						{
							sectorsThisTrack = (isD64 || isD71) ? l[i%35] : 40;
							for(j=0;j<sectorsThisTrack;++j)
							{
								if(kbStop())
								{
									// Break out of the outer loop by presetting
									// the track number beyond the highest
									// supported value.
									i=77*2;
									break;
								}

								drawProgressBar("Making image...", ++p, size);

								cbm_write(15, buffer,
									sprintf(buffer,"u1 2 0 %u %u", i+1, j));

#ifndef __VIC20__
#ifdef __PET__
								timeSpent = (clock() - timeStart) / CLOCKS_PER_SEC;
#else
								timeSpent = (time(NULL) - timeStart);
#endif
								timeLeft =
									//((long)(size - p) * 256L) // bytes remaining
									// / (((long)p * 256L)/timeSpent);
									(long)(size - p) * timeSpent / (long)p;
								writeStatusBarf(
#if size_x < 40
									"%u:%02u et %d B/s %u:%02u",
#elif size_x < 80
									"%u:%02u et %d B/s %u:%02u rem %4u/%4u",
#else
									"%u:%02u et %d B/s %u:%02u rem %4u/%4u - %3u,%2u",
#endif
									(unsigned)timeSpent/60u,
									(unsigned)timeSpent%60u,
									(unsigned)(((long)p * 256L)/timeSpent),
									(unsigned)timeLeft/60u,
									(unsigned)timeLeft%60u
#if size_x >= 40
									, p, size
#if size_x >= 80
									, i+1, j
#endif
#endif
									);
#endif

								r = cbm_read(2,fileBuffer, 256);
								// XXX: Check that r==256.
								cbm_write(3, fileBuffer, 256);
							}
						}
						cbm_close(2); cbm_close(3);
						cbm_close(15); cbm_close(14);
#ifndef __VIC20__
#ifdef __PET__
						timeSpent = (clock() - timeStart) / CLOCKS_PER_SEC;
#else
						timeSpent = (time(NULL) - timeStart);
#endif
						//retrieveScreen();
						reloadPanels();
						writeStatusBarf(
							(i == 77*2+1) ? "Stopped" :
								(created = true, "%u:%02u e.t. %d B/s"),
							(unsigned)timeSpent/60u,
							(unsigned)timeSpent%60u,
							(unsigned)(((long)size * 256L)/timeSpent));
#else
						reloadPanels();
						if (i == 77*2+1)
						{
							writeStatusBar("Stopped");
						}
						else
						{
							created = true;
						}
#endif
					}
					else
					{
#if size_x > 22
						writeStatusBarf("Target: %s",
							r >= 0 ? "unit not there" : buffer);
#else
						writeStatusBar(r >= 0 ? "Target not there" : buffer);
#endif
						cbm_close(3); cbm_close(14);
					}
				}
				else
				{
#if size_x > 22
					writeStatusBarf("Source: %s",
						r >= 0 ? "unit not there" : buffer);
#else
					writeStatusBar(r >= 0 ? "Source not there" : buffer);
#endif
				}
				cbm_close(2); cbm_close(15);
			}
			else
			{
				writeStatusBar("Stopped");
			}
		}
	}
	return created;
}

void batchCreateDiskImage(void)
{
#if !defined(__VIC20__) && !defined(__PET__)
	unsigned int count;
	char input[17], filename[16 + 1];
	static const char* const message[] =
	{
		"Enter the start-",
		"ing disk number",
		"(0 to 9999)"
	};

	input[0] = '\0';
	//saveScreen();
	if (drawInputDialog(
		A_SIZE(message), sizeof input - 1,
		message, "Batch Imaging", input) == OK_RESULT)
	{
		//retrieveScreen();

		input[4] = '\0';
		count = atoi(input);
		//if(sscanf(input, "%4u", &count) == 1)
		{
			do
			{
				sprintf(filename, "disk%04u.d64", count); ++count;
				if (!createDiskImage(filename))
				{
					break;
				}

				writeStatusBar("RETURN: next image, STOP/ESC: quit");
			}
			while (waitForEnterEsc() != CH_STOP);
		}
	}
#else
	writeStatusBar("Not implemented");
#endif
}

void writeDiskImage(void)
{
	static const char* const message[] =
	{
		{ "Is a blank disk" },
		{ "in the target " },
		{ "drive?" }
	};

	static const char* const confirm[] =
	{
		{ "Do you want to" },
		{ "format, first?" }
	};

	bool confirmed;
	unsigned char sd, td, i, j;
	const char *sp, *tp;
	const struct dir_node *currentNode;
#ifndef __VIC20__
#ifdef __PET__
	clock_t timeStart;
#else
	time_t timeStart;
#endif
	time_t timeSpent, timeLeft;
#endif
	int r;
	unsigned char tracks, sectors;
	unsigned int p = 0;

	targetPanel = (selectedPanel == &leftPanelDrive) ?
		&rightPanelDrive : &leftPanelDrive;

	if ((
		//targetPanel->drive != NULL &&
		(td =   targetPanel->drive->drive) !=
		(sd = selectedPanel->drive->drive)) |
		(strcmp(tp =   targetPanel->path,
				sp = selectedPanel->path) != 0))
	{
		if ((currentNode = getSelectedNode(selectedPanel)) != NULL)
		{
			switch (currentNode->size)
			{
			case D64_SIZE:
			case D71_SIZE:
			case D81_SIZE:
			case D64_SIZE * 254 / 256:		// IDE64 sizes
			case D71_SIZE * 254 / 256 + 1:
			case D81_SIZE * 254 / 256 + 1:
				break;
			default:
				saveScreen();
				writeStatusBar("Image not supported");
				waitForEnterEsc();
				retrieveScreen();
				return;
			}

			//saveScreen();
			confirmed = writeYesNo("Put image on disk",
				message, A_SIZE(message));
			//retrieveScreen();

			if(confirmed)
			{
				confirmed = writeYesNo("Format Disk",
					confirm, A_SIZE(confirm));

				//cbm_open(15, sd, 15, "");
				if((r = cbmOpen(2, sd, CBM_SEQ, sp, currentNode->name, 15)) == 0)
				{
					/*
					100 OPEN 1,8,15
					110 OPEN 2,8,2, "#"
					120 PRINT#2, "TEST DATA"
					130 PRINT#1, "U2 2 0 10 0"
					140 CLOSE 2 : CLOSE 1
					*/

					if(confirmed)
					{
						writeStatusBar("Formatting disk...");
						//cbm_open(14, td, 15, "n:temp,00");
						sprintf(buffer, "n%s:temp,00", tp);
						r = cbmOpen(3, td, 15, "", buffer, 14);
					}

					cbm_close(3); cbm_close(14);

					// If the user didn't ask for a formatting,
					// then "r" is zero from the previous cbmOpen().
					//if(!confirmed || r == 0)
					if(r == 0 && (r = cbmOpen(3,td,3,"","#",14)) == 0)
					{
						//cbm_open(3,td,3,"#");

						drawBox(
							getCenterX(20), getCenterY(3),
							19, 3,
							color_text_borders,
							false);

						writeStatusBar("Making disk...");
#ifndef __VIC20__
#ifdef __PET__
						timeStart = clock();
#else
						timeStart = time(NULL);
#endif
#endif

						(void)textcolor(color_text_other);

						switch(currentNode->size)
						{
						  case D64_SIZE:
						  case D64_SIZE * 254 / 256:
							tracks = 35u;
							break;
						  case D71_SIZE:
						  case D71_SIZE * 254 / 256 + 1:
							tracks = 35u*2;
							break;
						  case D81_SIZE:
						  case D81_SIZE * 254 / 256 + 1:
							sectors = 40u;
							tracks = 80u;
							break;
						}

						//waitForEnterEscf("size: %4u, tracks: %2u", currentNode->size, tracks);

						for(i=0;i<tracks;++i)
						{
							switch(currentNode->size)
							{
							  case D64_SIZE:
							  case D71_SIZE:
							  case D64_SIZE * 254 / 256:
							  case D71_SIZE * 254 / 256 + 1:
								sectors = l[i%35];
								break;
							}
							for(j=0;j<sectors;++j)
							{
								if(kbStop())
								{
									// Break out of the outer loop by presetting
									// the track number beyond the highest
									// supported value.
									i=77*2;
									break;
								}

								drawProgressBar(currentNode->name,
									++p, currentNode->size);

								r = cbm_read(2,fileBuffer,256);
								// XXX: Check that r==256.

								//memcpy(temp, (fileBuffer + 1), 256-1);
								//temp[256-1] = fileBuffer[0];
								//
								//cbm_write(3,temp,256);
								cbm_write(14,"b-p 3 0", 7);
								cbm_write(3,fileBuffer,256);
								cbm_write(14,buffer,
									sprintf(buffer, "u2 3 0 %u %u", i+1, j));

#ifndef __VIC20__
#ifdef __PET__
								timeSpent = (clock() - timeStart) / CLOCKS_PER_SEC;
#else
								timeSpent = (time(NULL) - timeStart);
#endif
								timeLeft =
									//((long)(currentNode->size - p) * 256L) // bytes remaining
									// / (((long)p * 256L)/timeSpent);
									(long)(currentNode->size - p) * timeSpent
									/ (long)p;
								writeStatusBarf(
#if size_x < 80
									"%u:%02u e.t. %d B/s, %u:%02u rem",
#else
									"%u:%02u e.t. %d B/s, %u:%02u rem - %3u:%2u of %3u,%2u",
#endif
									(unsigned)timeSpent/60u,
									(unsigned)timeSpent%60u,
									(unsigned)(((long)p * 256L)/timeSpent),
									(unsigned)timeLeft/60u,
									(unsigned)timeLeft%60u
#if size_x >= 80
									, i + 1, j, tracks, sectors - 1
#endif
									);
#endif
							}
						}
						cbm_close(2);
						cbm_close(3);
						cbm_close(14);
						cbm_close(15);
#ifndef __VIC20__
#ifdef __PET__
						timeSpent = (clock() - timeStart) / CLOCKS_PER_SEC;
#else
						timeSpent = (time(NULL) - timeStart);
#endif
						//retrieveScreen();
						reloadPanels();
						writeStatusBarf(
							(i == 77*2+1) ? "Stopped" : "%u:%02u e.t. %d B/s",
							(unsigned)timeSpent/60u,
							(unsigned)timeSpent%60u,
							(unsigned)(((long)currentNode->size * 256uL)/timeSpent));
						//waitForEnterEsc();
						//retrieveScreen();
#else
						//retrieveScreen();
						reloadPanels();
						if (i == 77*2+1)
						{
							writeStatusBar("Stopped");
						}
#endif
					}
					else
					{
#if size_x > 22
						writeStatusBarf("Target: %s",
							r >= 0 ? "unit not there" : buffer);
#else
						writeStatusBar(r >= 0 ? "Target not there" : buffer);
#endif
						cbm_close(3); cbm_close(14);
						//waitForEnterEsc();
					}
				}
				else
				{
#if size_x > 22
					writeStatusBarf("Source: %s",
						r >= 0 ? "unit not there" : buffer);
#else
					writeStatusBar(r >= 0 ? "Source not there" : buffer);
#endif
					//waitForEnterEsc();
				}
				cbm_close(2);
				cbm_close(15);
			}
		}
	}
}

//bool selectDiskImageType(void)
//{
//	static const char* const message[] =
//	{
//		{ "Is the disk" },
//		{ "in the 1571" },
//		{ "double-sided?" }
//	};
//
//	return writeYesNo("Image Type", message, A_SIZE(message));
//}
#endif

void copyDisk(void)
{
#if defined(__CBM__) //&& !defined(__VIC20__)
	static const char* const message[] =
	{
		{ "Are you ready?" }
	};
	static bool yesNo;
	unsigned char sd, td, sf, tf;
	const char *sp, *tp;
	unsigned char j, i = 0, trackCount = 0, sectorCount = 40;

	targetPanel = (selectedPanel == &leftPanelDrive) ?
		&rightPanelDrive : &leftPanelDrive;

	if(((td =   targetPanel->drive->drive) !=
		(sd = selectedPanel->drive->drive)) |
		(strcmp(tp =   targetPanel->path,
				sp = selectedPanel->path) != 0))
	{
		//saveScreen();
		yesNo = writeYesNo("Copy Disk", message, A_SIZE(message));
		//retrieveScreen();
		if(yesNo)
		{
			// The two formats must be compatible.
			if ((sf = getFormat(selectedPanel)) == (tf = getFormat(targetPanel))
				// These matches will change a double-sided
				// into a single-sided disk!
#ifndef __PET__
				|| sf == F_1541 && tf == F_1571
#endif
#if defined(__PET__) || defined(__C64__) || defined(__C128__)
				|| sf == F_8050 && tf == F_8250
#endif
				)
			{
				// Known floppy-disk formats will set trackCount;
				// other formats won't set it.
				switch (sf)
				{
				case F_1541:
#ifdef __PLUS4__
					writeStatusBar("1541-/1551-Drive Copy.");
#else
					writeStatusBar("1541-/2031-Drive Copy.");
#endif
					trackCount = 35;
					break;
#ifndef __PET__
				case F_1571:
					writeStatusBar("1571-Drive Copy...");
					trackCount = 35*2;
					break;
				case F_1581:
					writeStatusBar("1581-Drive Copy...");
					trackCount = 80;
					break;
#endif
#if defined(__PET__) || defined(__C64__) || defined(__C128__)
				case F_8050:
					writeStatusBar("8050-Drive Copy...");
					trackCount = 77;
					break;
				case F_8250:
					writeStatusBar("1001-/8250-Drive Copy.");
					trackCount = 77*2;
					break;
#endif
				}
			}

			if (trackCount != 0)
			{
				cbm_open(15, sd, 15, "");
				cbm_open(14, td, 15, "");
				cbm_open(2, sd, 2, "#");
				cbm_open(3, td, 3, "#");

				// This is constant if it's a 1581.
				//sectorCount = 40;

				for(; i < trackCount; ++i)
				{
					// Some formats have a variable number of sectors per track.
					// Change that conut at the appropriate cylinders.
					switch (sf)
					{
					case F_1541:			// single-sided
					case F_1571:			// double-sided
						switch (i)
						{
						case 0:				// side 0
						case 0+35:			// side 1
							sectorCount = 21;
							break;
						case 17:
						case 17+35:
							sectorCount = 19;
							break;
						case 24:
						case 24+35:
							sectorCount = 18;
							break;
						case 30:
						case 30+35:
							sectorCount = 17;
						}
						break;
					case F_8050:			// single-sided
					case F_8250:			// double-sided
						switch (i)
						{
						case 0:
						case 0+77:
							sectorCount = 29;
							break;
						case 39:
						case 39+77:
							sectorCount = 27;
							break;
						case 53:
						case 53+77:
							sectorCount = 25;
							break;
						case 64:
						case 64+77:
							sectorCount = 23;
						}
						break;
					}

					for(j = 0; j < sectorCount; ++j)
					{
						if (kbStop())
						{
							// Break out of the outer loop by presetting the
							// track number beyond the highest supported value.
							i=77*2;
							break;
						}

						cbm_write(15,buffer, sprintf(buffer,"u1 2 0 %u %u",i+1,j));
						cbm_read(2,fileBuffer, 256);

						cbm_write(14, "b-p 3 0", 7);
						cbm_write(3,fileBuffer,256);
						cbm_write(14,buffer, sprintf(buffer,"u2 3 0 %u %u",i+1,j));
#if size_x > 22
						writeStatusBarf("Track %u, Sector %2u copied", i+1, j);
#else
						writeStatusBarf("%u, %2u copied", i+1, j);
#endif
					}
				}

				cbm_close(3);
				cbm_close(2);
				cbm_close(14);
				cbm_close(15);
			}
			else
			{
				writeStatusBar("1 or 2 invalid drives");
				return;
			}

			selectedPanel = targetPanel;
			rereadSelectedPanel();
			writeSelectorPosition(selectedPanel, '>');

			writeStatusBarf("Disk-copy %sed",
				(i == 77*2+1) ? "stopp" : "finish");
		}
		else
		{
			writeStatusBar("Disk-copy aborted");
		}
	}
#else
	writeStatusBar("Not implemented");
#endif
}
