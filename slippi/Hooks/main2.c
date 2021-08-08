#include "../MexTK/mex.h";

void OnBoot(){
	OSReport("OnBoot!\n");
}

void OnSceneChange(){
	OSReport("OnSceneChange!\n");
}

void OnCSSLoad(){
	OSReport("OnCSSLoad!\n");
}

void OnSSSLoad(){
	OSReport("OnSSSLoad!\n");
}

// 1 - make custom asm hook and compile -> codes.gct  / codes.txt
// 2 - Create the dat file with the code
// 3 - Move dat to FS