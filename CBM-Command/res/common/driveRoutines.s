;/***************************************************************
;Copyright (c) 2012, Payton Byrd
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

.import		SETLFS, SETNAM, OPEN, CLOSE, READST

.export		_checkDrive
;
; unsigned char __fastcall checkDrive(unsigned char drive);
;
.proc _checkDrive
		; Prepare for SETLFS.
		TAX					; .A -> .X (First address: device number)
		LDY		#15			; 15 -> .Y (Second address: channel)
		TYA					; .Y -> .A (Logical file number)
		JSR		SETLFS		; (15, drive, 15)

; The IEEE bus needs to send a command, in order to get results;
; the IEC bus doesn't.
.ifdef	__PET__
		.rodata
cmd:	.byte	"u0"		; Effectively, does nothing
		.code
		LDA		#.sizeof(cmd)
		ldx		#<cmd
		ldy		#>cmd
.else
		; Don't send a CBM DOS command.
		LDA		#0
.endif
		JSR		SETNAM

		; Call open, and handle the result.
		JSR		OPEN
.ifdef	__PET__
		BCS		error
		LDA		#<0
error:	PHA					; Push result to stack
.endif

		; Close our open channel.
		LDA		#15			; Get the file number
		JSR		CLOSE

		; Pass the error status back to the caller.
.ifdef	__PET__
		PLA					; Retrieve OPEN's result from stack
.else
		jsr		READST		; Retrieve CLOSE's result from status variable
		tax
		beq		noerror		;(bze)
		lda		#<5			; "device not present"
.endif
;		LDX		#>5			; Promote it to int
noerror:
		RTS					; We're done.
.endproc
