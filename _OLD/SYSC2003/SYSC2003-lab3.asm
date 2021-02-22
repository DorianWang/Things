



   org $0200
   ;MSG     FCC   'Hello World\0'

   testThing: EQU 08
   ;actuallyZero: EQU 0


   newArray:	fill	 testThing, 'A'

   newWordArray:  dw 100, 100, 100, 100, 100, 100, 100, 100
   
   
   byteArg:   db 1
   wordArg:   dw 1
   
   byteArg1:   db 1
   byteArg2:   db 1
   wordArg1:   dw 1
   wordArg2:   dw 1
   
   bytePointerArg1:  db 1
   wordPointerArg1:  dw 1
   
   
   




   ;void f1();
   f1:
   RTS

   ;void f2 (byte arg);
   f2:
   RTS
   
   ;void f3(word arg);
   f3:
   LDD 2, SP
   RTS
   
   ;void f4(byte arg1, byte arg2)
   f4:
   LDAA 2, SP
   RTS
   
   ;void f5(word arg1, word arg2);
   f5:
   LDX 2, SP ;arg1
   LDY 4, SP ;arg2
   RTS
   
   ;byte f6(void);
   f6:
   RTS
   
   ;word f7 (byte arg)
   f7:
   LDAA 4, SP
   STD 2, SP
   RTS
   
   ;byte f8(word arg);
   f8:
   RTS
   
   ;word f9(byte arg1, byte arg2)
   f9:
   LDAA 2, SP ;arg1
   LDAB 3, SP ;arg2
   RTS
   
   ;byte f10(word arg1, word arg2);
   f10:
   LDX 3, SP ;arg1
   LDY 5, SP ;arg2
   STAA 2, SP ;return value
   RTS
   
   ;void f11(byte *arg1, word arg2);
   f11:
   LDX 2, SP ;arg1
   LDY 4, SP ;arg2
   RTS
   
   ;byte f12(word *arg1, byte arg2);
   f12:
   LDX 3, SP ;arg1
   LDA 5, SP ;arg2
   STAA 2, SP ;return
   RTS


   org $4000

   LDX #newArray
   LDA #0
   STA 7, X
   LDS #$3DFF


   
   LDX #newWordArray
   LDB #0
   LDY #testThing
   
   
   
   loopingNToZero:
   STY B, X
   INCB
   INCB
   
   DBNE Y, loopingNToZero
   
   

   jsr f1
   
   LDAB byteArg 
   jsr f2
   
   LDD wordArg
   PSHD
   jsr f3
   PULD ;Just to keep the stack small. Otherwise you would decrement the SP by 2 (?)
   
   LDAB byteArg1
   LDAA byteArg2
   PSHA
   jsr f4
   PULA
   
   LDD wordArg2
   PSHD
   LDD wordArg1
   PSHD
   jsr f5
   PULD
   PULD
   
   jsr f6
   ;Do things with B
   
   LDAA byteArg
   PSHA
   PSHD
   jsr f7
   PULD ;Return value
   ins ;Removing A without pulling, hopefully.
   
   LDD wordArg
   jsr f8
   
   LDAA byteArg1
   LDAB byteArg2
   PSHB
   PSHA
   jsr f9
   ins
   ins
   
   LDD wordArg2
   PSHD
   LDD wordArg1
   PSHD
   DES
   jsr f10
   PULA
   
   ins
   ins
   ins
   ins


   LDD wordArg2
   PSHD
   LDD #byteArg1
   PSHD
   jsr f11
   
   ins
   ins
   ins
   ins
   
   
   LDAA byteArg2
   PSHA 
   LDD #wordArg1
   PSHD
   jsr f12
   
   ins
   ins
   ins
   
   
   
   END



   
   
   
   
   
   
   
   








