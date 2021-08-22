#include "../../../MexTK/mex.h";
#include "../../common.h"
#include "../../slippi.h"

#include "../../Core/Notifications/notifications.c"
#include "../../CSS/Chat/chat.c"

void minor_think(){
	OSReport("Slippi minor_think!\n");
}

typedef struct FogAdjDesc {
	void *fogDesc;
	void **unksArr;
} FogAdjDesc;

typedef struct SceneDesc {
	JOBJSet** jobjs;
	COBJDesc** cameras;
	LOBJSet** lights;
	FogAdjDesc *fog;
} SceneDesc;

void inputThinkFunction(GOBJ* gobj){

}

void camInitFunction(GOBJ* gobj){
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

void minor_load(){
	OSReport("Slippi minor_load!\n");

	GOBJ* gobj = GObj_Create(0x4, 0, 0x80); // = 80c237a0
	void* thinkFn = 0x80c1eaf8; // This is maybe the main input proc?
	GObj_AddProc(gobj, inputThinkFunction, 0);

	ArchiveInfo* guiFile = File_Load("SlippiScenesGUI.dat"); //= 80cf4600
	SceneDesc* sceneDesc = File_GetSymbol(guiFile, "SceneData"); //= 80ce8d60

	// Camera Setup
	GOBJ* camGobj = GObj_Create(2, 3, 0x80); //= 80cf4680
	COBJ* camera = COBJ_LoadDesc(sceneDesc->cameras[0]); //= 80c1ada0
	GObj_AddObject(camGobj, 1, camera);
	void* camInitFn = 0x80c1ec34; // Camera init function?
	GOBJ_InitCamera(camGobj, camInitFunction, 0);
	GObj_AddProc(camGobj, MainMenu_CamRotateThink, 5);

	// TODO: figure this out
	// This seems like a hack to force the main camera to be our camera
	// and then forcing our cam gobj to use somce specific cobj links, (whatever those are)
	stc_css_cobjdesc = sceneDesc->cameras[0];
	OSReport("CamObj 0x%x, 0x%x\n", camGobj->cobj_links, camGobj->cobj_links+0x4);
	camGobj->cobj_links = 0x000000000000002f;
	OSReport("CamObj 0x%x, 0x%x\n\n", camGobj->cobj_links, camGobj->cobj_links+0x4);
	// End camera setup

	// Fog Setup
	GOBJ* fogGobj = GObj_Create(0xe, 2, 0); //= 80cf47e0
	HSD_Fog* fog = Fog_LoadDesc(sceneDesc->fog->fogDesc); //= 80c1ae80
	GObj_AddObject(fogGobj, 4, fog);
	GObj_AddGXLink(fogGobj, GXLink_Fog, 0, 0x80);

	// Light Setup
	GOBJ* lobjGobj = GObj_Create(3, 4, 0x80); //= 80cf4880
	void* lobjs = LObj_LoadAll(sceneDesc->lights); //= 80c1aea0
	GObj_AddObject(lobjGobj, 2, lobjs);
	GObj_AddGXLink(lobjGobj, GXLink_LObj, 0, 0x80);


	JOBJ* jobj = sceneDesc->jobjs[0];
	JOBJ_LoadSet(false, sceneDesc->jobjs[0], 0, 0.0f, 3, 1, true, GObj_Anim);
	JOBJ_LoadSet(false, sceneDesc->jobjs[1], 0, 0.0f, 3, 1, true, GObj_Anim);

	ListenForNotifications();
    ListenForChatInput();
}


void minor_exit(){
	OSReport("Slippi minor_exit!\n");
}

