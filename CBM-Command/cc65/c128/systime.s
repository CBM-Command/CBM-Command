;
; 2009-7-27, Stefan Haubenthal
; 2012-09-5, Greg King
;
; time_t _systime (void);
; /* Similar to time(), but:
;  *   - Is not ISO C;
;  *   - Does not take the additional pointer;
;  *   - Does not set errno when returning -1.
;  */
;

	.include	"time.inc"
	.include	"c128.inc"
	.include	"get_tv.inc"

	.constructor	initsystime
	.importzp	sreg, tmp1, tmp2
	.import		pusheax, tosadd0ax, tosumul0ax

	.macpack	generic

;----------------------------------------------------------------------------

.proc	__systime
	ldx	#0
	stx	sum
	stx	sum+1
	stx	sreg
	stx	sreg+1
	lda	CIA1_TODHR
	bpl	AM
	and	#<~%10000000
	sed
	add	#$12
	cld
AM:	jsr	BCD2sum
	lda	CIA1_TODMIN
	jsr	BCD2sum
	lda	CIA1_TODSEC
	bit	CIA1_TOD10				; Dummy read to unfreeze

; sum = sum * 60 + time
BCD2sum:
	pha
	lda	sum
	ldx	sum+1
	jsr	pusheax
	lda	#<60
	ldx	#>60
	jsr	tosumul0ax
	jsr	pusheax
	pla

; dec = (((BCD >> 4) * 10) + (BCD & 0xf))
BCD2dec:
	tax
	and	#$0f
	sta	tmp1
	txa
	and	#$f0		; *16
	lsr	a			; *8
	sta	tmp2
	lsr	a
	lsr	a			; *2
	adc	tmp2		; = *10
	adc	tmp1

; sum += time
	ldx	#>0
	jsr	tosadd0ax
	sta	sum
	stx	sum+1
	rts
.endproc

;----------------------------------------------------------------------------
; Constructor that writes to the 1/10 sec. register of the TOD, to kick it
; into action. If that is not done, the clock hangs. We will read the register;
; and, write it again, ignoring a possible change in between.
.segment	"INIT"

.proc	initsystime
	lda	CIA1_TOD10
	sta	CIA1_TOD10
	jsr	_get_tv
	cmp	#TV::PAL
	bne	@60Hz
	lda	CIA1_CRA
	ora	#%10000000
	sta	CIA1_CRA
@60Hz:
	rts
.endproc

;----------------------------------------------------------------------------
.bss

sum:	.res	2
