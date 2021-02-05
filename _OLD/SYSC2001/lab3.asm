; SYSC2001 - Lab 3
; Program to complete 8-bit unsigned shift & add multiplication
; AH = A; CH = Q; BH = M; Final 16-bit result in AX

.ORG 0000h
Data:
	Y:	.DB	15		; Multiplicand
	X: 	.DB	73		; Multiplier
	
.ORG 0010h
Init:
					; Initialize AX to zero. AH serves as accumulator and AX will hold product
	mov AX, 0000h
					; Initialize CH (Q) = X
	mov CH, [X]
					; Init BH (M) = Y
	mov BH, [Y]
					; Init DL as a loop counter with number of iterations required
	mov DL, 8h
	
mainLoop:
					; Shift out the lsb of the multiplier (Q[0]) into the carry flag
	rcr CH, 1
					; Check the carry flag: If Q[0] was not set, skip over Add and just shift
	jnc shift
AddM:
					; A = A + M
	add AH, BH
shift:
					; Shift AH and AL (16-bit result will be here eventually). Also need to shift CF into MSb of AH...
	rcr AX, 1
					; Decrement loop counter
	dec DL
					; If loop counter reaches zero, quit, else, loop back
	jnz mainLoop
quit:
	HLT
.END	Init





















