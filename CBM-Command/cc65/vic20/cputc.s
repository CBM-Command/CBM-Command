;
; 2003-04-09, Ullrich von Bassewitz
; 2013-04-07, Greg King
;
; void __fastcall__ cputcxy (unsigned char x, unsigned char y, char c);
; void __fastcall__ cputc (char c);
;

	.export		_cputcxy, _cputc
	.export		cputdirect, putchar, newline, plot

	.import		callgoto
	.import		PLOT

	.macpack	generic
	.include	"vic20.inc"


_cputcxy:
	jsr	callgoto	; Set cursor

; Plot a character -- also used as an internal function.

_cputc:	cmp	#$0D		; Is it CBM '\n'?
	beq	newline		; Recalculate pointers
	cmp	#$0A		; Is it CBM '\r'?
	beq	cr

; Printable char., of some sort.
; Convert it from PetSCII into a screen-code.

	cmp	#$FF		; BASIC token
	bne	convert
	lda	#$DE		; Pi symbol
convert:
	tay
	lsr	a		; Divide by 32
	lsr	a
	lsr	a
	lsr	a
	lsr	a
	tax			; .X = %00000xxx
	tya
	eor	pet_to_screen,x

cputdirect:
	jsr	putchar		; Write the character to the screen

; Advance the cursor position

advance:
	iny
	cpy	#XSIZE
	bne	L3
	jsr	newline		; new line
				; + CR
; Carriage-return

cr:	ldy	#0
L3:	sty	CURS_X
	rts


; Move down by one full screen-line.  Note: this routine doesn't scroll.
;
; (Both screen RAM and color RAM are aligned to page boundaries.
; So, the lower bytes of their addresses have the same values.
; Shorten the code by taking advantage of that fact.)

newline:
	lda	SCREEN_PTR
	add	#XSIZE
	sta	SCREEN_PTR
	sta	CRAM_PTR
	bcc	L5
	inc	SCREEN_PTR+1
	inc	CRAM_PTR+1
L5:	inc	CURS_Y
	rts


; Set cursor position, calculate RAM pointers

plot:	ldy	CURS_X
	ldx	CURS_Y
	clc
	jmp	PLOT		; Set the new cursor


; Write one character to the screen without doing anything else, return the x
; position in .Y

putchar:
	eor	RVS		; Set reverse-bit
	ldy	CURS_X
	sta	(SCREEN_PTR),y	; Set char
	lda	CHARCOLOR
	sta	(CRAM_PTR),y	; Set color
	rts


	.rodata
pet_to_screen:
	.byte %10000000,%00000000,%01000000,%00100000  ; PetSCII -> screen-code
	.byte %01000000,%11000000,%10000000,%10000000
