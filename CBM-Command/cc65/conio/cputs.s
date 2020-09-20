;
; 1998-08-06, Ullrich von Bassewitz
; 2012-09-10, Greg King
;
; void __fastcall__ cputsxy (unsigned char x, unsigned char y, char* s);
; void __fastcall__ cputs (char* s);
;

	.export		_cputsxy, _cputs

	.import		callgoto, _cputc
	.importzp	ptr1

	.macpack	cpu

.define bze beq
.define bnz bne

_cputsxy:
	stx	ptr1+1		; Save s for later
	jsr	callgoto	; Set cursor
;	jmp	L0		; Same as cputs...
	.byte	$2C		;(bit $1234) Warning! This might hit I/O chips on non-CBM targets

_cputs:	stx	ptr1+1		; Save s
L0:	sta	ptr1
L1:
.if	.cpu & CPU_ISET_65SC02
	lda	(ptr1)
.else
	ldy	#0
	lda	(ptr1),y
.endif
	bze	L9		; Jump if done
	jsr	_cputc		; Output char, advance cursor
	inc	ptr1		; Bump low byte
	bnz	L1		; Next char
	inc	ptr1+1		; Bump high byte
	bnz	L1

; Done

L9:	rts


