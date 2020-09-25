;
; 2003-04-09, Ullrich von Bassewitz
; 2020-09-22, Greg King
; Using lots of code from MagerValp, MagerValp@cling.gu.se
;
; void __fastcall__ cputcxy (unsigned char x, unsigned char y, char c);
; void __fastcall__ cputc (char c);
;

	.export		_cputcxy, _cputc
	.export		cputdirect, putchar, newline, plot

	.import		callgoto
	.import		PLOT, NEWLINE, PRINT

	.include	"c128.inc"

newline	:=	NEWLINE

;--------------------------------------------------------------------------

_cputcxy:
	jsr	callgoto	; Set cursor

; Plot a character -- also used as and internal function.

_cputc:	cmp	#$0D		; LF?
	bne	L2
	jmp	NEWLINE		; Update cursor position

L2:	cmp	#$0A		; CR?
	beq	cr		; Output a cr

; Printable char., of some sort.
; Convert it from PETSCII into a screen-code.
; (Note: This new method combines the fast speed of table look-up
;  with the small size of the computation method.  I call it
;  "folded-table look-up." -GK)

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

; Output one character to the screen. We will disable scrolling while doing so.

cputdirect:
	tax			; Save output char
	lda	SCROLL
	pha			; Save scroll flag
	lda	#$C0
	sta	SCROLL		; Disable scrolling
	txa			; Restore output char
	jsr	PRINT
	pla
	sta	SCROLL		; Restore old scroll flag
	rts

; Carriage return

cr:	lda	#0
	sta	CURS_X

; Set the cursor position, calculate RAM pointers.

plot:	ldy	CURS_X
	ldx	CURS_Y
	clc
	jmp	PLOT		; Set the new cursor

; Write one character to the screen without doing anything else, return the x
; position in .Y

putchar	:=	$CC2F

.rodata

pet_to_screen:
	.byte %10000000,%00000000,%01000000,%00100000  ; PETSCII -> screen-code
	.byte %01000000,%11000000,%10000000,%10000000

