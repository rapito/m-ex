#ifndef SLIPPI_CSS_MAIN_C
#define SLIPPI_CSS_MAIN_C
#include "main.h"

#include "../Core/Notifications/notifications.c"
#include "Chat/chat.c"
#include "Components/sheik_selector.c"

void UpdateOnlineCSS(){
    if(!IsSlippiOnlineCSS()) return;
    bp();

    InitSheikSelector();
    ListenForNotifications();
    ListenForChatInput();
}

#endif SLIPPI_CSS_MAIN_C
