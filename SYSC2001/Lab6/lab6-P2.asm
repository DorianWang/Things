; Lab6 - Subroutine to retrive and print a salary from 
;		  an Employee record from an array of structures

; Constant definitions
Display	.EQU 04E9h	; address of Libra display

; Offsets to access individual fields within the records
ID			.EQU 0	; Zero bytes from start of record is ID
NAME 		.EQU 1	; One byte from start of record is name
GENDER 		.EQU 3	; 3 bytes from start of record is gender
SALARY 		.EQU 4	; 4 bytes from start of record is salary
REC_SIZE	.EQU 5	; Total size of each record is 5 bytes


;;;;;;;;;;;;;;;;;;;;
; Function: getSalary
; Function to retrive the salary of the specified employee record.
;
; Input Parameters:
; 	Stack: 16-bit address of start of array of structures
;	Stack: 8-bit record number to be printed (record numbering starts at 0)
; Output Paramters:
;	Stack: returns 8-bit salary of specified Employee record
getSalary:
	; Save register values that will be modified in this routine
	PUSH AX						; FIX ME
	PUSH SI						; FIX ME
	PUSH BP						; FIX ME

	; Get input parameters from the stack. 
	; Use SI for the start address of the array of structures
	; Use AL for record number
	MOV BP, SP					; FIX ME
	MOV SI, [BP + 10]			; FIX ME
	MOV AL, [BP + 8]			; FIX ME
	
	; Calculate OFFSET of this record (distance from starting address of array of structures)
	; Offset is REC_NUM*REC_SIZE)
								; FIX ME: Load REC_SIZE into suitable register (for MUL)
								; FIX ME: Multiply REC_NUM by REC_SIZE 
								; FIX ME: move offset into a suitable register (see next line)

	; For the next instruction, you MUST USE BASED-INDEXED Addressing mode (look it up!)
	MOV AL, 					; FIX ME - Load the salary of this record into AL

	MOV 						; FIX ME - Save return value into reserved slot in stack frame
	
	; Restore registers
								; FIX ME
								; FIX ME
		 						; FIX ME

	; Return to calling function
								; FIX ME


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; INSERT SUBROUTINES FROM lab6-P1.asm HERE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; END OF SUBROUTINES FROM lab6-P1.asm
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

		
;;;;;;;;;;;;;;;;;;;;;;;;	
; main: Main function to test all subroutines
main:

	; Print salary of dayShiftDB[0]
	MOV BX, dayShiftDB
	MOV AL, 0
							; FIX ME Make room on stack for return value from getSalary
							; FIX ME Push record number
							; FIX ME Push starting address of array of structures
							; FIX ME Invoke getSalary subroutine
							; FIX ME Pop starting address of array of structures
							; FIX ME Pop record number
	call printSalary		; Input parameter for printSalary is already on the stack!
	CALL newLine
							; FIX ME Pop the return value from getSalary
	
	; Print dayShiftDB[3] FIX ME. Add LINES BELOW TO DO THIS











	
	; Print nightShiftDB[0] FIX ME. Add LINES BELOW TO DO THIS









	
	;Quit
	HLT
	
	
;;;;;;;;;;;;;;;;;;;;;;;;
; Test data
;

; Record format:
;Struct Employee {
;	char* name;	// 2-byte pointer to string of chars
;	bool gender;	// 1-byte Boolean (zero-->male, else-->female)
;	short salary;	// 1-byte unsigned short int salary (in $1000’s)
;	int id;		// 1-byte unsigned integer ID
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