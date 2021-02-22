varA: dc.w 1,123

varB: dc.w 1,3

varC: dc.w 1,98

varD: ds.w 1

overFlowFlag: dc.b 1, 0


  org     $4000

  START:

  LDD #varA
  ADDD #varB


  BVC skipOverFlow1

  INC overFlowFlag

  SUBD #varC

  BRA finish

  skipOverFlow1:

  SUBD #varC




  BVC skipOverFlow2

  INC overFlowFlag

  skipOverFlow2:



  finish:


  STD varD


  END
