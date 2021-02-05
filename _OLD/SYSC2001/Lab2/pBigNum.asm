;
; 	Name: Dorian Wang
; 	Student number: 101009020
;
;	SYSC2001 Lab2
;


;; CONSTANTS
; In this case we are defining the symbol 'Display' to have the value
;  of 04E9h - the address of the Libra display screen
;
Display	.EQU 04E9h	; address of Libra display

; Define a memory location where the series of numbers to be printed will be start
; ** You will have to manually enter these numbers in memory in Libra prior to running your code!
NumAddr .EQU 0050h


;
;	Program: PrtNum - print the number in AL on the screen by converting to ASCII 
;
;  	
Main:
;
; We must do some initialization
; First, we must initialize our pointer (BX) to point to the first number in memory that 
; we wish to print (You will have to set the contents of memory!)
; To do this, we use immediate addressing mode to set BX==NumAddr
;
	mov BX, NumAddr
	mov DX, Display
;
; The next statement defines another address - the beginning of the print loop
;
mainLoop:
;
; Place the number to print into the 'AX' register
;
	MOV AL, [BX]		; 

; Now we need to check if it we are done printing. If AH contains a zero, then quit.
;

	cmp AL, 0    ; Jumps if the value is less than 0. 
	JGE skipnegNum	
	
	
negNum:
	; If we get here, then our number is negative. We need to first print a negative sign
	; and then convert our number to its positive equivalent before printing

	; Print a negative sign
	
	push AX
	
	mov AL, '-'		; moves a '-' into the register
	out DX, AL		; Send the character out to the display port
	
	pop AX
	
	; Convert the negative number in AH to a positive number. This can be accomplished with a single instruction!
	neg AL	
	
	skipnegNum:
	MOV CL, 0Ah
	DIV CL
	
	cmp AL, 0    ; Jumps if the value is 0. 
	JE skipFirstDigit		
	



	;MOV AL, AH		; ***FIX ME. Should copy number to be printed into register AL
	ADD AL, '0'		; adds '0' for offset
	out DX, AL		; print the char
	
	skipFirstDigit:
	
	MOV AL, AH		; 
	ADD AL, '0'		; adds '0' for offset
	out DX, AL		; print the char

; Print a carriage return, so that the next number is printed on a new line
	MOV AL, 0xD		; ***FIX ME. Should load carriage return (CR) into AL
	out DX, AL		; print the CR
	MOV AL, 0xA		; ***FIX ME. Should load line feed (LF) into AL
	out DX, AL		; print the LF

	;inc BX			; step along the array of numbers to the next number
;
; The next statement is an unconditional jump back to the beginning of the loop
; "Unconditional" means that the jump is ALWAYS taken (no CMP needed).
;
	;jmp mainLoop; loop back
	
EndPrt:
	HLT			; Stop the Libra processor

.END Main










