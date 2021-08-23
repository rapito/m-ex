#ifndef CUSTOM_SCENE_MINOR_H
#define CUSTOM_SCENE_MINOR_H

#include "../../../MexTK/mex.h"
#include "major.h"

#define min_submenu 0
#define max_submenu 2

// Structs
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

// Functions
void UpdateInput(GOBJ* gobj);

void CommonSceneSetup(void* inputThinkFunction, char* GUIFilename, char* sceneDescSymbolName);
void CommonCamInit();

#endif CUSTOM_SCENE_MINOR_H