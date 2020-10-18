# Tool declarations
#CC65_PATH := $(HOME)/.local/bin/
CC := $(CC65_PATH)cc65
CA := $(CC65_PATH)ca65
LD := $(CC65_PATH)ld65
CL := $(CC65_PATH)cl65
AR := $(CC65_PATH)ar65

# POSIX-style programs
CHMOD := chmod +w
CP := cp
MKDIR := mkdir -p
RMDIR := $(RM) -r

# VICE programs
#VICEPATH := /usr/local/bin/
C1541 := $(VICEPATH)c1541
X64 := $(VICEPATH)x64 -autostart
X128 := $(VICEPATH)x128 -autostart
XPET := $(VICEPATH)xpet -autostart
XVIC := $(VICEPATH)xvic -autostart
XPLUS4 := $(VICEPATH)xplus4 -autostart

T := c64

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
	cc65/common/_printf.s
# _printf() is a library function; but, it's called by only other library
# functions.  Therefore, its substitute must be linked directly.  [If it was
# in cc65.lib, then ld65 would pass by it before functions asked for it.]

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
	${wildcard cc65/$(T)/cputc.s} \
	cc65/conio/cputs.s \
	cc65/conio/callgoto.s

VIC_CONFIG := vic20-32k.cfg

ifeq "$(T)" "vic20"
LD_CONFIG := -C $(VIC_CONFIG) -Wl -D__STACKSIZE__=$$400
else
LD_CONFIG := -Wl -D__STACKSIZE__=$$400
endif

#DEBUG := -g -T --check-stack
CFLAGS1 := $(CFLAGS) $(DEBUG) #-g -vm -m $(T)/cmd.map
CFLAGS2 := $(CFLAGS) #-g -vm -m $(T)/cfg.map

TITLE := cbmcommand
CONFIG_TITLE := cbmcmdcfg

PROGRAM := $(TITLE).$(T)
CONFIG_PROGRAM := $(CONFIG_TITLE).$(T)

LIBS := $(T)/cc65.lib

.PHONY: all
all: $(addsuffix progs,c128 c64 pet plus4 vic20)

%progs:
	$(MAKE) progs T:=$*

ifeq "$(T)" "pet"
progs: pet/$(PROGRAM)40 pet/$(PROGRAM)80 pet/$(CONFIG_PROGRAM)40 pet/$(CONFIG_PROGRAM)80
else
progs: $(T)/$(PROGRAM) $(T)/$(CONFIG_PROGRAM)
endif

# Launch VICE
.PHONY: vice
ifeq "$(T)" "pet"
vice: clean pet/$(TITLE).pet80
	$(XPET) pet/$(TITLE).pet80
#	$(XPET) pet/$(TITLE).pet40
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

	$(C1541) $(TITLE).d64 -write c64/$(TITLE).c64 -write c128/$(TITLE).c128
	$(C1541) $(TITLE).d64 -write vic20/$(TITLE).vic20 -write plus4/$(TITLE).plus4
#	$(C1541) $(TITLE).d64 -write pet/$(TITLE).pet40 -write pet/$(TITLE).pet80

	$(C1541) $(TITLE).d64 -write c64/$(CONFIG_TITLE).c64 -write c128/$(CONFIG_TITLE).c128
	$(C1541) $(TITLE).d64 -write vic20/$(CONFIG_TITLE).vic20 -write plus4/$(CONFIG_TITLE).plus4
#	$(C1541) $(TITLE).d64 -write pet/$(CONFIG_TITLE).pet40 -write pet/$(CONFIG_TITLE).pet80

D80: all
	$(RM) $(TITLE).d80
	$(CP) $(TITLE)_master.d80 $(TITLE).d80
	$(CHMOD) $(TITLE).d80

	$(C1541) $(TITLE).d80 -write pet/$(TITLE).pet40 -write pet/$(TITLE).pet80
	$(C1541) $(TITLE).d80 -write c64/$(TITLE).c64 -write c128/$(TITLE).c128
	$(C1541) $(TITLE).d80 -write vic20/$(TITLE).vic20 -write plus4/$(TITLE).plus4

	$(C1541) $(TITLE).d80 -write pet/$(CONFIG_TITLE).pet40 -write pet/$(CONFIG_TITLE).pet80
	$(C1541) $(TITLE).d80 -write c64/$(CONFIG_TITLE).c64 -write c128/$(CONFIG_TITLE).c128
	$(C1541) $(TITLE).d80 -write vic20/$(CONFIG_TITLE).vic20 -write plus4/$(CONFIG_TITLE).plus4

# Build our programs
$(T)/$(PROGRAM) pet/$(PROGRAM)40: $(SOURCES) $(ASMFILES) $(LIBS) | $(T)
	$(CL) -Or -t $(T) $(CFLAGS1) $(LD_CONFIG) -o $@ $+

pet/$(PROGRAM)80: $(SOURCES) $(ASMFILES) $(LIBS) | $(T)
	$(CL) -D _PET80_ -Or -t pet $(CFLAGS1) $(LD_CONFIG) -o $@ $+

$(T)/$(CONFIG_PROGRAM) pet/$(CONFIG_PROGRAM)40: $(CONFIG_SOURCES) res/$(T)/PlatformSpecific.s $(LIBS) | $(T)
	$(CL) -Or -t $(T) $(CFLAGS2) $(LD_CONFIG) -o $@ $+

pet/$(CONFIG_PROGRAM)80: $(CONFIG_SOURCES) res/pet/PlatformSpecific.s $(LIBS) | $(T)
	$(CL) -D _PET80_ -Or -t pet $(CFLAGS2) $(LD_CONFIG) -o $@ $+

$(T):
	$(MKDIR) $@

$(LIBS): $(LIBFILES) | $(T)
	$(CL) -Or -t $(T) -c $?
	$(AR) a $@ $(addsuffix .o,${basename $?})

# Our cleaning targets, for managing the project
.PHONY: mostly-clean clean zap
mostly-clean:
	$(RM) $(SOURCES:.c=.s)
	$(RM) *.o $(CONFIG_SOURCES:.c=.s)
	$(RM) */*/*.o */cc65.lib

clean: mostly-clean
	$(RMDIR) c128 c64 pet plus4 vic20

zap: clean
	$(RM) $(TITLE).d64 $(TITLE).d80
