#ifndef CUSTOM_SCENE_MAJOR_H
#define CUSTOM_SCENE_MAJOR_H
#include "../../../MexTK/mex.h"


// Internal Major's Minor Scene IDs
enum CUSTOM_SCENE_MINOR_KIND {
    CUSTOM_SCENE_MINOR_SUBMENU_1,
    CUSTOM_SCENE_MINOR_SUBMENU_2,
    CUSTOM_SCENE_MINOR_SUBMENU_3,
};

// Common Minor Kind ID (this is the one that needs to be overriden on MxDT)
#define COMMON_MNRKIND_CUSTOM_SCENE_SUBMENU 85

typedef enum CUSTOM_SCENE_CURRENT_SUBMENU {
    CUSTOM_SCENE_CURRENT_SUBMENU_NONE,
    CUSTOM_SCENE_CURRENT_SUBMENU_OPTION_1,
    CUSTOM_SCENE_CURRENT_SUBMENU_OPTION_2
} CUSTOM_SCENE_CURRENT_SUBMENU;


typedef enum CUSTOM_SCENE_OPTION {
    CUSTOM_SCENE_OPTION_GOTO_OPTION_1,
    CUSTOM_SCENE_OPTION_GOTO_OPTION_2,
    CUSTOM_SCENE_OPTION_BACK_TO_MAIN_MENU
} CUSTOM_SCENE_OPTION;

// Structs
typedef struct SharedMinorData
{
    int submenuIndex;                               // Currently selected submenu index option.
    CUSTOM_SCENE_CURRENT_SUBMENU currentSubmenu;    // Identifies in which submenu the minor scene is at right now
    CUSTOM_SCENE_OPTION pressedOptionOnExit;        // Option Item that was pressed on the menu before exiting the minor scene.
} SharedMinorData;


void ScenePrep(MinorScene* minor);
void SceneDecide(MinorScene* minor);


#endif CUSTOM_SCENE_MAJOR_H