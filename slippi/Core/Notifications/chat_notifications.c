#ifndef SLIPPI_CORE_CHAT_NOTIFICATION_C
#define SLIPPI_CORE_CHAT_NOTIFICATION_C
#include "../../common.h"
#include "chat_notifications.h"
#include "notifications.c"
#include "../../CSS/chat/text.c"

int ChatMessagesLocalCount = 0;
int ChatMessagesRemoteCount = 0;

GOBJ* _chatNotificationsGOBJ;

void FreeChatNotifications(void* ptr){
	_chatNotificationsGOBJ = NULL;
}

void ListenForChatNotifications(){
    if(GOBJ_IsAlive(_chatNotificationsGOBJ)) return;
	OSReport("ListenForChatNotifications\n");

	ChatMessagesLocalCount = 0;
	ChatMessagesRemoteCount = 0;

	GOBJ* gobj = GObj_Create(0x4, 0x5, 0x80);

    _chatNotificationsGOBJ = gobj;

	GObj_AddUserData(gobj, 0x4, FreeChatNotifications, NULL);
	GObj_AddGXLink(gobj, GXLink_Common, 1, 129);
	GObj_AddProc(gobj, UpdateChatNotifications, 0x4);
}

void UpdateChatNotifications(){
	SlippiCSSDataTable* dt = GetSlpCSSDT();
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


	CreateAndAddChatMessage(dt->SlpCSSDatAddress, msrb, playerIndex, messageId);
}

void CreateAndAddChatMessage(SlpCSSDesc* slpCss, MatchStateResponseBuffer* msrb, int playerIndex, int messageId){
	bool isLocalMessage = playerIndex == msrb->localPlayerIndex;
	if(!isLocalMessage && ChatMessagesRemoteCount >= GetRemotePlayerCount()*CHAT_MAX_PLAYER_MESSAGES)
		// Don't allow adding more messages than the allowed max per player
		return;

	NotificationMessage* chatMessage = CreateChatMessage(playerIndex, messageId);
	CreateAndAddNotificationMessage(slpCss, chatMessage);
	chatMessage->text = CreateChatMessageText(chatMessage);
	SFX_Play(CHAT_SOUND_NEW_MESSAGE);
	
	if(isLocalMessage){
		ChatMessagesLocalCount++;
	} else {
		ChatMessagesRemoteCount++;
	}
}

Text* CreateChatMessageText(NotificationMessage* msg){
	SlippiCSSDataTable* dt = SLIPPI_CSS_DATA_REF->dt;
	
	// stc_textcanvas_first[0]->gxlink = 3; // Hack to change struct gxlink? initial value
	// Text* text = createSlippiPremadeText(1, 0x88, 2, 0, -29.5f, -23.25f - jobj->trans.Y, 5.0f, 0.04f);
	// msg->text = text;
	// stc_textcanvas_first[0]->gxlink = 1; // restore

	Text* text = Text_CreateText(0, 0);
	text->gobj->gx_link = 1;
	text->gobj->gx_pri = 127;
	text->kerning = 1;
	text->align = 0;
	text->trans.Z = 5.0f;

	float textScale = 0.1;
	text->scale.X = textScale;
	text->scale.Y = textScale; 

	text->hidden = true; // hide by default

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

	createSubtext(text, &MSG_COLORS[colorIndex], 0x0, 0, (char**){name}, scale, xPos, yPos, 0.0f, 0.0f);
	createSubtext(text, &MSG_COLORS[0], 0x0, 0, (char**){message}, scale, strlen(name)*6.8f + xPos, yPos, 0.0f, 0.0f);

	return text;
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

void UpdateChatMessage(GOBJ* gobj){
	NotificationMessage* msg = gobj->userdata;
	JOBJ* jobj = (JOBJ*)gobj->hsd_object;
	int framesLeft = msg->framesLeft--;
	JOBJ_AnimAll(jobj);
	// OSReport("UpdateChatMessage ID: %i, FramesLeft: %i\n", msg->id, msg->framesLeft);

	// Skip update until timer runs out
	if(framesLeft>0) return;

	switch (msg->state)
	{
	case SLP_NOT_STATE_STARTING:
		// Restart timer and move to next state
		msg->state = SLP_NOT_STATE_IDLE;
		msg->framesLeft = CHAT_FRAMES*8.5; 
		
		// Show text here
		// if(!msg->text){
		// 	msg->text = CreateChatMessageText(msg, jobj);
		// }
		
		break;
	case SLP_NOT_STATE_IDLE:
		// Restart timer and move to next state
		msg->state = SLP_NOT_STATE_CLEANUP;
		msg->framesLeft = CHAT_FRAMES; 

		// Destroy/Hide Text Here
		// OSReport("UpdateChatMessage.Idle: ptr: 0x%x", (void*)(msg->text+0x5c));
		// HSD_Free((void*)(msg->text+0x5c));
		if(msg->text)
			Text_Destroy(msg->text);
		
		// Animate chat message to hide
		JOBJ_AddSetAnim(jobj, msg->jobjSet, 1);
		JOBJ_ReqAnimAll(jobj, 0.0f);
		break;
	case SLP_NOT_STATE_CLEANUP:
		NotificationMessagesSet[msg->id] = false;
		if(msg->playerIndex == GetSlpCSSDT()->msrb->localPlayerIndex){
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

#endif SLIPPI_CORE_CHAT_NOTIFICATION_C