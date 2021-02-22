
; Write the HC12 code corresponding to the C code. Let one exercise continue to the next.

	org $800

; #define NUM_BYTES 4
; unsigned byte byte1 = 1;
; unsigned byte byte2 = 2;
; unsigned byte byte3 = 3;
; unsigned byte byte4;

  NUM_BYTES:  EQU  4

  byte1: db 6
  byte2: db 9
  byte3: db 3
  byte4: rmb 1


;Moved things up here because of some interesting interactions
  sum: dw 0
  counter: dw 2
  maxCount: dw 100



; #define NUM_WORDS 4
; int word1 = 1;
; int word2 = 2;
; int word3

  NUM_WORDS: EQU 4

  word1: dw 8
  word2: dw 3
  word3: rmw 1

  END_STRING_CHAR: EQU '$'





; char string[] = "Look at the terminating character of this string"
; WARNING: C inserts an implicit \0 terminating character at the end of strings
;	   In HC12, you must manually insert this terminating character.
;	   Furthermore, the character is '$'

  string:   fcc "Look at the terminating character of this string"
	fcb '$'
;I thought it was '\0'...

	org $4000

	
; if statement: Set word3 to the minimum value of word1 and word2
; if word1 > word2, word3 = word2, else word3 = word1 ?

  LDD word1
  STD word3
  LDD word2
  LDX #word3
  EMINM 0, X


        
; if else statement: Set byte4 to the minimum value of byte1, byte2, and byte3.

  LDA byte1
  CMPA byte2
  BHI byte1Higher

  BRA compareByte3

  byte1Higher:
  LDA byte2

  compareByte3:

  CMPA byte3
  BHI byte3Lower
  
  STAA byte4
  
  BRA finishByteCompare
  
  byte3Lower:
  
  LDA byte3
  STAA byte4
  

  

	

; for statement : Find the sum of all even numbers between 0 and 100, inclusive
; You must write two versions of the solution: 
;           Once using Compare (C*) and Branch (B*) instrutions
;       and once using DBNE or IBNE or TBEQ
;
; _____  sum = 0;     // You must choose the data type of "sum" - byte or word, unsigned or signed
;
; for (reg int i = 2; i<100; i+=2) sum += i;


	finishByteCompare: 
	
	LDX maxCount
	LDD sum

	forLoopThings:
	
	ADDD counter
	
	STD sum
	
	INC counter
	INC counter ;Must be done twice to increase by 2

	CPX maxCount
	BEQ finishLooping
	BRA forLoopThings
	
	finishLooping:
	
	ADDD counter
	STD sum
  
	
	STS $3DFD
	
	
	
	;sum2: dw 0
	
	
	;;LDX maxCount
	;LDD sum2
	
	
	
	
	;ADDD
	
	;DBNE X
	
  
  
  
  







; while statement - Find the length of the string, by looking for its terminating character.
;     You must use an INDEXed addressing mode in your solution.
;
; ______ length = 0;    // You must choose the data type of "length" - byte or word, unsigned or signed
; while ( string[length] != '$' ) length++
;

; nested for-if statement - Find the location of the first character 'n' in the string. Set to -1 if none is found.
;	You must write two versions of this program
;	Once, using the length variable found in the previous part, combined with DBNE or IBNE
;   and Once, with the length unknown.







END





























