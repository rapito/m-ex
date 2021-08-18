
#include "../slippi copy.h";

#define CHAT_MESSAGE_SET_LENGTH 10				// Max Messages allowed to be shown
#define CHAT_MAX_PLAYER_MESSAGES 4				// Max Messagees allowed per player
int ChatMessagesSet[CHAT_MESSAGE_SET_LENGTH]; 	// Max mesages allowed
int ChatMessagesLocalCount = 0;
int ChatMessagesRemoteCount = 0;

typedef struct ChatWindow {
	JOBJDesc* jobj;
} ChatWindow;

typedef struct SlpCss {
	ChatWindow* chatWindow;
	JOBJSet* chatMessage;
} SlpCss;

enum ChatMessageState
{
	CHAT_MSG_STATE_NONE,
    CHAT_MSG_STATE_STARTING,	// Message is animating to show
    CHAT_MSG_STATE_IDLE,		// Message finished animating and is just idle
    CHAT_MSG_STATE_CLEANUP,		// MEssage is animating to hide and exit
} ChatMessageState;

typedef struct ChatMessage {
	JOBJSet* jobjSet;						
	int id; 					// ID of this message
	int framesLeft; 			// Amount of frames the message is displayed for 
	int state; 					// Initial message state
	Text* text;					// Created Text Object
	int playerIndex;			// Player Index
	int messageId;				// Message Id
} ChatMessage;


#define CHAT_FRAMES 20 
ChatMessage* CreateChatMessage(int playerIndex, int messageId){
	ChatMessage* msg = calloc(sizeof(ChatMessage));
	msg->framesLeft = CHAT_FRAMES;
	msg->state = CHAT_MSG_STATE_STARTING;
	msg->playerIndex = playerIndex;
	msg->messageId = messageId;
	return msg;
}

// EXI Transfer Modes
typedef enum EXI_TX_MODE { 
	EXI_TX_READ,
	EXI_TX_WRITE,
} EXI_TX_MODE;

int Inputs_GetPlayerInstantInputs(int port);

void* (*EXITransferBuffer)(void* buffer, int bufferSize, EXI_TX_MODE txMode) = (void *)0x800055f0;

Text* (*createSlippiPremadeText)(int playerIndex, int messageId, int textType, int GXLinkPriority, float x, float y, float z, float scale) = (void *)0x800056b4;
int (*createSubtext)(Text* text, GXColor* color, int textType, int outlineColor, char** strArray, float scale, float x, float y, float innerTextY, float outlineSize) = (void *)0x800056b4;

enum MSG_COLOR_NAMES {
	MSG_COLOR_WHITE,
	MSG_COLOR_RED, 
	MSG_COLOR_BLUE,
	MSG_COLOR_YELLOW,
	MSG_COLOR_GREEN,
	MSG_COLOR_CHAT_WINDOW,
	MSG_COLOR_CHAT_WINDOW_IDLE,
};

static GXColor MSG_COLORS[] = {
    {255, 255, 255, 255},
	{229, 76, 76, 255},
	{59, 189, 255, 255},
	{255, 203, 4, 255},
	{0, 178, 2, 255},
	{255, 234, 47, 255},
	{201, 195, 135, 255},
};

#define CHAT_WINDOW_FRAMES 60*2.5 // 4 seconds at 60fps
#define CHAT_ALLOW_COMMAND_FRAMES 60*0.2 // 1.5 seconds at 60fps
typedef struct ChatWindowData {
	Text* text;			// Text Struct to show headers and labels
	SlpCss* slpCss;		// Slp CSS to pass around
	int groupId;		// Chat Group ID (Misc, Compliments), this holds the input value
	int framesLeft;		// FramesLeft before closing the window for inactivity
	int framesCounter;	// Frames Counter used to allow sending another chat command
} ChatWindowData;


enum CHAT_LABELS {
	CHAT_STR_HEADER,
	CHAT_STR_UP,
	CHAT_STR_LEFT,
	CHAT_STR_RIGHT,
	CHAT_STR_DOWN,
};

