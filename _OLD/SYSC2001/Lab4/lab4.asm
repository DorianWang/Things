DisplayPort .EQU 04E9h
ApplicationNumber .EQU 42



.ORG 0010h
AppNums:
	.DB 85
	.DB 28
	.DB 8 
	.DB 91
    .DB 2  
	.DB 40
	.DB 42

.org 0100h
SadMsg:
 .DB "No go Mars...$"

HappyMsg:
 .DB "I go Mars at: $"

ListEnd:
 .DW 0017h
 
.org 0200h
printLargeNumber:
	push AX
	push BX
	push CX
	
	mov BX, SP
	
	mov AX, [BX + 8]
	mov BX, 0000h
	mov CL, 0Ah
	
	loopGetLargeNumber:
	div CL

	push AX
	inc BX
	
	cmp AL, 00h
	ja loopGetLargeNumber
	
	mov AL, AH
	mov AH, 00h
	
	loopPrintLargeNumber:

	pop AX
	add AH, '0'
	mov AL, AH
	out DX, AL
	
	dec BX
	
	cmp BX, 0000h
	ja loopPrintLargeNumber
	
	pop CX
	pop BX
	pop AX
ret 2

printMessage:
	push AX
	push BX
	push CX
	
	mov BX, SP
	mov CX, [BX + 8]
	mov BX, HappyMsg
	cmp CX, 0h
	jne failure
		mov BX, SadMsg
	failure:
	
	mov AL, [BX]
	printMessageLoop:
	
	cmp AL, '$'
	je exitPrintMessageLoop
	
	out DX, AL
	
	inc BX	
	mov AL, [BX]

	jmp printMessageLoop
	
	exitPrintMessageLoop:
	
	cmp CX, 0h
	je skipPrintNumber
		push CX
		call printLargeNumber
	skipPrintNumber:
	
	pop CX
	pop BX
	pop AX
	ret

	
searchList:
	mov BX, AppNums
	mov CL, ApplicationNumber
	searchListLoop:
		cmp CL, [BX]
		je skipSearchListLoop
		inc BX
	cmp BX, ListEnd
	jb searchListLoop
	
	mov AX, 0000h
	ret
	skipSearchListLoop:
		sub BX, AppNums
		add BX, 1
		mov AX, BX
	
ret
	
main:

	mov DX, DisplayPort

	
	
	
	
	
	call searchList
	push AX
	call printMessage
	pop AX
	







quit:
	hlt
	
	
.END main

