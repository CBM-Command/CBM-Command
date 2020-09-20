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

#ifndef _GLOBALS_H
#define _GLOBALS_H

#define __fastcall __fastcall__

//#include <stdbool.h>
#include <conio.h>

//extern unsigned char size_x, size_y;
#if defined(__VIC20__)
#define size_x 22u
#define size_y 23u
#elif defined(__C128__) || (defined(__PET__) && defined(_PET80_)) || defined(__CBM610__)
#define size_x 80u
#define size_y 25u
#else
#define size_x 40u
#define size_y 25u
#endif

// Global Hotkeys
#define HK_COPY_DISK		'o'
#define HK_EXECUTE_SELECTED	(CH_ENTER|0x80)
#define HK_REREAD_LEFT		'e'
#define HK_REREAD_RIGHT		'E'	// SHIFT-E
#define HK_REREAD_SELECTED	'r'
#define HK_DRIVE_LEFT		'd'
#define HK_DRIVE_RIGHT		'D'	// SHIFT-D
#define HK_DRIVE_CURRENT	'u'
#define HK_SELECT_ALL		'a'
#define HK_DESELECT_ALL		's'
#define HK_QUIT				'q'
#define HK_COPY				'c'
#define HK_RENAME			'n'
#define HK_DELETE			'l'
#define HK_MAKE_DIRECTORY	'm'
#define HK_ENTER_DIRECTORY	0xdb
#define HK_LEAVE_DIRECTORY	0xdd
#ifdef __CBM__
#define HK_TO_TOP			'^'
#define HK_TO_BOTTOM		('^'|0x80)	// SHIFT-^ (Pi)
#else
#define HK_TO_TOP			'{'
#define HK_TO_BOTTOM		'}'
#endif
#ifdef __CBM__
#define HK_PAGE_UP			'+'
#define HK_PAGE_DOWN		'-'
#else
#define HK_PAGE_UP			'['
#define HK_PAGE_DOWN		']'
#endif
#define HK_SELECT			' '
#define HK_HELP				'h'
#define HK_DRIVE_COMMAND	'@'
#define HK_CREATE_D64		'x'
#define HK_BATCH_CREATE_D64	'b'
#define HK_WRITE_D64		'X'	// SHIFT-X
#define HK_PATH				'p'
#define HK_ORIENTATION		'i'

#ifdef __C128__
#define HK_HELP_128			0x84	// HELP key
#define HK_GO64				'g'
#endif



//extern bool arePanelsOn;
extern unsigned char buffer[47];	// (must be long enough to hold configuration file)

#ifdef COLOR_RED
extern unsigned char outsideFrame;
extern unsigned char outsideScreen;
extern unsigned char outsideText;
#endif

void initialize(void);
#endif
