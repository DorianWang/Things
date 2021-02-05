; Lab3b Part I - Subroutine to prompt the user to enter a key
;				get a character from the keyboard
; 				and check if the user presses 'y'

; Constant definitions
Display	.EQU 04E9h	; address of Libra display


;---------------------------
;Insert subroutines printStr and newLine from Lab 5 here
;---------------------------

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


;;;;;;;;;;;;;;;;;
; printDigit: Subroutine to print a single decimal digit
; Input parameters:
; 	AL: Unsigned decimal digit (between 0-9) to be printed
; Output parameters:
;	None.
printDigit:
	; Save registers modified by this subroutine
	PUSH AX			; FIX ME
	PUSH DX			; FIX ME
	
	MOV DX, Display
	ADD AL, '0'		; Convert number to ASCII code
	OUT DX,AL		; Print it
	
	; Restore registers
	POP DX			; FIX ME
	POP AX			; FIX ME
	
	RET
	
		
;;;;;;;;;;;;;;;;;
; printInt: Subroutine to print a 1-byte unsigned (short) integer between 0-255
; Input parameters:
; 	AL: Unsigned short int to be printed
; Output parameters:
;	None.
printInt:
	; Save registers modified by this subroutine
	PUSH AX			; FIX ME
	PUSH CX			; FIX ME
	PUSH DX			; FIX ME
	
	MOV DX, Display
	MOV CL, 10		; Will be dividing by 10...
	
LoopPI:
	CMP AL, 10		; Compare the number to 10
	JL printLast	; If it is less than 10, then print this digit
	; If it is greater than 10, divide by 10
	MOV AH, 0		; Clear AH
	DIV CL			; Divide number by 10
	CALL printDigit ; Print the quotient in AL
	MOV AL, AH		; Move remainder into AL to be printed
	jmp LoopPI		; Jump back to the top of the loop
printLast:
	CALL printDigit
	
	; Restore registers
	POP DX			; FIX ME
	POP CX			; FIX ME
	POP AX			; FIX ME
	
	RET


;;;;;;;;;;;;;;;;;
; printSalary: Subroutine to print employee salary
; Input parameters:
; 	AL: Unsigned short int (0-99) representing salary in 1000's of $
; Output parameters:
;	None.

; Constant strings for this subroutine:
s_thousands: .DB ',000$'
s_dollars: .DB '$'

printSalary:
	; Save registers modified by this subroutine
	PUSH AX		
	PUSH SI			
	PUSH DX			
	
	MOV DX, Display
	
	MOV AH,AL			; Keep a copy of the salary in AH (need AL for printing...)
	MOV AL, [s_dollars]	; Print '$' preceeding number
	OUT DX,AL			; Print it
	MOV AL,AH			; Move salary back into AL
	CALL printInt		; Print the salary (0-255)
	MOV SI, s_thousands	; Move the starting address of s_thousands string into BX
	CALL printStr 		; Print ',000'
	
	; Restore registers
	POP DX			
	POP SI			
	POP AX			
	
	RET


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
	
;---------------------------
;End of subroutines printStr and newLine from Lab 5 here
;---------------------------


;;;;;;;;;;;;;;;;;
; getChar: waits for a keypress and returns pressed key in AL
; Input parameters:
; 	none.
; Output parameters:
;	AL: ASCII Value of key pressed by user

; Constants used by this subroutine
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


	
	
	
Message1: .DB	'Enter a number between 0 and 9.$'		; Message to be printed on screen
Message2: .DB	'Have a nice day!$'						; Message to be printed on screen
		
;;;;;;;;;;;;;
; Main function: Asks the user whether they want to quit or not. 
; 				Repeats until user presses 'y'
;
;				Uses printStr, newline, and getChar subroutines.
main:
	mov SI, Message1				;Move starting address of Message1 to SI
	call printStr					;Call prtstr to print Message1
	call newLine					;Print a new line
	
	
	
	
	getNewDigitLoop:
	call getChar					;call Getchar to get value from keyboard
	cmp AL, '0'
	jl getNewDigitLoop 
	cmp AL, '9'
	jg getNewDigitLoop
			
			
	mov DX, Display
	out DX, AL						; Echo the character back to the display
	call newLine
									
	cmp AL, '0'						; compare input character with numbers
	je foundDiv3Digit				;If user did not press a number divisible by 3, then re-prompt (start over)
	
	cmp AL, '3'						; compare input character with numbers
	je foundDiv3Digit				;If user did not press a number divisible by 3, then re-prompt (start over)
	
	cmp AL, '6'						; compare input character with numbers
	je foundDiv3Digit				;If user did not press a number divisible by 3, then re-prompt (start over)
	
	cmp AL, '9'						; compare input character with numbers
	je foundDiv3Digit				;If user did not press a number divisible by 3, then re-prompt (start over)
	
	jmp main
	
	foundDiv3Digit:
	mov SI, Message2
	call printStr
	;Quit
	HLT
	
	

.END main		;Entry point of program is main()


























