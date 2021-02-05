; Lab5 - Subroutine to print a record from an array of structures

; Constant definitions
Display		.EQU 04E9h	; address of Libra display


; Constant strings (prompts or labels to be printed to screen, etc)
s_name: 	.DB 'Name: $'
s_male: 	.DB 'Mr. $'
s_female: 	.DB 'Ms. $'
s_empNum: 	.DB 'Employee number: $'
s_salary: 	.DB 'Salary: $'

; Offsets to access individual fields within the records
ID			.EQU 0	; Zero bytes from start of record is ID
NAME 		.EQU 1	; One byte from start of record is name
GENDER 		.EQU 3	; 3 bytes from start of record is gender
SALARY 		.EQU 4	; 4 bytes from start of record is salary
REC_SIZE	.EQU 5	; Total size of each record is 5 bytes

; Other defines
male		.EQU 0	; Gender field: 0=male, 1=female
female		.EQU 1	;

;;;;;;;;;;;;;;;;;;;;
; Function: printEmployee
; Function to print an employee record to screen.
;
; Input Parameters:
; 	BX: Address of start of array of structures
;	AL: Record number to be printed (record numbering starts at 0)
; Output Paramters:
;	None.
printEmployee:
		; Save register values that will be modified in this routine
		PUSH AX			
		PUSH SI			
		PUSH DX			

		; Calculate starting address of this record
		; Starting address is START+(REC_NUM*REC_SIZE)
		MOV AH, REC_SIZE			; Load REC_SIZE into AH
		MUL AH						; Multiply REC_NUM (already in AL) by REC_SIZE (in AH)
		ADD BX, AX						; Compute START+(REC_NUM*REC_SIZE)

		; Print 'Name: ' label
		MOV SI, s_name				
		CALL printStr

		; Print Mr/Mrs according to gender
		MOV AL, [BX + GENDER]			
		CMP AL, male				; Compare gender to male
		je printMale
	printFemale:
		MOV SI, s_female			; Print Ms. if not male
		CALL printStr
		JMP	printName
	printMale:
		MOV SI, s_male				; Print Mr. if male
		CALL printStr

		; Prints name. Must load name pointer into SI, then call printStr
	printName:
		MOV SI, [BX + 1]				
		CALL printStr				
		CALL newLine				; Print a newLine character

		; Prints employee number
	printEmpNum:
		MOV SI, s_empNum				; Print 'Employee number: '
		CALL printStr
		MOV AL, [BX]			; Gets employee number from record.
		CALL printInt
		CALL newLine					; Print a newLine character

		; Print employee salary
	printEmpSalary:
		MOV SI, s_salary			; Print the 'Salary: ' label 
		CALL printStr
		MOV AL,	[BX + SALARY]		; Loads the SALARY field into AL.
		CALL printSalary			; Prints salary in 1000's of $
		CALL newLine				; Print a newLine character

		; Restore registers
		POP DX			
		POP SI			
		POP AX			
	; Return to calling function
	RET


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; INSERT SUBROUTINES FROM lab5-P1.asm HERE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

printStr:
	; Save registers modified by this subroutine
	PUSH AX			
	PUSH BX			
	PUSH DX			

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
	POP DX			
	POP BX			
	POP AX			

	RET


;;;;;;;;;;;;;;;;;
; printDigit: Subroutine to print a single decimal digit
; Input parameters:
; 	AL: Unsigned decimal digit (between 0-9) to be printed
; Output parameters:
;	None.
printDigit:
	; Save registers modified by this subroutine
	PUSH AX			
	PUSH DX
	PUSH BP
	
	MOV BP, SP
	MOV AX, [BP + 8]
	
	MOV DX, Display
	ADD AL, '0'		; Convert number to ASCII code
	OUT DX, AL		; Print it
	
	; Restore registers
	POP BP
	POP DX			
	POP AX			
	
	RET
	
		
;;;;;;;;;;;;;;;;;
; printInt: Subroutine to print a 1-byte unsigned (short) integer between 0-255
; Input parameters:
; 	AL: Unsigned short int to be printed
; Output parameters:
;	None.
printInt:
	; Save registers modified by this subroutine
	PUSH AX			
	PUSH CX			
	PUSH DX			
	
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
	POP DX			
	POP CX			
	POP AX			
	
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
; END OF SUBROUTINES FROM lab5-P1.asm
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

		
;;;;;;;;;;;;;;;;;;;;;;;;	
; main: Main function to test all subroutines
main:

	; Print dayShiftDB[2]
	MOV BX, dayShiftDB		; Load address of dayShiftDB into register BX
	MOV AL, 2				; Load record number into register AL
	CALL printEmployee
	CALL newLine
	
	; Print nightShiftDB[0]
	MOV BX, nightShiftDB 	; Load address of nightShiftDB
	MOV AL, 0				; Load record number
	CALL printEmployee
	CALL newLine
	
	; Print dayShiftDB[3]
	MOV BX, dayShiftDB 		; Load address of dayShiftDB
	MOV AL, 3				; Load record number
	CALL printEmployee
	CALL newLine
	
	;Quit
	HLT
	
	
;;;;;;;;;;;;;;;;;;;;;;;;
; Test data
;

; Record format:
;Struct Employee {
;	int id;			// 1-byte unsigned integer ID
;	char* name;		// 2-byte pointer to string of chars
;	bool gender;	// 1-byte Boolean (zero-->male, else-->female)
;	short salary;	// 1-byte unsigned short int salary (in $1000’s)
;};
.ORG 5000h

dayShiftDB:
	; Record dayShiftDB[0]
	.DB 12			; dayShiftDB[0].id
	.DW name0		; dayShiftDB[0].name
	.DB 0			; dayShiftDB[0].gender
	.DB 50			; dayShiftDB[0].salary
	
	; Record dayShiftDB[1]
	.DB 27
	.DW name1		
	.DB 1
	.DB 58
	
	; Record dayShiftDB[2]
	.DB 1
	.DW name2		
	.DB 1
	.DB 70

	; Record dayShiftDB[3]
	.DB 77
	.DW name3		
	.DB 0
	.DB 32

nightShiftDB:
	.DB 7
	.DW name4		; Record nightShiftDB[0]
	.DB 1
	.DB 99
	
	.DB 80
	.DW name5		; Record nightShiftDB[1]
	.DB 0
	.DB 75

name0: .DB 'Sam Jones$'
name1: .DB 'Sara Thomas$'
name2: .DB 'Samira Smith$'
name3: .DB 'Max Golshani$'
name4: .DB 'The Boss!$'
name5: .DB 'Sven Svenderson$'

.END main		;Entry point of program is main()