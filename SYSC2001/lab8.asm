; Program to accept a signed decimal number in the format +/-xxxx
; Calculate the 8-bit "quarter precision" IEEE-754 encoding and print it to screen.
; 

; Format -/+xxxx in decimal, entered as ASCII.
; 1) Get sign
; 2) Get number
; 3) Normalize number to get exponent
; 3) Compute bias-** representation of exponent
; 4) Create final IEEE-754 representation

; Constant definitions
Display	.EQU 04E9h	; address of Libra display

; Global variables
.ORG 0000
SIGN:	.DB	0		; Sign of entered number (0=positive, 1=negative)
NUM:	.DB	0		; Unsigned  binary representation of entered number
EXP:	.DB	0		; Excess/bias representation of exponent (only uses lower 3 bits)
FP:		.DB	0		; 8-bit quarter-precision IEEE-754 representation of number

.ORG 1000h


;;;---------------------------
;Insert Sub-routines getChar, printStr, and newLine from Lab 7 here
;;;---------------------------

KBSTATUS .EQU 0064h			; port number of keyboard STATUS reg
KBBUFFER .EQU 0060h			; port number of keyboard BUFFER reg

getChar:
	push DX        ; save reg used
GCWait:
	MOV DX, KBSTATUS	; load addr of keybrd STATUS
	IN	AL, DX			; Read contents of keyboard STATUS register
	cmp AL, 1			; key pressed?
	jne GCWait			; no, go back and check again for keypress

	MOV DX, KBBUFFER	; load port number of kbrd BUFFER register
	IN  AL, DX			; get key into AL from BUFFER
GCDone:
	pop DX        	; restore regs
	ret

;;;;;;;;;;;;;;;;;
; newLine: Subroutine to print a newline and a linefeed character
; Input parameters:
; 	None.
; Output parameters:
;	None.

; Constants for this subroutine:
s_CR .EQU 0Dh		; ASCII value for Carriage return
s_LF .EQU 0Ah		; ASCII value for NewLine

newLine:
	; Save registers modified by this subroutine
	PUSH AX			; FIX ME
	PUSH DX			; FIX ME
	
	MOV DX, Display		; Initialize the output port number in DX
	MOV AL, s_LF		; Load line feed (LF) into AL
	out DX,AL			; print the char
	MOV AL, s_CR		; Load carriage return (CR) into AL
	out DX,AL			; print the char
	
	; Restore registers
	POP DX			; FIX ME
	POP AX			; FIX ME
	
	RET

;;;;;;;;;;;;;;;;;
; printStr: Subroutine to print a '$'-terminated string
; Input parameters:
; 	SI: Address of start of string to be printed
; Output parameters:
;	None.
printStr:
	; Save registers modified by this subroutine
	PUSH AX			; FIX ME
	PUSH BX			; FIX ME
	PUSH DX			; FIX ME

	MOV DX, Display
	MOV BX,0
LoopPS:
	MOV AL, [SI+BX]	; Load the next char to be printed - USING INPUT PARAMETER SI
	CMP AL, '$'		; Compare the char to '$'
	JE quitPS		; If it is equal, then quit subroutine and return to calling code
	OUT DX,AL		; If it is not equal to '$', then print it
	INC BX			; Point to the next char to be printed
	jmp LoopPS		; Jump back to the top of the loop
quitPS:
	; Restore registers
	POP DX			; FIX ME
	POP BX			; FIX ME
	POP AX			; FIX ME

	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; END OF SUBROUTINES FROM lab7.asm
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;
; getSign: waits for user to press '+' or '-'. Ignores other chars. 
;          Valid input sign character is echoed to screen.
; Input parameters:
; 	none.
; Output parameters:
;	AL: Returns a zero for '+' and one for '-'
getSign:
	push DX
	mov DX, Display
	getSignLoop:
	call getChar
	
	cmp AL, '-'
	jne skipNegativeSign
	out DX, AL
	call newLine
	mov AL, 1
	pop DX
	RET
	
	skipNegativeSign:
	
	cmp AL, '+'
	jne skipPositiveSign
	out DX, AL
	call newLine
	mov AL, 0
	pop DX
	RET
	
	skipPositiveSign:
	
	jmp getSignLoop
	pop DX
	RET
	

;;;;;;;;;;;;;;;;;
; getDigit: waits for user to press 0-9 digit. Ignores other chars except RETURN
; Input parameters:
; 	none.
; Output parameters:
;	AL: Returns binary value of digit in AL. Returns 99 if user presses ENTER

; Constants used by this subroutine
ENTERK .EQU 0Ah

getDigit:
push DX
mov DX, Display
	getDigitLoop:
	
	CALL getChar		
	
	cmp AL, ENTERK
	jne skipEnterKey
	mov AL, 99
	pop DX
	RET
	skipEnterKey:
	
	cmp AL, '9'
	jg getDigitLoop
	cmp AL, '0'
	jl getDigitLoop
	out DX, AL
	sub AL, '0'
	pop DX
	RET
	

;;;;;;;;;;;;;;;;;
; getNumber: Accepts a series of decimal digits and builds a binary number using shift-add
; Input parameters:
; 	none.
; Output parameters:
;	AL: Returns binary value of number in AL. 

; Constants used by this subroutine
DONE .EQU 99

getNumber:				; FIX ME -- complete entire subroutine
	PUSH CX				; Save CX register
	PUSH BX
	MOV CH, 0			; Use CH for running sum
	MOV CL, 10			; Use CL for multiplier=10
	mov AL, 0
loopGN:

	call getDigit
	cmp AL, 99
	je doneGN
	mov BL, AL

	mov AL, CH
	mul CL
	mov CH, AL

	mov AL, BL
	


	add CH, AL

	JMP loopGN
doneGN:
	mov AL, CH			; Put final sum into AL
	POP BX
	POP CX				; Restore CX
	RET
	

Message1: .DB	'Enter a number BW -29 to +30.$'		; FIX ME -- Message to be printed on screen

		
;;;;;;;;;;;;;
; Main function: Asks the user to enter a signed number between -MAX to +MAX 
; 				Computes quarter-precision 8-bit IEEE-754 representation
;
;				Uses printStr, newline, and getChar subroutines.
main:
	mov DX, Display			;FIX ME Print prompt
	mov SI, Message1
	call printStr			;FIX ME
	call newLine			;FIX ME

part2:			
	call getSign			;FIX ME - call getSign to get +/- sign from keyboard
	mov [SIGN], AL			;FIX ME - Save sign to global variable SIGN
	call getNumber			;FIX ME -  call getNumber to get the unsigned number
	mov [NUM], AL			;FIX ME -  Save number to global variable NUM

;Quit
	HLT
	
	

.END main		;Entry point of program is main()









