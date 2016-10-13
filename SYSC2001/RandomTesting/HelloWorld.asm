
; This should be a comment. 
; The below code is (hopefully) a hello world program.
	

.org 3000;
testVariable: .dw -5
	

;[DI] contains the value of the next location in the stack. DI is the head of the stack

;Adds another space to the list.
increaseTestVarSizeOne:
;;{
	push BX
	
	mov BX, [DI]
	add BX, 0001h
	mov [DI], BX
	
	pop BX
	
	ret
;;}

;Reduces the size of the stack.
decreaseTestVarSizeOne:
;;{
	push BX
	
	mov BX, [DI]
	add BX, 0001h
	
	cmp		[DI], BX	; If the value of DX is less than DX + 1 (IE, list is empty, skip doing things.)
	jle 	skipDecreaseTestVarValue

	mov BX, [DI]
	sub BX, 0001h
	mov [DI], BX
	
	pop BX
	
	skipDecreaseTestVarValue:
	
	ret
;;}



;Adds spaces to the list equal to the value in SI.
increaseTestVarSize:
;;{
	startIncreaseTestVarSizeLoop:
	
	call increaseTestVarSize
	call increaseTestVarSize
	ret
;;}

;Reduces the size of the stack.
decreaseTestVarSizeTwo:
;;{
	push BX
	
	mov BX, [DI]
	add BX, 2
	
	cmp		[DI], BX	; If the value of DX is less than DX + 2 (IE, list is empty, skip doing things.)
	jle 	skipDecreaseTestVarValue

	mov BX, [DI]
	sub BX, 2
	mov [DI], BX
	
	pop BX
	
	skipDecreaseTestVarValue:
	
	ret
;;}

;Adds a value to the stack, and increases its size.
;Adds the value in the SI register, and takes the list head from DI
pushTestVar:
;;{

	push BX
	
	mov BX, [DI]
	mov [BX], SI     ;   Value of current end is changed to the passed value.
	call increaseTestVarSize
	
	pop BX
	
	ret
;;}


;Removes a value to the stack, and decreases its size.
;Leaves the value in the SI register. The former value of SI is left on the stack.
popTestVar:
;;{

	push SI
	push BX

	call decreaseTestVarSize
	
	mov BX, [DI]
	mov SI, [BX]     ;   Value of current end is changed to the passed value.

	
	pop BX
	
	ret
;;}



printList:







start:

        mov     DX, 04E9h	; load the display port I/O address into register DX (16 bits wide)
        ;mov     AL, 'H'		; load the ASCII value of the character 'H' into register AL (8 bits wide)
        ;out     DX, al		; send the ASCII character in AL out to the display at the port number in DX
		
        ;mov     AL, 'i'		; load the ASCII value of the character 'H' into register AL (8 bits wide)
        ;out     DX, al		; send the ASCII character in AL out to the display at the port number in DX
		
		
		mov DI, 1000h
		
		mov CX, 1002h
		mov [DI], CX
		mov SI, 0123h
		call pushTestVar
		mov SI, 4251h
		call pushTestVar
		mov SI, 4251h
		call pushTestVar
		mov SI, 4254h
		call pushTestVar
		mov SI, 4261h
		call pushTestVar
		mov SI, 4259h
		call pushTestVar
		
		
		mov 	CX, "an"
		push CX
		
		mov 	CX, "ri"
		push CX
		
		mov 	CX, "Do"
		push CX

		mov		CX, 0000h
		testt:
			add		CL, 1
			
			pop     AX  		; 
			
			;mov     CH, AH		; 
			;mov 	AH, AL
			;mov 	AL, CH

			out     DX, AL		; send the ASCII character in AL out to the display at the port number in DX
			
			mov 	AL, AH 		; swaps the characters in AX
			
			out 	DX, AL		; sends the second character to the display
			
			cmp		CL, 0002h	; Repeats 3 times.
			jle 	testt
		;call testfunc

		
        hlt					;  STOP the Libra CPU!
		
        .END     start				; Directive to assembler: this is the end of the program. The entry point of the program should be at the "start:" label
		
		
		
		
		