char ChatGroups[4][5][45] = {
	{
		"Common",
		"ggs",
		"one more",
		"brb",
		"good luck"
	},
	{
		"Compliments",
		"well played",
		"that was fun",
		"thanks",
		"too good"
	},
	{
		"Reactions",
		"oof",
		"my b",
		"lol",
		"wow"
	},
	{
		"Misc",
		"okay",
		"thinking",
		"let's play again later",
		"bad connection"
	}
};
char ChatGroup_Common[5][45] = {
	"Common",
	"ggs",
	"one more",
	"brb",
	"good luck"
};
char ChatGroup_Compliments[5][45] = {
	"Compliments",
	"well played",
	"that was fun",
	"thanks",
	"too good"
};
char ChatGroup_Reactions[5][45] = {
	"Reactions",
	"oof",
	"my b",
	"lol",
	"wow"
};
char ChatGroup_Misc[5][45] = {
		"Misc",
		"okay",
		"thinking",
		"let's play again later",
		"bad connection"
	};


bool isWidescreen(){
	bool res = R13_INT(R13_OFFSET_ISWIDESCREEN);
	// OSReport("isWidescreen ptr: %i\n", res);
	return res;
}

char * GetChatText(int groupId, int messageId, bool useMessageIndex){
	// OSReport("GetChatText groupId: %i, msgId: %i \n", groupId, messageId);

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

Text* CreateChatMessageText(ChatMessage* msg, JOBJ* jobj){
	// OSReport("UpdateChatMessage.InitializeText ID: %i, bg.Y: %f text.y: %f, y.diff: %f\n", msg->id, jobj->trans.Y, -23.25f, abs(-23.25f) - abs(jobj->trans.Y));
	SlippiCSSDataTable* dt = SLIPPI_CSS_DATA_REF->dt;
	
	// stc_textcanvas_first[0]->gxlink = 3; // Hack to change struct gxlink? initial value
	// Text* text = createSlippiPremadeText(1, 0x88, 2, 0, -29.5f, -23.25f - jobj->trans.Y, 5.0f, 0.04f);
	// msg->text = text;
	// stc_textcanvas_first[0]->gxlink = 1; // restore

	Text* text = Text_CreateText(0, 0);
	text->gobj->gx_link = 1;
	text->gobj->gx_pri = 127;
	msg->text = text;
	text->kerning = 1;
	text->align = 0;
	text->trans.Z = 5.0f;

	float textScale = 0.1;
	text->scale.X = textScale;
	text->scale.Y = textScale; 

	// Dolphin returns the group and mmessage id joined together so we need to split them
	int groupId = msg->messageId >> 4;
	int messageId = (groupId << 4) ^ msg->messageId;
	char* playerName = dt->msrb->p1Name; 
	// char* playerName = ((dt->msrb->p1Name)+(31*msg->playerIndex));

	char* name = strcat(playerName,": "); 
	char* message = GetChatText(groupId, messageId, false);
	float xPos = isWidescreen() ? -446.0f : -296.0f;
	float yPos = -252.0f + ((msg->id)*31.8f);
	int colorIndex = dt->msrb->localPlayerIndex+1; 
	float scale = 0.4f;

	// OSReport("UpdateChatMessage.InitializeText bg.Z: %f text.y: %f, y.diff: %f\n", jobj->trans.Z, yPos, abs(yPos) - abs(jobj->trans.Y));
	createSubtext(text, &MSG_COLORS[colorIndex], 0x0, 0, (char**){name}, scale, xPos, yPos, 0.0f, 0.0f);
	createSubtext(text, &MSG_COLORS[0], 0x0, 0, (char**){message}, scale, strlen(name)*6.8f + xPos, yPos, 0.0f, 0.0f);

	return text;
}

void UpdateChatMessage(GOBJ* gobj){
	ChatMessage* msg = gobj->userdata;
	JOBJ* jobj = (JOBJ*)gobj->hsd_object;
	int framesLeft = msg->framesLeft--;
	JOBJ_AnimAll(jobj);
	// OSReport("UpdateChatMessage ID: %i, FramesLeft: %i\n", msg->id, msg->framesLeft);

	// Skip update until timer runs out
	if(framesLeft>0) return;

	switch (msg->state)
	{
	case CHAT_MSG_STATE_STARTING:
		// Restart timer and move to next state
		msg->state = CHAT_MSG_STATE_IDLE;
		msg->framesLeft = CHAT_FRAMES*8.5; 
		
		// Show text here
		if(!msg->text){
			msg->text = CreateChatMessageText(msg, jobj);
		}
		
		break;
	case CHAT_MSG_STATE_IDLE:
		// Restart timer and move to next state
		msg->state = CHAT_MSG_STATE_CLEANUP;
		msg->framesLeft = CHAT_FRAMES; 

		// Destroy/Hide Text Here
		// OSReport("UpdateChatMessage.Idle: ptr: 0x%x", (void*)(msg->text+0x5c));
		// HSD_Free((void*)(msg->text+0x5c));
		Text_Destroy(msg->text);
		
		// Animate chat message to hide
		JOBJ_AddSetAnim(jobj, msg->jobjSet, 1);
		JOBJ_ReqAnimAll(jobj, 0.0f);
		break;
	case CHAT_MSG_STATE_CLEANUP:
		ChatMessagesSet[msg->id] = false;
		if(msg->playerIndex == SLIPPI_CSS_DATA_REF->dt->msrb->localPlayerIndex){
			ChatMessagesLocalCount--;
		} else {
			ChatMessagesRemoteCount--;
		}
		OSReport("Deleted Message with ID: %i\n", msg->id);
		GObj_Destroy(gobj);
		break;
	default:
		break;
	}

}

int GetNextChatMessageID(){
	for(int i=0;i<CHAT_MESSAGE_SET_LENGTH;i++){
		if(!ChatMessagesSet[i]){
			ChatMessagesSet[i] = true;
			OSReport("GetNextChatMessageID: %i\n", i);
			return i;
		}
	}
	return -1;
}

int CanAddNewMessage(){
	int activeMessages = 0;
	for(int i=0;i<CHAT_MESSAGE_SET_LENGTH;i++){
		if(ChatMessagesSet[i]) activeMessages++;
	}
	return activeMessages < CHAT_MAX_PLAYER_MESSAGES;
}

typedef struct OutgoingChatMessageBuffer {
	u8 cmd;			// SLIPPI EXI COMMAND
	u8 messageId;	// 0xYZ (Y = group id, Z=message id)
} OutgoingChatMessageBuffer;

#define spawn(X) calloc(sizeof(X)) 

void SendOutgoingChatCommand(int messageId)
{
	OutgoingChatMessageBuffer* buffer = spawn(OutgoingChatMessageBuffer);
	buffer->cmd = SLIPPI_CMD_SendChatMessage;
	buffer->messageId = messageId;
	EXITransferBuffer(buffer, sizeof(buffer), EXI_TX_WRITE);
}

#define CHAT_SOUND_NEW_MESSAGE 0xb7		// Global Sound ID
#define CHAT_SOUND_BLOCK_MESSAGE 0x3 	// Common Sound
#define CHAT_SOUND_OPEN_WINDOW 0x2 		// Common Sound
#define CHAT_SOUND_CANCEL_MESSAGE 0x0   // Common Sound


MatchStateResponseBuffer* MSRB(){
	return SLIPPI_CSS_DATA_REF->dt->msrb;
}

int GetRemotePlayerCount(){
 	int count = 0;
	MatchStateResponseBuffer* msrb = MSRB();
	if(msrb->localPlayerIndex != 0 && strlen(msrb->p1Name) >= 0) count++;
	else if(msrb->localPlayerIndex != 1 && strlen(msrb->p2Name) >= 0) count++;
	else if(msrb->localPlayerIndex != 2 && strlen(msrb->p3Name) >= 0) count++;
	else if(msrb->localPlayerIndex != 3 && strlen(msrb->p4Name) >= 0) count++;
 	return count;
}

void CreateAndAddChatMessage(SlpCss* slpCss, int playerIndex, int messageId){
	// TODO: Log and do nothing if message id is invalid
	MatchStateResponseBuffer* msrb = SLIPPI_CSS_DATA_REF->dt->msrb;
	bool isLocalMessage = playerIndex == msrb->localPlayerIndex;

	if(!isLocalMessage && ChatMessagesRemoteCount >= GetRemotePlayerCount()*CHAT_MAX_PLAYER_MESSAGES)
		// Don't allow adding more messages than the allowed max per player
		return;

	GOBJ* gobj = GObj_Create(0x4, 0x5, 0x80);
	JOBJ* jobj = JOBJ_LoadJoint(slpCss->chatMessage->jobj);
	ChatMessage* chatMessage = CreateChatMessage(playerIndex, messageId);
	
	int id;
	while(!CanAddNewMessage() || (id = GetNextChatMessageID()) < 0 ){} // stall
	chatMessage->id = id;
	chatMessage->jobjSet = slpCss->chatMessage;

	if(isWidescreen()) jobj->trans.X = -14.0f;
	float yMargin = 3.2f;
	jobj->trans.Y =  -(chatMessage->id*yMargin);  

	JOBJ_AddSetAnim(jobj, slpCss->chatMessage, 0);
	JOBJ_ReqAnimAll(jobj, 0.0f);

	SFX_Play(CHAT_SOUND_NEW_MESSAGE);

	if(isLocalMessage){
		ChatMessagesLocalCount++;
	} else {
		ChatMessagesRemoteCount++;
	}

	GObj_AddUserData(gobj, 0x4, HSD_Free, chatMessage);
	GObj_AddObject(gobj, 0x4, jobj);
	GObj_AddGXLink(gobj, GXLink_Common, 1, 128);
	GObj_AddProc(gobj, UpdateChatMessage, 0x4);
}

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

#define NULL ((void *)0)

void CloseChatWindow(JOBJ* jobj, ChatWindowData* data){
	JOBJ_SetFlagsAll(jobj, JOBJ_HIDDEN);
	Text* t = data->text;
	Text_Destroy(t);
	data->text = NULL;
	SLIPPI_CSS_DATA_REF->dt->chatWindowOpened = false;
}

int PadGetChatInput(HSD_Pad* pad){
	if(pad->down & PAD_BUTTON_DPAD_UP){
		return PAD_BUTTON_DPAD_UP;
	}
	else if(pad->down & PAD_BUTTON_DPAD_LEFT){
		return PAD_BUTTON_DPAD_LEFT;
	}
	else if(pad->down & PAD_BUTTON_DPAD_RIGHT){
		return PAD_BUTTON_DPAD_RIGHT;
	}
	else if(pad->down & PAD_BUTTON_DPAD_DOWN){
		return PAD_BUTTON_DPAD_DOWN;
	}
	return 0;
}

bool IsValidChatGroupId(int groupId){
	switch (groupId)
	{
	case PAD_BUTTON_DPAD_UP:
	case PAD_BUTTON_DPAD_LEFT:
	case PAD_BUTTON_DPAD_RIGHT:
	case PAD_BUTTON_DPAD_DOWN:
		return true;
	default:
		return false;
	}

	return false;
};

bool IsValidChatMessageId(int messageId){
	// For now use same logic
	return IsValidChatGroupId(messageId);
}


void CheckForIncomingMessages(SlpCss* slpCss){

	SlippiCSSDataTable* dt = SLIPPI_CSS_DATA_REF->dt;
	MatchStateResponseBuffer* msrb = dt->msrb;

	int messageId = 0;
	int playerIndex = msrb->chatMsgPlayerIndex;

	if(playerIndex >= 0){
	
		if(msrb->userChatMsgId){
			messageId = msrb->userChatMsgId;
		} else if(msrb->oppChatMsgId){
			messageId = msrb->oppChatMsgId;
		}
	}

	if(messageId <= 0) return;
	if(playerIndex < 0 || playerIndex > 3) return;
	
	int groupId = messageId >> 4; // 18 >> 4 == 1
	int finalMessageId = (groupId << 4) ^ messageId; // 18 XOR 10 == 8
	if(!IsValidChatGroupId(groupId) || !IsValidChatMessageId(finalMessageId))
	{
		OSReport("Invalid Chat Command: %i!\n", messageId);	
		return;
	}


	CreateAndAddChatMessage(slpCss, playerIndex, messageId);
}

void UpdateChatWindow(GOBJ* gobj)
{
	ChatWindowData* data = gobj->userdata;
	SlpCss* slpCss = data->slpCss;
	JOBJ* jobj = (JOBJ*)gobj->hsd_object;
	bool isHidden = (jobj->flags & JOBJ_HIDDEN) != 0;

	int framesLeft = data->framesLeft;
	data->framesCounter++; // Increase Frames Counter

	// Checks and adds incoming chat messages 
	CheckForIncomingMessages(slpCss);

	if(!data->text && !isHidden) {
		gobj->gx_pri = 128; // Hack to allow thing to be shown
		data->text = CreateChatWindowText(gobj, data->groupId);
	}

	// only decrease frame if open 
	if(!isHidden) data->framesLeft--;

	for(int playerIndex=0;playerIndex<4;playerIndex++)
	{
		HSD_Pad* pad = PadGet(playerIndex, PADGET_ENGINE);
		int chatInput = PadGetChatInput(pad);
		
		if(pad->down != 0){
			// OSReport("Player %i: Input!: %i", playerIndex, pad->down);
		}

		if(!isHidden && pad->down & PAD_BUTTON_B){
			SFX_PlayCommon(CHAT_SOUND_CANCEL_MESSAGE);
			CloseChatWindow(jobj, data);
			return;
		}
		
		
		if(chatInput > 0){

			if(isHidden) { // Should open chat window
				SFX_PlayCommon(CHAT_SOUND_OPEN_WINDOW);
				JOBJ_ClearFlagsAll(jobj, JOBJ_HIDDEN);
				data->framesLeft = CHAT_WINDOW_FRAMES;
				data->groupId = chatInput;
				SLIPPI_CSS_DATA_REF->dt->chatWindowOpened = true;
				return;
			} else { // Should close and send chat message
				// OSReport("CanAddNewMessage() %i\n", CanAddNewMessage());
				if(data->framesCounter < CHAT_ALLOW_COMMAND_FRAMES) {
					return;
				} else if(CanAddNewMessage()) {
					data->framesCounter = 0; // Reset frames counter since last message sent
					chatInput = (data->groupId << 4) + chatInput;
					SendOutgoingChatCommand(chatInput);
					// CreateAndAddChatMessage(slpCss, playerIndex, chatInput);
					CloseChatWindow(jobj, data);
					return;
				} else {
					SFX_PlayCommon(CHAT_SOUND_BLOCK_MESSAGE);
					return;
				}

			}
		}
	}

	
	if(data->framesLeft<=0){
		CloseChatWindow(jobj, data);
	}
	
}


void CreateAndAddChatWindow() {
	// Initialize ChatMessageSet
	for(int i=0;i<CHAT_MESSAGE_SET_LENGTH;i++) ChatMessagesSet[i] = false;

	ArchiveInfo *archive = File_Load("slpCss.dat");
    SlpCss* slpCss = File_GetSymbol(archive, "slpCSS");
	ChatWindowData* data = calloc(sizeof(ChatWindowData));
	data->groupId = PAD_BUTTON_DPAD_UP;
	data->slpCss = slpCss;
	data->framesLeft = CHAT_WINDOW_FRAMES;
	data->framesCounter = CHAT_ALLOW_COMMAND_FRAMES; // Allow sending right away
	SLIPPI_CSS_DATA_REF->dt->chatWindowOpened = true;

	GOBJ* gobj = GObj_Create(0x4, 0x5, 0x80);
	JOBJ* jobj = JOBJ_LoadJoint(slpCss->chatWindow->jobj);

	JOBJ_SetFlagsAll(jobj, JOBJ_HIDDEN);
	jobj->trans.X = isWidescreen() ? -35.0f : -20.0f;
	jobj->trans.Y = -16.5f;

	GObj_AddObject(gobj, 0x4, jobj);
	GObj_AddUserData(gobj, 4, HSD_Free, data);
	GObj_AddGXLink(gobj, GXLink_Common, 1, 129);
	GObj_AddProc(gobj, UpdateChatWindow, 0x4);
}

void OnBoot(){


	for(int i=0;i<20;i++) {
		OSReport("OnBoot.Working: %i!!!!\n", i);
	}

	ArchiveInfo *archive = File_Load("slpCss.dat");
    SlpCss* slpCss = File_GetSymbol(archive, "slpCSS");

	GOBJ* gobj = GObj_Create(0x4, 0x5, 0x80);
	JOBJ* jobj = JOBJ_LoadJoint(slpCss->chatWindow->jobj);

	GObj_AddObject(gobj, 0x4, jobj);

	GObj_AddGXLink(gobj, GXLink_Common, 7, 128);
	GObj_AddProc(gobj, UpdateChatWindow, 0x4);
}

#define SCENE_SLIPPI_CSS 8
void OnSceneChange(){

	OSReport("OnSceneChange minor: %i major: %i\n", stc_scene_info->minor_curr, stc_scene_info->major_curr);
	OSReport("Testing again!");
	if(IsSlippiOnlineCSS()){
		OnCSSLoaded();
	} else {
		return;
	}
}

void OnCSSLoad(){
	OSReport("OnCSSLoad\n");
}

void OnCSSLoaded(){
	CreateAndAddChatWindow();
}

void OnSSSLoad(){

}

void OnTest(){
	int j = 5;
	OSReport("OnTest.OnTest\n");
	int x =1;
}

void UpdateCSS(){
	// OSReport("UpdateCSS\n");
}
