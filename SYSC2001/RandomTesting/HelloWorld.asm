
; This should be a comment. 
; The below code is (hopefully) a hello world program.
	
;;{
testfunc:
	mov		CX, 00FFh
	mov 	BX, 0F0Fh
	ret
;;}


;Adds another space to the list.
increaseTestVarSize:
;;{
	push CX
	
	mov CX, [1000h]
	add CX, 2h
	mov [1000h], CX
	
	pop CX
	
	ret
;;}

;Clobbers whatever is in the last spot of the list, and reduces the size. Also refills the former last spot with FFFF.
decreaseTestVarSize:
;;{
	cmp		[1000h], 1002h	; If 1000h is less than 1002h (IE, list is empty, skip doing things.)
	jle 	skipDecreaseTestVarValue
	
	push CX
	
	mov CX, [1000h]
	sub CX, 2
	mov [1000h], CX
	
	pop CX
	
	skipDecreaseTestVarValue:
	
	ret
;;}


;Adds a value to the stack, and increases its size.
;Uses the value contained in the SI register, no registers should be affected.
pushTestVar:
;;{
	mov CX, [1000h]
	mov [CX], SI
	call increaseTestVarSize
	
	ret
;;}


start:

        mov     DX, 04E9h	; load the display port I/O address into register DX (16 bits wide)
        ;mov     AL, 'H'		; load the ASCII value of the character 'H' into register AL (8 bits wide)
        ;out     DX, al		; send the ASCII character in AL out to the display at the port number in DX
		
        ;mov     AL, 'i'		; load the ASCII value of the character 'H' into register AL (8 bits wide)
        ;out     DX, al		; send the ASCII character in AL out to the display at the port number in DX
		
		mov CX, 1002h
		mov [1000h], CX
		mov SI, 0123h
		call pushTestVar
		mov SI, 4251h
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
		call testfunc

		
        hlt					;  STOP the Libra CPU!
		
        .END     start				; Directive to assembler: this is the end of the program. The entry point of the program should be at the "start:" label
		
		
		
		
		