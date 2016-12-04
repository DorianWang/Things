; Lab10 - Magic 8 Ball
;	Set up the Timer interrupt
; 	Prompt user to ask a question, then press a button
; 	When a key is pressed, choose a message to print, depending on TICKS % 3
;	Go back and prompt the user

; Constant definitions
DISPLAY	.EQU 04E9h	; address of Libra display

; Set up Timer ISR vector (3 lines)
*
*
*

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

*
*
*

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; END OF SUBROUTINES FROM lab8.asm
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; INSERT modified TIMER ISR
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
timerISR:          ;FIX ME: Timer ISR (2 lines)
*
*
		
;;;;;;;;;;;;;
; Main function: Prompt the user to ask a question, then press a key 
; 				When a user presses a key, compute CX%3, 
;				then choose one of the 3 messages to print
;				repeat...
;
;				Uses printStr, newline, and getChar subroutines.
main:
	; Configure the timer subsystem (6 lines)
***
	STI

; Set up infinite Loop to prompt user and print answer
PromptUser:
	*					;FIX ME: Move starting address of prompt to SI
	*					;FIX ME: Call prtstr to print prompt message
	*					;FIX ME: Print a new line
			
	*					;FIX ME: call Getchar to get value from keyboard
	
	; FIX ME: Compute TICKS%3 (3 lines)
	*
	*
	*

	; Depending on remainder, set SI to point to the correct message (~12 lines)
***

	; Print the selected message (2 lines)
print:
	*					;FIX ME: Call prtstr to print selected Message (Pointed to by SI)
	*					;FIX ME: Print new line
	
	JMP PrompUser		; Go back and do it again

.END main		;Entry point of program is main()