

varA: dc.b 1,123

varB: dc.b 1,3

varC: dc.b 1,98

varD: ds.b 1

overFlowFlag: dc.b 1


  org     $4000

  START:

  LDAA #varA
  ADDA #varB


  BVC skipOverFlow1

   LDAB #overFlowFlag
  INCB 
  STAB overFlowFlag

  LDAB #varC
  SBA 

  BRA finish

  skipOverFlow1:

  LDAB #varC
  SBA 




  BVC skipOverFlow2

   LDAB #overFlowFlag
  INCB 
  STAB overFlowFlag

  skipOverFlow2:



  finish:


  STAA varD


  END






