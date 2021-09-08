#ifndef SLIPPI_CSS_SHEIK_SELECTOR_C
#define SLIPPI_CSS_SHEIK_SELECTOR_C
#include "sheik_selector.h"


#define CHAT_WINDOW_FRAMES 60*2.5           // 4 seconds at 60fps
#define CHAT_ALLOW_COMMAND_FRAMES 60*0.2    // 1.5 seconds at 60fps

GOBJ* _sheikSelectorMainGOBJ = NULL;

void FreeSheikSelector(void* ptr){
	_sheikSelectorMainGOBJ = NULL;
    if(ptr) HSD_Free(ptr);
}


void InitSheikSelector(){
    // OSReport("ListenForChatInput 0x%x 0x%x 0x%x 0x%x 0x%x\n",_sheikSelectorMainGOBJ, _sheikSelectorMainGOBJ->previous, _sheikSelectorMainGOBJ->next, _sheikSelectorMainGOBJ->previousOrdered, _sheikSelectorMainGOBJ->nextOrdered);
    if(GOBJ_IsAlive(_sheikSelectorMainGOBJ)) return;

	GOBJ* gobj = GObj_Create(0x4, 0x5, 0x80);

    _sheikSelectorMainGOBJ = gobj;

	GObj_AddUserData(gobj, 0x4, FreeSheikSelector, NULL);
	GObj_AddGXLink(gobj, GXLink_Common, 1, 0x80);
	GObj_AddProc(gobj, UpdateSheikSelector, 0x4);
}

void JOBJ_SetAllAlpha(JOBJ* jobj, float alpha){

    if(!jobj) return;

    DOBJ* dobj = jobj->dobj;
    while(dobj){
        if(dobj->mobj && dobj->mobj->mat)
            dobj->mobj->mat->alpha = alpha;
        dobj= dobj->next;
    }

    JOBJ_SetAllAlpha(jobj->child, alpha);
    JOBJ_SetAllAlpha(jobj->sibling, alpha);
}


void* (*MOBJ_SetFlags)(MOBJ* mobj, u32 flags) = 0x80362D38;
void* (*MOBJ_ClearFlags)(MOBJ* mobj, u32 flags) = 0x80362D50;
/**
 * Checks for input and handles chat window
 */
void UpdateSheikSelector(){
    OSReport("UpdateSheikSelector \n");


    u8 selectedFighter = ACCESS_U8(0x8043208f);

    JOBJ* sheikJobj;
    JOBJ* zeldaJobj;

    JOBJ_GetChild(stc_css_menumodel[0], &sheikJobj, 0x35, -1);
    JOBJ_GetChild(stc_css_menumodel[0], &zeldaJobj, 0x36, -1);

    if(!(selectedFighter == CKIND_ZELDA || selectedFighter == CKIND_SHEIK)) {
        
        JOBJ_SetFlagsAll(sheikJobj, JOBJ_HIDDEN);
        JOBJ_SetFlagsAll(zeldaJobj, JOBJ_HIDDEN);

        return;
    }
    

    JOBJ* selected; // HACK: Let's use this to still mobj's texp and tev desc to allow alpha channels on out jobjs
    JOBJ* unselected; // HACK: Let's use this to still mobj's texp and tev desc to allow alpha channels on out jobjs
    JOBJ* hovered; // HACK: Let's use this to still mobj's texp and tev desc to allow alpha channels on out jobjs

    bool prevHoveredJobj = NULL;

    JOBJ_GetChild(stc_css_menumodel[0], &selected, 0x37, -1);
    JOBJ_GetChild(stc_css_menumodel[0], &unselected, 60, -1);
    JOBJ_GetChild(stc_css_menumodel[0], &hovered, 0x38, -1);

    JOBJ_ClearFlagsAll(sheikJobj, JOBJ_HIDDEN);
    JOBJ_ClearFlagsAll(zeldaJobj, JOBJ_HIDDEN);

    sheikJobj->trans.X = -11.0f;
    sheikJobj->trans.Y = -14.5f;

    zeldaJobj->trans.X = -11.0f;
    zeldaJobj->trans.Y = -17.5f;

    JOBJ_SetMatFrame(sheikJobj, 25.0f);
    JOBJ_SetMatFrame(zeldaJobj, 18.0f);


    if(sheikJobj->dobj->mobj->tevdesc == hovered->dobj->mobj->tevdesc)
        prevHoveredJobj = sheikJobj;
    else if(zeldaJobj->dobj->mobj->tevdesc == hovered->dobj->mobj->tevdesc)
        prevHoveredJobj = zeldaJobj;

    switch (selectedFighter)
    {
    case CKIND_ZELDA:
        
    
        sheikJobj->dobj->mobj->tevdesc = unselected->dobj->mobj->tevdesc;
        sheikJobj->dobj->mobj->texp = unselected->dobj->mobj->texp;
        break;
    case CKIND_SHEIK:
        zeldaJobj->dobj->mobj->tevdesc = unselected->dobj->mobj->tevdesc;
        zeldaJobj->dobj->mobj->texp = unselected->dobj->mobj->texp;
        break;
    default:
        return;
    }

    u8 playerIndex = R13_U8(-0x49B0);
    void* playerSelections = R13_PTR(-0x49F0)+(playerIndex*0x24);

    float x = stc_css_cursors[0]->pos.X;
    float y = stc_css_cursors[0]->pos.Y;
    JOBJ* toSelect = NULL;
    JOBJ* toUnselect = NULL;
    JOBJ* toHover = NULL;
    int charId = CKIND_ZELDA;
    if(x >= -16 && x <= -14){
        OSReport("IN X\n");
        if(selectedFighter != CKIND_SHEIK && y >= -20 && y <= -18){
            OSReport("IN Y\n");
            toHover = sheikJobj;
            charId = CKIND_SHEIK;
            toUnselect = zeldaJobj;
        } else if(selectedFighter != CKIND_ZELDA && y >= -22 && y <= -21){
         OSReport("IN Y\n");
            toUnselect = sheikJobj;
            toHover = zeldaJobj;
            charId = CKIND_ZELDA;
        }
    }


    if(toHover){
        // if not hovered
        if(!prevHoveredJobj)
            SFX_PlayCommon(0x2);

        HSD_Pad* pad = PadGet(playerIndex, PADGET_ENGINE);
        toHover->dobj->mobj->tevdesc = hovered->dobj->mobj->tevdesc;
        toHover->dobj->mobj->texp = hovered->dobj->mobj->texp;
        if(pad->down & PAD_BUTTON_A){
            toSelect = toHover;
        }
    }

    if(toSelect){
        toSelect->dobj->mobj->tevdesc = selected->dobj->mobj->tevdesc;
        toSelect->dobj->mobj->texp = selected->dobj->mobj->texp;

        toUnselect->dobj->mobj->tevdesc = unselected->dobj->mobj->tevdesc;
        toUnselect->dobj->mobj->texp = unselected->dobj->mobj->texp;


        // Assign char ID to player selections
        *(u8*)(playerSelections+0x70) = charId;

        // Required to play correct sound and show correct nameplate
        *(u8*)(0x803f0cc8+1) = charId;
        CSS_PlayFighterName(charId);
        CSS_UpdateCSP(0);
        return;
    }

}

#endif SLIPPI_CSS_SHEIK_SELECTOR_C