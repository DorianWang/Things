; Lab6-P1 - Subroutines to print integers, digits, and salaries

; Constant definitions
Display	.EQU 04E9h	; address of Libra display


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  START OF SUBROUTINES to COPY to lab6-P2.asm
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;
; printInt: Subroutine to print a 1-byte unsigned (short) integer between 0-255
; Input parameters:
; 	Stack: Unsigned short int to be printed
; Output parameters:
;	None.
printInt:
	; Save registers modified by this subroutine
	PUSH AX
	PUSH CX
	PUSH BP			
	
	; Retrieve input parameter from stack into AL
	MOV BP, SP		; make a copy of SP
	MOV AL, [BP + 8]; read from the appropriate slot of your stack frame
	
	MOV CL, 10		; Will be dividing by 10...
	
LoopPI:
	CMP AL, 10		; Compare the number to 10
	JL printLast	; If it is less than 10, then print this digit
	; If it is greater than 10, divide by 10
	MOV AH, 0		; Clear AH
	DIV CL			; Divide number by 10
	
	PUSH AX			; Push the input parameter of printDigit onto the stack
					
	CALL printDigit ; Print the quotient in AL
	
	POP AX			; Pop the input parameter of printDigit back off the stack
					
	MOV AL, AH		; Move remainder into AL to be printed
	jmp LoopPI		; Jump back to the top of the loop
printLast:
	PUSH AX			; Push the input parameter of printDigit onto the stack
	CALL printDigit ; Print the quotient in AL
	POP AX			; Pop the input parameter of printDigit back off the stack

	; Restore registers
	POP BP			
	POP CX
	POP AX
	
	RET


;;;;;;;;;;;;;;;;;
; printDigit: Subroutine to print a single decimal digit
; Input parameters:
; 	Stack: Unsigned decimal digit (between 0-9) to be printed
; Output parameters:
;	None.
printDigit:
	; Save registers modified by this subroutine
	PUSH AX
	PUSH DX
	PUSH BP			
	
	; Retrieve input parameter from stack into AL
	MOV BP, SP		; make a copy of SP
	MOV AL, [BP + 8]; read from the appropriate slot of your stack frame
	
	MOV DX, Display
	ADD AL, '0'		; Convert number to ASCII code
	OUT DX, AL		; Print it
	
	; Restore registers
	POP BP			
	POP DX
	POP AX
	
	RET
	
		
;;;;;;;;;;;;;;;;;
; printSalary: Subroutine to print employee salary
; Input parameters:
; 	Stack: Unsigned short int (0-255) representing salary in 1000's of $
; Output parameters:
;	None.

printSalary:
	; Save registers modified by this subroutine
	PUSH AX
	PUSH SI				; Not strictly necessary, but please keep
	PUSH DX
	PUSH BP				
	
	; Retrieve input parameter from stack into AL
	MOV BP, SP			; make a copy of SP
	MOV AX, [BP + 10]	; read from the appropriate slot of your stack frame
	
	MOV DX, Display
	
	MOV AH,AL			; Keep a copy of the salary in AH (need AL for printing...)
	MOV AL, '$'			; Print '$' preceeding number
	OUT DX,AL			; Print it
	MOV AL,AH			; Move salary back into AL
	
	PUSH AX				; Push the input parameter of printInt onto the stack
	CALL printInt		; Print the salary (0-255)
	POP AX				; Pop the input parameter of printInt back off the stack

	MOV AL, ','			; Print ',' after number
	OUT DX,AL			; Print it
	MOV AL, '0'			; Print '0' after comma
	OUT DX,AL			; Print a zero
	OUT DX,AL			; Print a zero
	OUT DX,AL			; Print a zero
	
	; Restore registers
	POP BP				
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
	PUSH AX
	PUSH DX
	
	MOV DX, Display		; Initialize the output port number in DX
	MOV AL, s_LF		; Load line feed (LF) into AL
	out DX,AL			; print the char
	MOV AL, s_CR		; Load carriage return (CR) into AL
	out DX,AL			; print the char
	
	; Restore registers
	POP DX
	POP AX
	
	RET
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; END OF SUBROUTINES FOR lab6-P2.asm
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

		
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Test data
;

.ORG 00A0h

num1: .DB 86					; Should print as decimal 86
sal1: .DB 34 				; Should print as '$34,000'

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
; main: Main function to test all subroutines
.ORG 00B0h

main:

	; Print a short unsigned int (0-99). Use num1
	MOV AL, [num1]				; get input parameter
	PUSH AX						; place input parameter on stack
	CALL printInt
	POP AX						; remove input parameter from stack
	CALL newLine
	
	; Print a salary. Use sal1
	MOV AL, [sal1]				; FIX ME - get input parameter
	PUSH AX						; FIX ME - place input parameter on stack
	CALL printSalary
	POP AX						; FIX ME - remove input parameter from stack
	CALL newLine
	
	;Quit
	HLT
	
	

.END main		;Entry point of program is main()