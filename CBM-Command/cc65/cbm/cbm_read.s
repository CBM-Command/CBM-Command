;
; 2002-06-22, Ullrich von Bassewitz
; 2020-11-07, Greg King
;
; 2001-03-19, Original C code by Marc 'BlackJack' Rintsch
;
; int __fastcall__ cbm_read (unsigned char lfn, void* buffer, unsigned int size)
; /* Reads up to "size" bytes from a file to "buffer".
; ** Returns the number of actually read bytes, 0 if there are no bytes left
; ** (EOF or error); or -1 if a file error, _oserror contains an error-code then.
; */
; {
;     static unsigned int bytesread;
;     static unsigned char tmp;
;
;     /* if we can't change to the inputchannel #lfn then return an error */
;     if (_oserror = cbm_k_chkin(lfn)) return -1;
;
;     bytesread = 0;
;
;     while (bytesread<size && !cbm_k_readst()) {
;         tmp = cbm_k_basin();
;
;         /* The Kernal routine BASIN sets ST to EOF if the end of file
;         ** is reached the first time, then we must store tmp.
;         ** Every subsequent call returns EOF and READ ERROR in ST, then
;         ** we must exit the loop here immediately.
;         */
;         if (cbm_k_readst() & 0xBF) break;
;
;         ((unsigned char*)buffer)[bytesread++] = tmp;
;     }
;
;     cbm_k_clrch();
;     return bytesread;
; }
;

;        .include        "cbm.inc"
	.import		CHKIN, BASIN, READST, CLRCH

        .export         _cbm_read
        .importzp       ptr1, ptr2, ptr3, tmp1
        .import         popax, popa
        .import         __oserror


_cbm_read:
        inx
        stx     ptr1+1
        tax
        inx
        stx     ptr1            ; Save size with both bytes incremented separately

        jsr     popax
        sta     ptr2
        stx     ptr2+1          ; Save buffer

        jsr     popa
        tax
        jsr     CHKIN
        bcs     @E1             ; Branch on error

; bytesread = 0;

        lda     #$00
        sta     ptr3
        sta     ptr3+1
        beq     @L3             ; Branch always

; Loop

@L1:    jsr     BASIN           ; Read next char from file
        sta     tmp1            ; Save it for later

        jsr     READST
        tax                     ; Save status for later
        and     #<~%01000000    ; Don't check EOF here
        bne     @L4             ; Branch on read-errors

        lda     tmp1
        ldy     #$00
        sta     (ptr2),y        ; Store read byte

        inc     ptr2
        bne     @L2
        inc     ptr2+1          ; ++buffer;

@L2:    inc     ptr3
        bne     @L6
        inc     ptr3+1          ; ++bytesread;

@L6:    txa
        bne     @L4             ; Branch on End-Of-File

@L3:    dec     ptr1
        bne     @L1
        dec     ptr1+1
        bne     @L1

@L4:    jsr     CLRCH

        lda     ptr3
        ldx     ptr3+1          ; return bytesread;
        rts

; CHKIN failed

@E1:    sta     __oserror
        lda     #<-1
        tax
        rts                     ; return -1;
