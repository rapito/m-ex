#ifndef CUSTOM_SCENE_MINOR_C
#define CUSTOM_SCENE_MINOR_C

#include "minor.h"

SharedMinorData* data;
Text* text;

//<editor-fold desc="Minor Scene Functions">
void minor_think(){
//	OSReport("CustomScene.minor_think!\n");

    if(!text) return;
    if(!data) return;

    // Color submenu that is currently hovered
//    Text_SetColor(text, data->submenuIndex, stc_shieldcolors[0]);
}

void minor_load(SharedMinorData* minorData){
	OSReport("CustomScene.minor_load!\n");
	data = minorData;
	if(data->currentSubmenu != CUSTOM_SCENE_CURRENT_SUBMENU_NONE)
	    OSReport("CustomScene.Title Option %i!\n", data->currentSubmenu);

	CommonSceneSetup(UpdateInput, "CustomScene.dat", "SceneData");

    // This requires a canvas and I'm not doing this today so...
    return;
//    Text_CreateCanvas(2, )
	text = Text_CreateText(0, 0);
	text->kerning = 1;
	text->align = 0;
	text->trans.Z = 0.0f;
	text->scale.X = 0.1f;
	text->scale.Y = 0.1f;

	Text_AddSubtext(text, 0.0f, 5.0f, "Option 1");
	Text_AddSubtext(text, 0.0f, 10.0f, "Option 2");
	Text_AddSubtext(text, 0.0f, 15.0f, "Back to Main Menu");
	OSReport("CustomScene.minor_load3!\n");

	// Based on the current submenu, add another text at the top saying so
	char* title;
    switch (data->currentSubmenu) {
        case CUSTOM_SCENE_CURRENT_SUBMENU_NONE:
            title = "";
            break;
        case CUSTOM_SCENE_CURRENT_SUBMENU_OPTION_1:
            title = "You are on Option 1!";
            break;
        case CUSTOM_SCENE_CURRENT_SUBMENU_OPTION_2:
            title = "You are on Option 2!";
            break;
        default:
            title = "How are you here anyway?";
            break;

    }
    Text_AddSubtext(text, 0.0f, 0.0f, title);
    OSReport("CustomScene.minor_load4!\n");

}

void minor_exit(SharedMinorData* minorData){
	OSReport("CustomScene.minor_exit!\n");
	data = 0;
	text = 0;
}
//</editor-fold>

//<editor-fold desc="Custom Scene Functions">
void ShowOptions(){

    if(!data) return;
    switch (data->submenuIndex) {
        case CUSTOM_SCENE_OPTION_GOTO_OPTION_1:
            OSReport("Press A To Set Title to Option 1.\n");
            break;
        case CUSTOM_SCENE_OPTION_GOTO_OPTION_2:
            OSReport("Press A To Set Title to Option 2.\n");
            break;
        case CUSTOM_SCENE_OPTION_BACK_TO_MAIN_MENU:
            OSReport("Press A To Go back to Main Menu.\n");
            break;
        default:
            OSReport("How?.\n");

            break;
    }
}

void UpdateInput(GOBJ* gobj){
    HSD_Pad* pad = PadGet(0, PADGET_ENGINE);

    if(pad->down & PAD_BUTTON_UP) {
        data->submenuIndex--;
        if(data->submenuIndex < min_submenu)
            data->submenuIndex = max_submenu;
//        OSReport("CustomScene.selected option: %i \n", data->submenuIndex);
        ShowOptions();
    } else if(pad->down & PAD_BUTTON_DOWN) {
        data->submenuIndex++;
        if(data->submenuIndex > max_submenu)
            data->submenuIndex = min_submenu;
//        OSReport("CustomScene.selected option: %i \n", data->submenuIndex);
        ShowOptions();
   } else if(pad->down & PAD_BUTTON_A) {
//        OSReport("CustomScene.pressed option: %i \n", data->submenuIndex);
        data->pressedOptionOnExit = data->submenuIndex; // Same as CUSTOM_SCENE_OPTION really
        Scene_ExitMinor(); // Exit Minor and execute SceneDecide
    }

}
//</editor-fold>

//<editor-fold desc="Common Init Functions">
void CommonSceneSetup(void* inputThinkFunction, char* GUIFilename, char* sceneDescSymbolName){
    GOBJ* gobj = GObj_Create(0x4, 0, 0x80);
    GObj_AddProc(gobj, inputThinkFunction, 0);

    ArchiveInfo* guiFile = File_Load(GUIFilename);
    SceneDesc* sceneDesc = File_GetSymbol(guiFile, sceneDescSymbolName);

    // Camera Setup
    GOBJ* camGobj = GObj_Create(2, 3, 0x80);
    COBJ* camera = COBJ_LoadDesc(sceneDesc->cameras[0]);
    GObj_AddObject(camGobj, 1, camera);
    GOBJ_InitCamera(camGobj, CommonCamInit, 0);
    GObj_AddProc(camGobj, MainMenu_CamRotateThink, 5);
    *stc_css_cobjdesc = sceneDesc->cameras[0];
    camGobj->cobj_links = 0x1f;
    // End camera setup

    // Fog Setup
    GOBJ* fogGobj = GObj_Create(0xe, 2, 0);
    HSD_Fog* fog = Fog_LoadDesc(sceneDesc->fog->fogDesc); //= 80c1ae80
    GObj_AddObject(fogGobj, 4, fog);
    GObj_AddGXLink(fogGobj, GXLink_Fog, 0, 0x80);

    // Light Setup
    GOBJ* lobjGobj = GObj_Create(3, 4, 0x80);
    void* lobjs = LObj_LoadAll(sceneDesc->lights);
    GObj_AddObject(lobjGobj, 2, lobjs);
    GObj_AddGXLink(lobjGobj, GXLink_LObj, 0, 0x80);
}

void CommonCamInit(GOBJ* gobj){
    COBJ* camera = gobj->hsd_object;
    int res = CObj_SetCurrent(camera);

    if(!res){
        return;
    }

    CObj_SetEraseColor(0x00,0,0,0xFF);
    CObj_EraseScreen(camera, 1, 0, 1);
    CObj_RenderGXLinks(gobj, 7);
    CObj_EndCurrent();
}
//</editor-fold>

#endif CUSTOM_SCENE_MINOR_C


