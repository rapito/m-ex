#To be inserted at 8025b838
.include "../../Globals.s"
.include "../Header.s"

backupall
  .set REG_FX_ARRAY, 31
  .set REG_MEX_DATA, 30

  # Execute LoadRELDAT
  bl  FileName
  mflr  r3 # Hooks.dat
  addi  r4,sp,0x80 # FXStructPointer
  bl  SymbolName # bmFunction
  mflr  r5
  branchl r12, 0x803d709c # Standalone function LoadRELDAT
  mr REG_MEX_DATA, r4

  # Find offset to OnSSSLoad:
  lwz REG_FX_ARRAY, 0x80(sp)
  lwz r3, 0xC(REG_MEX_DATA) # FX Table
  lwz r3, 0x7*4(r3) # Offset to OnSSSLoad
  add r3, r3, REG_FX_ARRAY # offset from fx array to OnSSSLoad
  mtctr r3
  bctrl 

restoreall
mr r0, r30
b EXIT

FileName:
blrl
.string "Slippi.dat"
.align 2

SymbolName:
blrl
.string "slpFunction"
.align 2

EXIT:
  #Original Line
  lmw	r22, 0x0130 (sp)