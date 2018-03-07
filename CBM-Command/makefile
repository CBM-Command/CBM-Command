# Program declarations
CC65_PATH = ~/.local/bin
CC := $(CC65_PATH)/cc65
CA := $(CC65_PATH)/ca65
LD := $(CC65_PATH)/ld65
CL := $(CC65_PATH)/cl65
AR := $(CC65_PATH)/ar65

# POSIX-style programs
CHMOD := chmod +w
CP := cp
#MKDIR := mkdir.exe

# VICE programs
#VICEPATH := C:\WinVICE-2.4\WinVICE-2.4.28-x86-r31264
VICEPATH := /usr/local/bin
C1541 := $(VICEPATH)/c1541
X64 := $(VICEPATH)/x64 -autostart
X128 := $(VICEPATH)/x128 -autostart
XPET := $(VICEPATH)/xpet -autostart
XVIC := $(VICEPATH)/xvic -autostart
XPLUS4 := $(VICEPATH)/xplus4 -autostart

T := c128

VIC_CONFIG := vic20-32k.cfg

ifeq "$(T)" "vic20"
LD_CONFIG := -C $(VIC_CONFIG)
endif

CONFIG_SOURCES := \
	Configuration.c \
	ConfigurationManager.c \
	globalInput.c \
	globals.c \
	screen.c

SOURCES	:= \
	CBM-Command.c \
	CBM-REL.c \
	Configuration.c \
	DriveMenu.c \
	drives.c \
	FileMenu.c \
	globalInput.c \
	globals.c \
	input.c \
	menus.c \
	screen.c \
	Viewer.c \
	builtStamp.c
# builtStamp.c should be last in the list.

ASMFILES := \
	res/common/driveRoutines.s \
	res/$(T)/PlatformSpecific.s \
	cc65/common/_printf.s \
	${wildcard cc65/$(T)/systime.s}
# _systime() is a library function; but, it's called by only another library
# function -- in only some of the programs.  Therefore, its substitute
# must be linked directly.  [If it was in cc65.lib, then ld65 would pass by it
# before time() asked for it.]

# These functions are called by the programs' files.
# Their substitutes can be put into a library.
LIBFILES := \
	cc65/common/strcat.s \
	cc65/cbm/cbm_dir.c \
	cc65/cbm/cbm_open.s \
	cc65/cbm/cclear.s \
	cc65/cbm/chline.s \
	cc65/cbm/cvline.s \
	${wildcard cc65/$(T)/clrscr.s} \
	cc65/$(T)/cputc.s \
	cc65/conio/cputs.s \
	cc65/conio/callgoto.s

#DEBUG := -T --check-stack
CFLAGS := -Wl #-D,__STACKSIZE__=$$400
CFLAGS1 := $(CFLAGS) $(DEBUG) #-m $(T)/cmd.map
CFLAGS2 := $(CFLAGS) #-m $(T)/cfg.map

TITLE := cbmcommand
CONFIG_TITLE := cbmcmdcfg

PROGRAM := $(TITLE).$(T)
CONFIG_PROGRAM := $(CONFIG_TITLE).$(T)

LIBS := $(T)/cc65.lib

# Default target -- It builds for the C128.
.PHONY: all
ifeq "$(T)" "pet"
all: pet/$(PROGRAM)40 pet/$(PROGRAM)80 pet/$(CONFIG_PROGRAM)40 pet/$(CONFIG_PROGRAM)80
else
all: $(T)/$(PROGRAM) $(T)/$(CONFIG_PROGRAM)
endif

# Launch VICE
.PHONY: vice
ifeq "$(T)" "pet"
vice: clean pet/$(TITLE).pet80
		$(XPET) pet/$(TITLE).pet80
#		$(XPET) pet/$(TITLE).pet40
else
vice: clean $(T)/$(PROGRAM)
ifeq "$(T)" "c64"
		$(X64) $(T)/$(PROGRAM)
endif
ifeq "$(T)" "c128"
		$(X128) $(T)/$(PROGRAM)
endif
ifeq "$(T)" "vic20"
		$(XVIC) $(T)/$(PROGRAM)
endif
ifeq "$(T)" "plus4"
		$(XPLUS4) $(T)/$(PROGRAM)
endif
endif

