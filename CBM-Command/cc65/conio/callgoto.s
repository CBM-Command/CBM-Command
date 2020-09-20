;
; Several CONIO functions start at a new cursor position by calling this function.
;
; 2012-09-10, Greg King
;
	.export	callgoto

	.import	popa, _gotoxy

callgoto:
	pha			; Save the accumulator
	jsr	popa		; Get y
	jsr	_gotoxy		; Get x, and move the cursor
	pla			; Restore the accumulator
	rts
