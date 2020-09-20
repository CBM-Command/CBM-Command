;
; 1998-08-08, Ullrich von Bassewitz
; 2012-09-10, Greg King
;
; void __fastcall__ chlinexy (unsigned char x, unsigned char y, unsigned char length);
; void __fastcall__ chline (unsigned char length);
;

    	.export		_chlinexy, _chline
	.import		callgoto, cputdirect
	.importzp	tmp1

_chlinexy:
       	jsr    	callgoto	; Set cursor

_chline:
   	cmp	#0		; Is the length zero?
   	beq	L9  		; Jump if done
	sta	tmp1
L1:    	lda    	#64 		; Horizontal line, screen code
   	jsr	cputdirect	; Direct output
   	dec	tmp1
	bne	L1
L9:	rts


