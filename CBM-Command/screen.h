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

#ifndef _SCREEN_H
#define _SCREEN_H

#include <stdbool.h>
#include <stdarg.h>
#include "globals.h"

enum results { OK_RESULT, CANCEL_RESULT, YES_RESULT, NO_RESULT };
enum buttons { OK = 1, CANCEL = 2, YES = 4, NO = 8 };	// bit-masks
enum orientations { ORIENT_VERT, ORIENT_HORIZ };

extern enum orientations screenOrientation;	// direction of dividing line

extern bool isDoubleBuffered;
void beginDoubleBuffer(void);
void endDoubleBuffer(void);

void setupScreen(void);
void swapOrientation(void);
void __fastcall writeStatusBar(const char[]);
void __fastcall vwriteStatusBarf(const char[], va_list);
void writeStatusBarf(const char[], ...);

void __fastcall writePanel(
	bool drawBorder,
	bool reverse,
	unsigned char color,
	unsigned char x, unsigned char y,
	unsigned char h, unsigned char w,
	const char *title,
	const char *cancel,
	const char *ok);

void __fastcall drawBox(
	unsigned char x, unsigned char y,
	unsigned char w, unsigned char h,
	unsigned char color, bool reverse);

enum results __fastcall drawDialog(
	const char* const message[],
	unsigned char lineCount,
	const char* title,
	enum buttons button);

bool __fastcall writeYesNo(
	const char *title,
	const char *const message[],
	unsigned char lineCount);

enum results __fastcall drawInputDialog(
	unsigned char lineCount,
	unsigned char length,
	const char *const message[],
	const char *title,
	char *resultText);

void __fastcall drawProgressBar(
	const char* message,
	unsigned int currentValue,
	unsigned int maxValue);

void saveScreen(void);
void retrieveScreen(void);

//void notImplemented(void);

unsigned char __fastcall getCenterX(unsigned char w);
unsigned char __fastcall getCenterY(unsigned char h);
#endif
