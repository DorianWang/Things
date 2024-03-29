;
;	SYSC2001 Lab1
;
; Note that the assembler ignores anything that follows a semicolon
; This is how we add comment lines and comments to lines of code
; It is very good practice to document your programs well.  Place
;  comments at the beginning to explain the purpose of the program
; Place comments at the end of most lines of code unless the purpose
;  of the statement is obvious.
;
; The following statement is known as an 'Equate' statement
; It simply equates the symbol name to a value.  The symbol may then
;  be used anywhere in the program and the value will be substituted
; The reason we do this is in case we want to later change the value
;  We need only change the value once at the beginning of the program
;  where it is defined
;
; In this case we are defining the symbol 'Display' to have the value
;  of 04E9h - the address of the Libra display screen
;
Display	.EQU 04E9h	; address of Libra display
;
;
;	Program: PrtStr - print an ASCII string
;	Prints a string of ASCII characters to the screen. Uses the BX register
; 	as a pointer, stepping along the string of letters, pointing to one letter
;	at a time. The string is delimited by a '$', so stop printing when BX is
; 	pointing at a '$' character. 
;
;	Data for PrtStr follows
;	.DB is used to Define one or more Bytes (.DB)
;	Here, the assembler will convert each letter to its ASCII value.
;	Take a look at the list file (PrtStr.lst) and you can see how the text is
;	encoded. Compare the values to the ASCII table posted on the course website.
Message: .DB 'Hello World!$'

; The next statement defines an address symbol.  Symbols must be unique
;  and always end with a colon (:).  The assembler calculates the address
;  of the symbol and inserts the actual address in your code whenever
;  you use this symbol
; In this case we are defining the entry point of the program itself 
; (this symbol must appear after the .END directive at the bottom of this program)
;  	
PrtStr:
;
; Before we enter the main printing loop, we must do some initialization
; First, we must initialize our pointer (BX) to point to the first character
; of our string. To do this, we use immediate addressing mode to set BX==Message,
; where Message is simply the address of the first character ('H')
; We also load the display port number into register DX

	mov BX, Message
	mov DX, Display
;
; The next statement defines another address - the beginning of the print loop
;
PrtChar:
;
; You need to load the next character to be printed into the AL register
; Since register BX is pointing at the character in memory, we use indirect addressing
;
	mov AL,[BX]
;
; Try to figure out how the CMP and JE statements work here (because you are
; going to have to figure out how to add statements such as these 
; in Part II of the lab). These 2 statements check for a '$' symbol in AL and jump to the  
; end of program label "EndPrt" (look for it!) if so.
;
; CMP a,b compares a to b. After the CMP instruction, you can include a conditional
; jump instruction. In this case, we have used the JE conditional jump, which means
; jump to the label "EndPrt" if a EQUALS b. 
;
; There are many other conditional jump instructions, that we can use after CMP instructions.
; For example, JAE will "jump if a is above or equal to b".
;
; Essentially, conditional jumps are used to "skip over" some instructions that come
; after the jump instruction, iff the conditions are met.
;
	cmp AL,'$'	; compare the character in AL to the character '$'
	JE EndPrt	; if they were EQUAL, we are done so jump to EndPrt
;
; Here is the 'out' statement which actually prints a single character
;
	out DX,AL	; print the char
	
incPtr:
	inc BX		; step along the string to the next character
;
; The next statement is an unconditional jump back to the beginning of the loop
; "Unconditional" means that the jump is ALWAYS taken (no CMP needed).
;
	jmp PrtChar	; loop back
	
;
; The next statement defines another address - the end of the program
;

EndPrt:
	HLT			; Stop the Libra processor

.END PrtStr		; This is the .END directive, which tells the assembler that your code
				; is done, and also tells the loader where to begin executing your 
				; code (in the OBJ file). Here we will begin execution at label 'PrtStr'