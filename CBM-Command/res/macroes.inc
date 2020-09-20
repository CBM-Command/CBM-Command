
.macro	ldxa	arg
.if (.match(.left(1, {arg}), #))
		; immediate mode
		lda		#<(.mid(1, 9, {arg}))
		ldx		#>(.mid(1, 9, {arg}))
.else
		; Assume absolute or zero-page addressing.
		lda		arg
		ldx		1+arg
.endif
.endmac

;.if 0
; These macroes help to build menues.

; "Choice" creates, exports, and counts one line for a menu.
;
;.macro	Choice	name, text
;.export	name
;name:	.asciiz	text
;
;	.ifndef MenuWidth
;MenuWidth	.set	0
;	.endif
;MenuWidth	.set	.max(MenuWidth, .sizeof(name) - 1)
;
;	.ifndef MenuHeight
;MenuHeight	.set	0
;	.endif
;MenuHeight	.set	MenuHeight + 1
;.endmac

; "HotChoice" creates one line for a menu, and makes a hotkey for it.
; The "hotkey" argument is optional.
;
;.macro	HotChoice	name, text, hotkey
;.export	.ident(.concat(.string(name), "_KEY"))
;.ident(.concat(.string(name), "_KEY")):
;	.ifblank hotkey
;	; The lower-case initial is the default hotkey.
;	.ifdef __CBM__
;	.byte	.strat(text, 0) & <~('a' ^ 'A')
;	.else
;	.byte	.strat(text, 0) |   ('a' ^ 'A')
;	.endif
;	.else
;	.byte	hotkey
;	.endif
;	Choice	name, {text}
;.endmac
;.endif
