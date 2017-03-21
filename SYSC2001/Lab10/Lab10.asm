; Lab10 - Magic 8 Ball
;	Set up the Timer interrupt
; 	Prompt user to ask a question, then press a button
; 	When a key is pressed, choose a message to print, depending on TICKS % 3
;	Go back and prompt the user

; Constant definitions
Display	.EQU 04E9h	; address of Libra display

; Set up Timer ISR vector (3 lines)
.ORG 0x0020
.dw TimerISR
numTick .EQU 1000

; Data segment (beyond interrupt vector region)
.ORG 0x200
TICKS: .DB 0;			; Tracks how many ticks have elapsed.

prompt: .DB	'Ask your question, then press a key...$'		; Prompt message to be printed on screen

message0: .DB 'Yes, absolutely!$'
message1: .DB 'No way dude...$'
message2: .DB 'Ask your mother instead.$'

; Code segment
.ORG 0x300

;---------------------------
;Insert Sub-routines printStr, newLine, and getChar from Lab8 here
;---------------------------

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
; END OF SUBROUTINES FROM lab8.asm
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; INSERT modified TIMER ISR
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
TimerISR:          ;FIX ME: Timer ISR (2 lines)
INC BYTE [TICKS]
IRET
		
;;;;;;;;;;;;;
; Main function: Prompt the user to ask a question, then press a key 
; 				When a user presses a key, compute CX%3, 
;				then choose one of the 3 messages to print
;				repeat...
;
;				Uses printStr, newline, and getChar subroutines.
main:
	; Configure the timer subsystem (6 lines)
	MOV DX, 0x40    ;point DX to the timer subsystem DATA port 
	MOV AX, numTick ;1000 millisecond count
	OUT DX, AX      ;Set the new count in the timer
	MOV DX, 0x43    ;point DX to the timer subsystem CONTROL port
	MOV AL, 0x06    ;enable timer (bit 1) and timer interrupts (bit 2) (0x02 | 0x04 == 0x06)
	OUT DX, AL      ;start timer
	STI

; Set up infinite Loop to prompt user and print answer
PromptUser:
	mov SI, prompt					;FIX ME: Move starting address of prompt to SI
	call printStr					;FIX ME: Call prtstr to print prompt message
	call newLine					;FIX ME: Print a new line
			
	call getChar					;FIX ME: call Getchar to get value from keyboard
	
	; FIX ME: Compute TICKS%3 (3 lines)
	mov AH, 00h ;Extra bit to make sure AX is clear.
	mov BL, [TICKS]
	div BL
	

	; Depending on remainder, set SI to point to the correct message (~12 lines)
	mov SI, message2
	cmp AH, 2
	je print
	mov SI, message1
	cmp AH, 1
	je print
	mov SI, message0
	
	
	; Print the selected message (2 lines)
print:
	call printStr					;FIX ME: Call prtstr to print selected Message (Pointed to by SI)
	call newLine					;FIX ME: Print new line
	
	JMP PromptUser		; Go back and do it again

.END main		;Entry point of program is main()






