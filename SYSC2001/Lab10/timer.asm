; Set up the Timer ISR's Vector:
.ORG 0x20       ;location of ISR type 8
.dw timerISR        ;install ISR type 8
; I think it should be at 16?, because it is type 8?

numTick .EQU 1000

.ORG 0x200      ;past all ISR addresses (0x00 - 0x1FF)
start:

; Configure the timer to be enabled and generate interrupts every 1s
MOV DX, 0x40    ;point DX to the timer subsystem DATA port 
MOV AX, numTick ;1000 millisecond count
OUT DX, AX      ;Set the new count in the timer
MOV DX, 0x43    ;point DX to the timer subsystem CONTROL port
MOV AL, 0x06    ;enable timer (bit 1) and timer interrupts (bit 2) (0x02 | 0x04 == 0x06)
OUT DX, AL      ;start timer
STI

; Start an infinite loop (will be interrupted periodically)
Done:
JMP Done        

; Timer ISR: invoked by the timer subsystem every 1s
timerISR:          
INC     CX      ;Count the number of times the timer interrupt happens
IRET            ;Return from interrupt

.end start