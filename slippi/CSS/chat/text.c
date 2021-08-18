#ifndef SLIPPI_CSS_CHAT_TEXT_C
#define SLIPPI_CSS_CHAT_TEXT_C
#include "../../slippi.h"
#include "text.h"

char * GetChatText(int groupId, int messageId, bool useMessageIndex){
	OSReport("GetChatText groupId: %i, msgId: %i \n", groupId, messageId);

	char ** group; 
	int groupIndex = 0;

	switch (groupId)
	{
	case PAD_BUTTON_DPAD_UP:
		group = ChatGroup_Common;
		groupIndex = 0;
		break;
	case PAD_BUTTON_DPAD_LEFT:
		group = ChatGroup_Compliments;
		groupIndex = 1;
		break;
	case PAD_BUTTON_DPAD_RIGHT:
		group = ChatGroup_Reactions;
		groupIndex = 2;
		break;
	case PAD_BUTTON_DPAD_DOWN:
		group = ChatGroup_Misc;
		groupIndex = 3;
		break;
	default:
		groupIndex = -1;
		break;
	}

	int index = 0;

	if(useMessageIndex) 
	{
		index = messageId;
	}
	else {
		switch (messageId)
		{
		case PAD_BUTTON_DPAD_UP:
			index = 1;
			break;
		case PAD_BUTTON_DPAD_LEFT:
			index = 2;
			break;
		case PAD_BUTTON_DPAD_RIGHT:
			index = 3;
			break;
		case PAD_BUTTON_DPAD_DOWN:
			index = 4;
			break;
		default:
			index = messageId;
			break;
		}
	}


	OSReport("GetChatText s: %s\n", ChatGroups[groupIndex][index]);
	return ChatGroups[groupIndex][index];
};

Text* CreateChatWindowText(GOBJ* gobj, int groupId){
	

	Text* text = Text_CreateText(0, 0);
	// OSReport("text.gxLink: %i gxPri: %i", text->gobj->gx_link, text->gobj->gx_pri);

	text->kerning = 1;
	text->align = 0;
	text->trans.Z = 0.0f;
	text->scale.X = 0.1f;
	text->scale.Y = 0.1f; 

	float x = isWidescreen() ? -452.0f : -300.0f;
	float labelX = x + 15.0f;

	float offset = 0; // should change if widescreen
	
	char* title[30];
	sprintf(title, "Chat: %s", GetChatText(groupId, CHAT_STR_HEADER, true));
	// Create  Header
	createSubtext(text, &MSG_COLORS[MSG_COLOR_CHAT_WINDOW_IDLE], 0x0, 0, (char**){title}, 0.45f, x+offset, 79.0f, 0.0f, 0.0f);

	// Create Labels
	for(int i=CHAT_STR_UP;i<=CHAT_STR_DOWN;i++){
		char* label = GetChatText(groupId, i, true);
		float margin = 25.0f*(i+1); // starts with 2 lines from header
		float yPos = 79.0f+margin;

		// OSReport("yPos: %f", yPos);
		createSubtext(text, &MSG_COLORS[MSG_COLOR_WHITE], 0x0, 0, (char**){label}, 0.45f, labelX+offset, yPos, 0.0f, 0.0f);
	}

	return text;
}

#endif SLIPPI_CSS_CHAT_TEXT_C
