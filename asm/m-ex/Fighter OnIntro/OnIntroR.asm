#To be inserted @ 800bf20c
.include "../../Globals.s"
.include "../Header.s"

.set  REG_PlayerGObj,29
.set  REG_PlayerData,30

#Check for an onIntroR function
  lwz r0,OFST_FighterOnIntroR(rtoc)
  lwz r4,0x4(REG_PlayerData)
  mulli r4,r4,4
  lwzx  r12,r4,r0
  cmpwi r12,0
  beq Exit
#Branch to function
  mr  r3,REG_PlayerGObj
  mtctr r12
  bctrl

Exit:
  lwz r0,0x002C(r1)

