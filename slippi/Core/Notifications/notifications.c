#ifndef SLIPPI_CORE_NOTIFICATION_C
#define SLIPPI_CORE_NOTIFICATION_C
#include "notifications.h"
#include "chat_notifications.c"

GOBJ* _notificationsGOBJ = NULL;

void FreeNotifications(void* ptr){
	_notificationsGOBJ = NULL;
}

void InitNotifications(){
	if(GOBJ_IsAlive(_notificationsGOBJ)) return;
	OSReport("InitNotifications\n");

	for(int i=0;i<NOTIFICATION_MESSAGE_SET_LENGTH;i++) 
		NotificationMessagesSet[i] = false;

	GOBJ* gobj = GObj_Create(0x4, 0x5, 0x80);
    _notificationsGOBJ = gobj;

	GObj_AddUserData(gobj, 0x4, FreeNotifications, NULL);
	GObj_AddGXLink(gobj, GXLink_Common, 1, 129);
	GObj_AddProc(gobj, UpdateNotifications, 0x4);
}

/**
 * This method handles a proc that checks for notifications coming in
 * to popup on the screen.
 */
void ListenForNotifications(){
	InitNotifications();
    ListenForChatNotifications();
}

void UpdateNotifications(){
	// TODO: should check some static address queue and Create a NotificationMessage
}


void CreateAndAddNotificationMessage(SlpCSSDesc* slpCss, NotificationMessage* message){
	GOBJ* gobj = GObj_Create(0x4, 0x5, 0x80);
	JOBJ* jobj = JOBJ_LoadJoint(slpCss->chatMessage->jobj);
	
	int id;
	while(!CanAddNewMessage() || (id = GetNextNotificationMessageID()) < 0 ){} // stall
	message->id = id;
	message->jobjSet = slpCss->chatMessage;
	

	if(isWidescreen()) jobj->trans.X = -14.0f;
	float yMargin = 3.2f;
	jobj->trans.Y =  -(message->id*yMargin);  

	JOBJ_AddSetAnim(jobj, slpCss->chatMessage, 0);
	JOBJ_ReqAnimAll(jobj, 0.0f);

	GObj_AddUserData(gobj, 0x4, HSD_Free, message);
	GObj_AddObject(gobj, 0x4, jobj);
	GObj_AddGXLink(gobj, GXLink_Common, 1, 128);
	GObj_AddProc(gobj, UpdateNotificationMessage, 0x4);
}


void UpdateNotificationMessage(GOBJ* gobj){
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
		msg->framesLeft = msg->animationFrames*8.5; 

		if(msg->text){
			msg->text->hidden = false;
		}
		
		break;
	case SLP_NOT_STATE_IDLE:
		// Restart timer and move to next state
		msg->state = SLP_NOT_STATE_CLEANUP;
		msg->framesLeft = msg->animationFrames; 

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

/**
 * Finds next available notification message id
 */
int GetNextNotificationMessageID(){
	for(int i=0;i<NOTIFICATION_MESSAGE_SET_LENGTH;i++){
		if(!NotificationMessagesSet[i]){
			NotificationMessagesSet[i] = true;
			OSReport("GetNextNotificationMessageID: %i\n", i);
			return i;
		}
	}
	return -1;
}

/**
 * Checks if a new message can be added
 */
bool CanAddNewMessage(){
	int activeMessages = 0;
	for(int i=0;i<NOTIFICATION_MESSAGE_SET_LENGTH;i++){
		if(NotificationMessagesSet[i]) activeMessages++;
	}
	return activeMessages < CHAT_MAX_PLAYER_MESSAGES;
}


#endif SLIPPI_CORE_NOTIFICATION_C