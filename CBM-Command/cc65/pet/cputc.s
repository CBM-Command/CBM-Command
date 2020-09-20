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

	.macpack	generic
	.include	"pet.inc"

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

; Advance the cursor position.

advance:
	cpy	SCR_LINELEN	; xsize-1
	bne	L3
	jsr	newline		; new line
				; + CR
; Carriage-return

cr:	ldy	#<(0 - 1)
L3:	iny
	sty	CURS_X
	rts


; Move down by one full screen-line.  Note:  This routine doesn't scroll.

newline:
	lda	SCR_LINELEN	; xsize-1
	sec			; Account for -1 above
	adc	SCREEN_PTR
	sta	SCREEN_PTR
	bcc	L4
	inc	SCREEN_PTR+1
L4:	inc	CURS_Y
	rts


; Set cursor position, calculate RAM pointers

plot:	ldy	CURS_Y
	lda	ScrLo,y
	sta	SCREEN_PTR
	lda	ScrHi,y
	ldy	SCR_LINELEN
	cpy	#40+1
	blt	@L1
	asl	SCREEN_PTR	; 80-column screen
	rol	a
@L1:	ora	#>$8000		; Screen start
	sta	SCREEN_PTR+1
	rts


; Write one character to the screen without doing anything else, return the x
; position in .Y

putchar:
	eor	RVS		; Set reverse-bit
	ldy	CURS_X
	sta	(SCREEN_PTR),y	; Set char
	rts


.rodata

; PetSCII -> screen-code

pet_to_screen:
	.byte	%10000000,%00000000,%01000000,%00100000
	.byte	%01000000,%11000000,%10000000,%10000000

; Screen address tables -- offsets into the real screen

ScrLo:	.repeat	25, r
	.byte	<(r * 40)
	.endrep

ScrHi:	.repeat	25, r
	.byte	>(r * 40)
	.endrep


