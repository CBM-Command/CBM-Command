;
; 1998-08-08, Ullrich von Bassewitz
; 2012-09-10, Greg King
;
; void __fastcall__ cclearxy (unsigned char x, unsigned char y, unsigned char length);
; void __fastcall__ cclear (unsigned char length);
;

    	.export		_cclearxy, _cclear
	.import		callgoto, cputdirect
	.importzp	tmp1

_cclearxy:
       	jsr    	callgoto	; Set cursor

_cclear:
       	cmp	#0		; Is the length zero?
       	beq	L9  		; Jump if done
    	sta	tmp1
L1:    	lda    	#$20		; Blank - screen code
   	jsr	cputdirect	; Direct output
   	dec	tmp1
	bne	L1
L9:	rts


