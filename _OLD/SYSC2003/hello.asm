;***************************************************************************
;  HELLO.ASM
;       Example program for the Axiom PM12DP256 module on the Axiom CMD912x board.
;	A text string is sent to the terminal using COM1.

;**************
;   EQUATES   *
;**************
RAMSTRT:        EQU     $1000   ; start of ram used
PRGSTRT:        EQU     $4000   ; start of code

;*******************
;   INCLUDE FILES
;*******************
#include        "dp256reg.asm"     ; include register equates

;**************
;     RAM     *
;**************
        ORG     RAMSTRT

IBUFSIZ:        EQU     15      ; input buffer size
EOT:    	EQU	$04	; end of text/table character
INBUFF  RMB  IBUFSIZ            ; input buffer, defined but not used
ENDBUFF EQU  *
COUNT   RMB  1                  ; characters read, also unused

;**********************
; Program starts here *
;**********************
        ORG     PRGSTRT

; If programming to Flash, uncomment the following line
;        ORG     $4000   ; DG/DP128 or DP256

START:

; If programming to Flash, uncomment the following line
;        LDS     #$3FFE   ; DG/DP128 or DP256 - initialize the stack pointer (don't do this if under monitor)

; enable PLL ...
	movb	#$01,SYNR		; 8 Mhz Eclock

; wait for PLL lock...
PLL_Wait:
	ldaa	CRGFLG		; get PLL flags
	bita	#$08			; test for lock
	beq	PLL_Wait		; wait for lock
	movb	#$80,CLKSEL		; PLL on...


        JSR     ONSCI   ; initialize serial port
	LDX	#MSG	; get message string
	JSR	OUTSTRG	; send it out serial port
	JSR	OUTCRLF	; output carriage-return
ENDPROG:
;        RTS             ; return (use this only if CALL is used, from monitor for example)
	JSR	COP_RESET	; clear watch-dog timer
	bra	ENDPROG	; endless loop

;**********************
; End of main loop    *
;**********************

; Initialize the SCI0 for 9600 baud
; Since we're using a 16.0 MHz clock and the Baud rate register is calculated:
; 	BR = MCLK / (16 * Baud_Rate)
;	MCLK = crystal / 2
;	BR = 8,000,000 / (16 * 9600) which is 153,600
;	BR = 52 which is 34 hex
ONSCI:
	ldaa	#$34	; get baud rate constant
	staa	SC0BDL	; store low byte
	clr	SC0BDH	; clear high byte
	ldaa	#$00	; configure SCI0 control registers
	staa	SC0CR1
	ldaa	#$0C	; enable transmit and receive
	staa	SC0CR2
	RTS

; Output string of ASCII bytes starting at x until end of text ($04).
OUTSTRG:
	JSR  OUTCRLF	; output carriage-return
OUTSTRG0:
	PSHA           ; save a
OUTSTRG1:
	LDAA 0,X	; read char into a
        CMPA #EOT      ; is this end of text?
        BEQ  OUTSTRG3	; jump if yes
        JSR  OUTPUT	; output character
        INX            ; incriment pointer
        BRA  OUTSTRG1  ; loop
OUTSTRG3:
	PULA		; restore a
        RTS

; Output a Carriage return and a line feed.  Returns a = cr.
OUTCRLF:
        LDAA #$0A       ; get LF
	JSR  OUTPUT	; send it
	LDAA #$0D	; get CR
	JSR  OUTPUT	; send it
        LDAA #$00
	JSR  OUTPUT	; output padding
	LDAA #$0D
	RTS

; Output A to SCI0
OUTPUT:
OUTSCI2:
	LDAB	SC0SR1	; read status
	BITB	#$80	; test Transmit Data Register Empty bit
	BEQ	OUTSCI2	; loop if TDRE=1
	ANDA	#$7F	; mask parity
	STAA	SC0DRL	; send character
        RTS

; The M68HC12A4 powers on with the COP (Computer Operating Properly)
; watchdog system enabled.  If you don't reset it occasionally in your software
; it will reset you.  This subroutine will reset the COP.
COP_RESET:
;        LDAA    #$55    ; get 1st COP reset value
;        STAA    COPRST  ; store it
;        LDAA    #$AA    ; get 2nd COP reset value
;        STAA    COPRST  ; store it
        rts

;**************
; TEXT TABLES *
;**************
MSG     FCC   'Hello World'
        FCB   EOT


; If programming to Flash, uncomment the following 2 lines
;       org     $fffe                   reset vector
;       fdb     START



         END
