;
; 1998-05-31, Ullrich von Bassewitz
; 2012-09-10, Greg King
;
; This version favors small over fast.
;
; char* __fastcall__ strcat (char* dest, const char* src);
;

	.export		_strcat

	.import		popax
	.importzp	ptr1, ptr2, tmp3

	.macpack	cpu

_strcat:
	sta	ptr1		; Save src
	stx	ptr1+1
	jsr	popax		; Get dest
	sta	ptr2
	stx	ptr2+1
	sta	tmp3		; Remember for function return
	ldy	#0

; find end of dest

sc1:
.if	.cpu & CPU_ISET_65SC02
	lda	(ptr2)
.else
	lda	(ptr2),y
.endif
	beq	sc4
	inc	ptr2
	bne	sc1
	inc	ptr2+1
	bne	sc1

; end found, copy src

sc4:	lda	(ptr1),y
	sta	(ptr2),y
	beq	sc5
	iny
	bne	sc4
	inc	ptr1+1
	inc	ptr2+1
	bne	sc4

; done, return pointer to dest

sc5:	lda	tmp3		; X does still contain high byte
	rts