.PHONY: D64 D80
D64: all
	$(RM) $(TITLE).d64
	$(CP) $(TITLE)_master.d64 $(TITLE).d64
	$(CHMOD) $(TITLE).d64

	$(C1541) $(TITLE).d64 -write c64/$(TITLE).c64 $(TITLE).c64
	$(C1541) $(TITLE).d64 -write c128/$(TITLE).c128 $(TITLE).c128
	$(C1541) $(TITLE).d64 -write vic20/$(TITLE).vic20 $(TITLE).vic20
	$(C1541) $(TITLE).d64 -write plus4/$(TITLE).plus4 $(TITLE).plus4

	$(C1541) $(TITLE).d64 -write c64/$(CONFIG_TITLE).c64 $(CONFIG_TITLE).c64
	$(C1541) $(TITLE).d64 -write c128/$(CONFIG_TITLE).c128 $(CONFIG_TITLE).c128
	$(C1541) $(TITLE).d64 -write vic20/$(CONFIG_TITLE).vic20 $(CONFIG_TITLE).vic20
	$(C1541) $(TITLE).d64 -write plus4/$(CONFIG_TITLE).plus4 $(CONFIG_TITLE).plus4

D80: all
	$(RM) $(TITLE).d80
	$(CP) $(TITLE)_master.d80 $(TITLE).d80
	$(CHMOD) $(TITLE).d80

	$(C1541) $(TITLE).d80 -write pet/$(TITLE).pet40 -write pet/$(TITLE).pet80
#	$(C1541) $(TITLE).d80 -write c64/$(TITLE).c64 -write c128\$(TITLE).c128
#	$(C1541) $(TITLE).d80 -write vic20/$(TITLE).vic20 -write plus4\$(TITLE).plus4

	$(C1541) $(TITLE).d80 -write pet/$(CONFIG_TITLE).pet40 -write pet/$(CONFIG_TITLE).pet80
#	$(C1541) $(TITLE).d80 -write c64/$(CONFIG_TITLE).c64 -write c128\$(CONFIG_TITLE).c128
#	$(C1541) $(TITLE).d80 -write vic20/$(CONFIG_TITLE).vic20 -write plus4\$(CONFIG_TITLE).plus4

# Build our programs
$(T)/$(PROGRAM) pet/$(PROGRAM)40: $(SOURCES) $(ASMFILES) $(LIBS)
		$(CL) $(CFLAGS1) -v -O -t $(T) $(LD_CONFIG) -o $@ $+

pet/$(PROGRAM)80: $(SOURCES) $(ASMFILES) $(LIBS)
		$(CL) -D _PET80_ $(CFLAGS1) -Or -t pet -o $@ $+

$(T)/$(CONFIG_PROGRAM) pet/$(CONFIG_PROGRAM)40: $(CONFIG_SOURCES) res/$(T)/PlatformSpecific.s $(LIBS)
		$(CL) $(CFLAGS2) -v -O -t $(T) $(LD_CONFIG) -o $@ $+

pet/$(CONFIG_PROGRAM)80: $(CONFIG_SOURCES) res/pet/PlatformSpecific.s $(LIBS)
		$(CL) -D _PET80_ $(CFLAGS2) -Or -t pet -o $@ $+

$(LIBS): $(LIBFILES)
		$(CL) -Or -t $(T) -c $?
		$(AR) a $@ $(addsuffix .o,${basename $?})

# Our cleaning targets, for managing the project
.PHONY: mostly-clean clean zap
mostly-clean:
		$(RM) $(SOURCES:.c=.o) $(SOURCES:.c=.s)
		$(RM) $(CONFIG_SOURCES:.c=.o) $(CONFIG_SOURCES:.c=.s)
		$(RM) $(ASMFILES:.s=.o)
		$(RM) $(addsuffix .o,${basename $(LIBFILES)}) $(LIBS)

clean: mostly-clean
ifeq "$(T)" "pet"
		$(RM) ${wildcard pet/$(TITLE).pet* pet/$(CONFIG_TITLE).pet*}
else
		$(RM) $(T)/$(PROGRAM) $(T)/$(CONFIG_PROGRAM)
endif

zap: clean
	$(RM) $(TITLE).d64 $(TITLE).d80
