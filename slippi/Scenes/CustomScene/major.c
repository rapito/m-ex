#ifndef CUSTOM_SCENE_MAJOR_C
#define CUSTOM_SCENE_MAJOR_C
#include "major.h"

// Initialize minor data
static SharedMinorData minorData = {0};
static VSMinorData cssMinorData = {0};

// Initialize Minor Scene Array
static  MinorScene __attribute__((__used__)) minor_scene[] = {
    // Sub Menu 1
    {
        .minor_id = CUSTOM_SCENE_MINOR_SUBMENU_1,
        .heap_kind = HEAPKIND_UNK3,
        .minor_prep = ScenePrep,
        .minor_decide = SceneDecide,
        .minor_kind = COMMON_MNRKIND_CUSTOM_SCENE_SUBMENU,
        .load_data = &minorData,
        .unload_data = &minorData,
    },
    // Sub Menu 2
    {
        .minor_id = CUSTOM_SCENE_MINOR_SUBMENU_2,
        .heap_kind = HEAPKIND_UNK3,
        .minor_prep = ScenePrep,
        .minor_decide = SceneDecide,
        .minor_kind = COMMON_MNRKIND_CUSTOM_SCENE_SUBMENU,
        .load_data = &minorData,
        .unload_data = &minorData,
    },
    // Sub Menu 3
    {
        .minor_id = CUSTOM_SCENE_MINOR_SUBMENU_3,
        .heap_kind = HEAPKIND_UNK3,
        .minor_prep = ScenePrep,
        .minor_decide = SceneDecide,
        .minor_kind = MNRKIND_CSS,
        .load_data = &cssMinorData,
        .unload_data = &cssMinorData,
        },
};


void major_load(){
	OSReport("CustomScene.major_load!\n");

    // Setup minorData

	// Set first option as selected
	minorData.submenuIndex = 0;
	minorData.currentSubmenu = CUSTOM_SCENE_CURRENT_SUBMENU_NONE;
	minorData.pressedOptionOnExit = -1;
}

void major_exit(){
	OSReport("CustomScene.major_exit!\n");

}

/**
 * This is called before a minor scene is loaded.
 * @param scene
 */
void ScenePrep(MinorScene *scene){
    OSReport("CustomScene.major prep!\n");

    // We really don't have anything to do here, but you could do some stuff
    // with the minorData before passing it over to the next minor scene
    // if you don't own the code for it. (Like opening a Match Scene with some custom data)

    if(scene->minor_id == CUSTOM_SCENE_MINOR_SUBMENU_3) { // CSS prep
        Preload_Invalidate();
    }

}


/**
 * This is called when a minor exits.
 * You can Move to another minor inside your major scene,
 * or you can move to another major instead.
 * @param scene
 */
void SceneDecide(MinorScene *scene){
    OSReport("CustomScene.major decide!\n");
    SharedMinorData *data = scene->load_data;

    // We are going to make some control flow in here:
    // let's check the option that was pressed before exiting
    // the minor to determine where we want to move next.

    OSReport("CustomScene.SceneDecide PressedOptionOnExit: %i\n", data->pressedOptionOnExit);
    switch (data->pressedOptionOnExit) {
        case CUSTOM_SCENE_OPTION_GOTO_OPTION_1:
            Scene_SetNextMinor(CUSTOM_SCENE_MINOR_SUBMENU_2);
            data->currentSubmenu = CUSTOM_SCENE_CURRENT_SUBMENU_OPTION_1;
            break;
        case CUSTOM_SCENE_OPTION_GOTO_OPTION_2:
            Scene_SetNextMinor(CUSTOM_SCENE_MINOR_SUBMENU_3);
            data->currentSubmenu = CUSTOM_SCENE_CURRENT_SUBMENU_OPTION_2;
            break;
        case CUSTOM_SCENE_OPTION_BACK_TO_MAIN_MENU:
            Scene_SetNextMajor(MJRKIND_MNMA);
            Scene_ExitMajor();
            return;
        default:
            // This shouldn't really happen so let's just move
            // back to the same scene and log it.
            OSReport("How did you get here?\n");
            Scene_SetNextMinor(scene->minor_id);
            break;
    }

    Scene_ExitMinor();
    return;
}

#endif CUSTOM_SCENE_MAJOR_C

