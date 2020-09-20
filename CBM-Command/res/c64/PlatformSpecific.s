;/***************************************************************
;Copyright (c) 2010, Payton Byrd
;All rights reserved.
;
;Redistribution and use in source and binary forms, with or
;without modification, are permitted provided that the following
;conditions are met:
;
;* Redistributions of source code must retain the above
;  copyright notice, this list of conditions and the following
;  disclaimer.
;
;* Redistributions in binary form must reproduce the above
;  copyright notice, this list of conditions and the following
;  disclaimer in the documentation and/or other materials
;  provided with the distribution.
;
;* Neither the name of Payton Byrd nor the names of its
;  contributors may be used to endorse or promote products
;  derived from this software without specific prior written
;  permission.
;
;THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
;CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
;INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
;MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
;DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
;CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
;SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
;NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
;LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
;HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
;CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
;OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
;EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;***************************************************************/

		.export		_saveScreen, _retrieveScreen
		.import		pushax, _memcpy

		.include	"../macroes.inc"
		.include	"c64.inc"

SCREEN_SIZE		=	XSIZE * YSIZE

R_6510			:=	$01
HIBASE			:=	$0288		; holds high-byte of screen address
SCREEN_BUFFER	:=	$F000		; this is under the Kernal ROM

COLOR_RAM		:=	$D800
COLOR_BUFFER	:=	SCREEN_BUFFER + 1024


; ---------------------------------------------------------------
; void __near__ saveScreen (void)
; ---------------------------------------------------------------

.proc	_saveScreen: near

;
; memcpy(COLOR_BUFFER, COLOR_RAM, SCREEN_SIZE);
;
		ldxa	#COLOR_BUFFER
		jsr		pushax
		ldxa	#COLOR_RAM
		jsr		pushax
		ldxa	#SCREEN_SIZE
		jsr		_memcpy
;
; memcpy(SCREEN_BUFFER, SCREEN, SCREEN_SIZE);
;
		ldxa	#SCREEN_BUFFER
		jsr		pushax
		ldx		HIBASE
		lda		#<$00
		jsr		pushax
		ldxa	#SCREEN_SIZE
		jmp		_memcpy

.endproc


; ---------------------------------------------------------------
; void __near__ retrieveScreen (void)
; ---------------------------------------------------------------

.proc	_retrieveScreen: near

;
; memcpy(COLOR_RAM, COLOR_BUFFER, SCREEN_SIZE);
;
		ldxa	#COLOR_RAM
		jsr		pushax
		ldxa	#COLOR_BUFFER
		jsr		pushax
		lda		R_6510
		php
		pha
		eor		#HIRAM | LORAM	; hide Kernal ROM, but keep color RAM visible
		sei						; disable interrupts before changing memory map
		sta		R_6510
		ldxa	#SCREEN_SIZE
		jsr		_memcpy
;
; memcpy(SCREEN, SCREEN_BUFFER, SCREEN_SIZE);
;
		ldx		HIBASE
		lda		#<$00
		jsr		pushax
		ldxa	#SCREEN_BUFFER
		jsr		pushax
		ldxa	#SCREEN_SIZE
		jsr		_memcpy
		pla
		sta		R_6510			; re-enable Kernal ROM
		plp						; re-enable interrupts
		rts

.endproc
