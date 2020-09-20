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

#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include <conio.h>

#ifndef NULL
#define NULL 0
#endif

/* Drive Constants */
#define SLIDING_WINDOW_SIZE	30		// Thirty directory entries are cached
#define COPY_BUFFER_SIZE	1018	// The size of 4 CBM blocks, plus 2 bytes

/* Count the number of cells in an array. */
#define A_SIZE(a) (sizeof(a)/sizeof(a)[0])

/* Keyboard Constants */
#define KEY_AT				'@'
#ifdef __CBM__
//#define KEY_F1				0x85
//#define KEY_F3				0x86
//#define KEY_F5				0x87
//#define KEY_F7				0x88
//#define KEY_F2				0x89
//#define KEY_F4				0x8A
//#define KEY_F6				0x8B
//#define KEY_F8				0x8C

#define KEY_SH_SPACE		(' '|0x80)
#define KEY_SH_RETURN		(CH_ENTER|0x80)
#define KEY_SH_PLUS			0xDB
#define KEY_SH_MINUS		0xDD
#define KEY_ALT_D			'D'	//0xAC
#define KEY_ALT_E			'E'	//0xB1
#endif

#ifndef CH_F1
// Pretend function-keys on Pet/CBM keyboards
#define CH_F1				'1'
#define CH_F2				'2'
#define CH_F3				'3'
#define CH_F4				'4'
#define CH_F5				'5'
#define CH_F6				'6'
#define CH_F7				'7'
#define CH_F8				'8'
#endif

//#define KEY_CTRL(c)			(c) //& 0x1F)

#define KEY_CTRL_A			'a' //KEY_CTRL('A')
#define KEY_CTRL_B			'b' //KEY_CTRL('B')
#define KEY_CTRL_C			'c' //KEY_CTRL('C')
#define KEY_CTRL_D			'd' //KEY_CTRL('D')
#define KEY_CTRL_E			'e' //KEY_CTRL('E')
#define KEY_CTRL_F			'f' //KEY_CTRL('F')
#define KEY_CTRL_G			'g' //KEY_CTRL('G')
#define KEY_CTRL_H			'h' //KEY_CTRL('H')
#define KEY_CTRL_I			'i' //KEY_CTRL('I')
#define KEY_CTRL_J			'j' //KEY_CTRL('J')
#define KEY_CTRL_K			'k' //KEY_CTRL('K')
#define KEY_CTRL_L			'l' //KEY_CTRL('L')
#define KEY_CTRL_M			'm' //KEY_CTRL('M')
#define KEY_CTRL_N			'n' //KEY_CTRL('N')
#define KEY_CTRL_O			'o' //KEY_CTRL('O')
#define KEY_CTRL_P			'p' //KEY_CTRL('P')
#define KEY_CTRL_Q			'q' //KEY_CTRL('Q')
#define KEY_CTRL_R			'r' //KEY_CTRL('R')
#define KEY_CTRL_S			's' //KEY_CTRL('S')
#define KEY_CTRL_T			't' //KEY_CTRL('T')
#define KEY_CTRL_U			'u' //KEY_CTRL('U')
#define KEY_CTRL_V			'v' //KEY_CTRL('V')
#define KEY_CTRL_W			'w' //KEY_CTRL('W')
#define KEY_CTRL_X			'x' //KEY_CTRL('X')
#define KEY_CTRL_Y			'y' //KEY_CTRL('Y')
#define KEY_CTRL_Z			'z' //KEY_CTRL('Z')

#endif
