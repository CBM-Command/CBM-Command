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

#ifdef __CBM__
#include <cbm.h>
#endif
#include <conio.h>
#include <stdarg.h>
#include <stdbool.h>
//#include <string.h>
//#include <stdlib.h>
//#include <stdio.h>

#include "Configuration.h"
//#include "constants.h"
//#include "drives.h"
#include "globalInput.h"
#include "globals.h"
#include "screen.h"

static char key;

#if defined(__C128__) || defined(__PET__)
char getKey(void)
{
	key = cgetc();
	if (key == CH_ESC) return CH_STOP;
	return key;
}
#endif

char waitForEnterEsc(void)
{
	revers(true);
	(void)textcolor(color_text_highlight);
	cputsxy(size_x - 5, 0, "(RET)");
	//revers(false);

	do
	{
		key = getKey();
	}
	while (key != CH_ENTER
		&& key != CH_STOP);

	//writeCurrentFilename(selectedPanel);
	writeStatusBar("");
	return key;
}

char waitForEnterEscf(const char* format, ...)
{
	va_list ap;

	va_start(ap,  format);
	vwriteStatusBarf(format, ap);
	va_end(ap);

	return waitForEnterEsc();
}
