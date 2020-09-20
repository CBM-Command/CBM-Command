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

//#include <string.h>
#include <conio.h>
#ifdef __CBM__
#include <cbm.h>
#endif

#include "Configuration.h"
//#include "constants.h"
#include "drives.h"
#include "globals.h"
#include "screen.h"
//#include "PlatformSpecific.h"

//unsigned char size_x, size_y;

//bool arePanelsOn = true;
unsigned char buffer[47];	// (must be long enough to hold configuration file)

#ifdef COLOR_RED
unsigned char outsideFrame;
unsigned char outsideScreen;
unsigned char outsideText;
#endif

// Obtains the screen's attributes;
// and, sets up global values, as necessary.
void initialize(void)
{
	//screensize(&size_x, &size_y);

// Save the original colors if a platform supports colors.
#ifdef COLOR_RED
	outsideFrame = bordercolor(0);
	outsideScreen = bgcolor(color_background);
	outsideText = textcolor(0);
#ifdef __C128__
	// Work around a bug in the C128's CONIO VDC-color functions.
	// They usually return a value that's one higher than it should be.
	if (textcolor(0) != 0)		// do it if bug hasn't been fixed
	{
	if (outsideScreen != COLOR_BLACK) --outsideScreen;
	--outsideText;
	}
#endif
#endif

	// Loads the configuration from
	// the drive from which the
	// application was started.
	load();

	// Prepares the screen for use.
	setupScreen();
}
