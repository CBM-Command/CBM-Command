;
; 2001-03-18, Marc 'BlackJack' Rintsch (wrote original C code)
; 2002-06-22, Ullrich von Bassewitz (converted it to Assembly code)
; 2012-09-08, Greg King (shortenned it by swapping first two expressions)
;
; unsigned char __fastcall__ cbm_open (unsigned char lfn,
;                                      unsigned char device,
;                                      unsigned char sec_addr,
;                                      const char* name);
; /* Opens a file.  Works just as the BASIC command does.
;  * Returns 0 if openning was successful; otherwise, an error-code.
;  */
; {
;     cbm_k_setlfs(lfn, device, sec_addr);
;     cbm_k_setnam(name);
;     return _oserror = cbm_k_open();
; }
;

	.export	_cbm_open

	.import	popa
	.import	_cbm_k_setlfs, _cbm_k_setnam, _cbm_k_open
	.import	__oserror

_cbm_open:
	jsr	_cbm_k_setnam

	jsr	popa		; Get sec_addr
	jsr	_cbm_k_setlfs	; Call SETLFS, pop all args.

	jsr	_cbm_k_open
	sta	__oserror
	rts
