#include "../../../MexTK/mex.h";

void prep();
void decide();

typedef struct MinorData
{
    u8 foo;
} MinorData;


static MinorData minor_data = {0}; // event select screen minor scene data


static  MinorScene __attribute__((__used__)) minor_scene[] = {
      // Menu
    {
        .minor_id = 0,
        .heap_kind = HEAPKIND_UNK3,
        .minor_prep = prep,
        .minor_decide = decide,
        .minor_kind = 81,
        .load_data = &minor_data,
        .unload_data = &minor_data,
    },
};

static ScDataVS major_data = {0};         // final VS data. each scene adds its data to this

void prep(MinorScene *scene){
	OSReport("Slippi major prep!\n");

}

void decide(MinorScene *scene){
	OSReport("Slippi major decide!\n");
    MinorData *data = scene->load_data;
    Scene_SetNextMajor(MJRKIND_MNMA);
    Scene_ExitMajor();

    return;
}

void major_load(){
	OSReport("Slippi major_load!\n");

     // init major data
//    memset(&major_data, 0, sizeof(major_data));
//    CSS_InitMajorData(&major_data);
//    CSS_ResetKOStars();
//
//     hacky but set main menu cursor values for returning to it
//    u8 *menu_data = 0x804a04f0;
//    menu_data[0] = 1; // 1p menu
//    menu_data[3] = 4; // training option
}

void major_exit(){
	OSReport("Slippi major_exit!\n");
}
