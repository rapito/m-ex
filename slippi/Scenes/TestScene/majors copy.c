#include "../../../MexTK/mex.h";

void Menu_Prep();
void Menu_Decide();

typedef struct CustomMinorData
{
    u8 leave_kind; // 0 = back, 1 = forward
    u8 page;
} CustomMinorData;

static CustomMinorData __attribute__((__used__)) minorData = {0}; // event select screen minor scene data

// Minor scene table
static  MinorScene __attribute__((__used__)) minor_scene[] = {
    // Menu
    {
        .minor_id = 0,
        .heap_kind = HEAPKIND_UNK3,
        .minor_prep = Menu_Prep,     //0x801b14a0,
        .minor_decide = Menu_Decide, //0x801b14dc,
        .minor_kind = MNRKIND_CSS,
        .load_data = &minorData,
        .unload_data = &minorData,
    },
};

void major_load(){
	OSReport('Slippi major_load!\n');
}

void major_exit(){
	OSReport('Slippi major_exit!\n');
}

void css_load(){
	OSReport('Slippi css_load!\n');
}

void sss_load(){
	OSReport('Slippi sss_load!\n');
}

// Menu Functions
void Menu_Prep(MinorScene *minor_scene)
{

    CustomMinorData *data = minor_scene->load_data;

    // reset leave_kind
    data->leave_kind = 0;

    return;
}
void Menu_Decide(MinorScene *minor_scene)
{

    CustomMinorData *data = minor_scene->load_data;

    // back to main menu
    if (data->leave_kind == 1)
    {
        Scene_SetNextMajor(1);
        Scene_ExitMajor();
    }
    // go to CSS
    else if (data->leave_kind == 2)
    {
        Scene_SetNextMinor(1);
        Scene_ExitMinor();
    }

    return;
}
