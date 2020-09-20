;
; 1998-08-08, Ullrich von Bassewitz
; 2012-09-10, Greg King
;
; void __fastcall__ cvlinexy (unsigned char x, unsigned char y, unsigned char length);
; void __fastcall__ cvline (unsigned char length);
;

    	.export		_cvlinexy, _cvline

	.import		callgoto, putchar, newline
	.importzp	tmp1

_cvlinexy:
       	jsr    	callgoto	; Set cursor

_cvline:
   	cmp	#0		; Is the length zero?
   	beq	L9  		; Jump if done
    	sta	tmp1
L1:	lda	#93 		; Vertical bar
   	jsr	putchar		; Write, no cursor advance
   	jsr	newline		; Advance cursor to next line
	dec	tmp1
	bne	L1
L9:	rts


