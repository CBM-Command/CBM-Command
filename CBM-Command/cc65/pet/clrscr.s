;
; Ullrich von Bassewitz, 26.11.1998
;
; void clrscr (void);
;

	.export		_clrscr

	.import		pusha, _gotoxy
	.importzp	ptr1

	.include	"pet.inc"
	.macpack	generic

_clrscr:

; Set the screen base address

	lda	#<$8000
	sta	ptr1
	lda	#>$8000
	sta	ptr1+1

; Determine, how many pages to fill

	ldx	#4
	lda	SCR_LINELEN	; Check length of one line
	cmp	#40+1
	blt	L1
	ldx	#8

; Clear the screen

L1:	lda	#$20		; Screen code for blank
	ldy	#$00
L2:	sta	(ptr1),y
	iny
	bne	L2
	inc	ptr1+1
	dex
	bne	L2

; Set the cursor to (0,0).

	txa			; lda #0
	jsr	pusha
	jmp	_gotoxy

