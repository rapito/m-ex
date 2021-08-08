#To be inserted at 801a4c94
.include "../../Globals.s"
.include "../Header.s"

#Original Line
  stw	r3, -0x4F74 (r13)

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
  # Offsets from FXStructPointer
  # 0xC => pointer to fn table
    # 0x0 index first method
    # 0x0 offset address first method 
    # 0x1 index  method
    # 0x168 offset  method 
    # 0x2 index  method
    # 0x68 offset  method 

  # 0x10 => num of entries

  # Find offset to OnSceneChange:
  lwz REG_FX_ARRAY, 0x80(sp)
  lwz r3, 0xC(REG_MEX_DATA) # FX Table
  lwz r3, 0x3*4(r3) # Offset to OnSceneChange
  add r3, r3, REG_FX_ARRAY # offset from fx array to OnSceneChange
  mtctr r3
  bctrl 

restoreall
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
