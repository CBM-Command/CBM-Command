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

.importzp	sp
.import		pusha, incsp2
.macpack	generic

.if 0
.export	_writeVdcRegister
.export	_readVdcRegister
.export	_clearVdcScreen
.endif
.export	_copyVdcScreen	; void __fastcall copyScreen(
						;		unsigned char srcHi,
						;		unsigned char trgHi)

;.include	"c128.inc"

; ---------------------------------------------------------------------------

	.struct	VDC			; Video Display Controller
		.res	$D600
CTRL	.byte
DATA	.byte
	.endstruct

	.struct	BIG_END
HI		.byte
LO		.byte
	.endstruct

	.struct	CRTC		; Cathode-Ray Tube Controller
		.byte	12
DSPLY	.tag	BIG_END
		.byte	4
ADDR	.tag	BIG_END
		.byte	4
VSCROLL	.byte
HSCROLL	.byte
COLORS	.byte
		.byte
CSET	.byte
		.byte
COUNT	.byte
DATA	.byte
	.endstruct

; ---------------------------------------------------------------------------

.if 0
READ:	STA VDC::CTRL	; Put the value into the register
RVR1:	BIT VDC::CTRL	; Compare the top bit of the register
		BPL RVR1		; Branch if the top bit is low (0)
		LDA VDC::DATA	; Get the result into the accumulator
		RTS
.endif

WRITE:	STX VDC::CTRL	; Put the value into the register
WVR1:	BIT VDC::CTRL	; Compare the top bit of the register
		BPL WVR1		; Branch if the top bit is low (0)
		STA VDC::DATA	; Store the accumulator in the register's value
		RTS

.proc _copyVdcScreen
		JSR pusha		; Store the accumulator

		; Write target address
		LDY #$00		; Offset for second of two parameters
		LDA (sp),y		; Load accumulator with parameter value
		LDX #CRTC::ADDR
		JSR WRITE		; Writes accumulator to register 18
		LDA #<$00		; Low bytes are at location 00
		INX				; Next register
		JSR WRITE		; Writes accumulator to register 19

		; Issue block-copy command
		LDX #CRTC::VSCROLL	; Get value of register 24
		LDA #%10000000		; Set bit 7 high
		JSR WRITE			; Write register 24

		; Write source address
		LDY #$01		; Offset for first of two parameters
		LDA (sp),y		; Load accumulator with parameter value
		LDX #32
		JSR WRITE		; Writes accumulator to register 32
		LDA #<$00		; Low bytes are at location 00
		INX				; Next register
		JSR WRITE		; Writes accumulator to register 33

		; Set block size
		LDX #CRTC::COUNT	; block-size register (30)
		LDA #<256			; Full block-size of 256 bytes
		JSR WRITE

		; Loop until executed 16 times (16 * 256 = 4096).
		LDY #$10		; Copy 16 blocks
CLRPAG:	JSR WRITE
		DEY
		BNE CLRPAG

		; We're done, clean up
		JMP incsp2
.endproc

.if 0
.proc _clearVdcScreen
		JSR pusha		; Store the accumulator

		; Set the start location
		LDY #$01		; Offset for first of two parameters
		LDA (sp),y		; Load accumulator with parameter value
		LDX #18
		JSR WRITE		; Writes accumulator to register 18

		LDY #$00		; Offset for the second of two parameters
		LDA (sp),y		; Store the parameter in the accumulator
		INX				; Now we are on register 19
		JSR WRITE		; Write the accumulator to register 19

		; Set address to clear
		LDX #18
		LDA #00
		JSR WRITE
		INX
		JSR WRITE

		; Clear the copy bit && Reverse Bit
		LDX #24			; Located in register 24
		;JSR READ		; Read it to accumulator
		;AND #$7F		; OR the accumulator
		LDA #$00
		JSR WRITE		; Write it back

		; Set data byte
		LDX #31			; Located in register 31
		JSR WRITE		; Set the register

		LDX #30			; Word count register?
		LDA #$00
		JSR WRITE

		LDY #$10
		LDA #00
CLRPAG:	JSR WRITE
		DEY
		BNE CLRPAG

		JSR incsp2
		RTS
.endproc

;-------------------------
; Read .A from VDC Register named by .A
_readVdcRegister := READ

;-------------------------
; Write .A to VDC Register named by .X
.proc _writeVdcRegister
		TAX				; Copy accumulator to .X

		LDY #$00		; Offset for the first of two parameters
		LDA (sp),y		; Store the parameter in the accumulator

		JSR WRITE
		JMP	incsp1		; Finish our function
.endproc
.endif
