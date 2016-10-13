; SYSC2001 - Lab 3 bonus
; Program to complete 8-bit multiplication
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
	mov AL, [X]
					; Init BH (M) = Y
	mov BH, [Y]

	mul BH

quit:
	HLT
.END	Init





