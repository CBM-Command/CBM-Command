## What is it?

**CBM-Command** is a disk manager for the Commodore 64, Commodore 128, Commodore VIC-20, Commodore Plus/4, and Commodore Pet/CBM computers. It is written in the vein of Norton Commander or Midnight Commander; but, it is much simpler due to the target platforms. Each Commodore model has its own native version of the application.

## What does it do?

It works on both real machines and the VICE emulators.

So far, we have tested it with the following hardware:

*   Flat Commodore 128 w/JiffyDOS
*   Commodore 1571 w/JiffyDOS
*   Commodore 1581 w/JiffyDOS
*   uIEC/SD
*   Commodore 64 (stock)
*   Commodore 64 w/JiffyDOS
*   Commodore 64 w/SuperCPU
*   Commodore 1541 (stock)
*   Commodore 1541 w/JiffyDOS
*   Blue Chip 128 Disk Drive
*   Creative Micro Designs FD-2000 Floppy Disk drive
*   ZoomFloppy USB-to-IEC adapter (w/ WinVICE and OpenCBM)

Additional testing has been performed under VICE, using the following file systems:

*   IDE64 Native Filesystem
*   VICE File System
*   VICE Virtual Drives (not True Drive Emulation)
*   1541 TDE
*   1551 TDE
*   1571 TDE
*   1581 TDE
*   CBM 8050 TDE
*   CBM 8250 TDE
*   Commodore Plus 4 Emulation
*   Commodore VIC 20 Emulation
*   Commodore 64 Emulations
*   Commodore 128 Emulation
*   Commodore Pet Emulation

## Features

*   File copying between drives
*   File renaming and deleting
*   D64 Image Writing, and Creation of Disks from Those Images
*   D71 Image Writing, and Creation of Disks from Those Images
*   D81 Image Writing, and Creation of Disks from Those Images
*   Send command to drive
*   Simple "Paging" Text-File Viewer
*   Moving into, and out of, sub-directories and disk-images
*   Configuration Utility
*   Whole Disk Copying

## Development tools

**CBM-Command** has been developed with the following tools:

*   CC65 [link](https://cc65.github.io/) -- C and Assembly compilers with linker, for building 6502-based binaries.
*   GNU Make [link](http://www.gnu.org/software/make/) -- Makefile processor which builds the sources, using CC65.
*   Visual Studio 2010 [link](http://blogs.msdn.com/danielfe/archive/2009/10/21/download-visual-studio-2010-express-editions.aspx) -- I suggest downloading the ISO with all of the express editions; but specifically, you need the C++ edition.
*   VICE [link](http://vice-emu.sf.net/ "VICE Home Page") -- The best Commodore emulators available.
*   CodePlex [link](http://www.codeplex.com/) -- Previous project management and source control.
*   Github [link](https://github.com/) -- Current project management and source control.

## HELP!

I'm looking for folks who want to adopt portions of the code, and help to make it better. If you are familiar with C and/or Assembly, or just want to get familiar with them (I had practically zero Assembly knowledge when I started this project), then contact me; and, we'll see if I can find something for you to do. :-)

# Screenshots

*   Removed old screenshots -- need to make new ones that are representative of the current code and platforms.