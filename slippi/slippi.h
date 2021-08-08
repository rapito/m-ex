#include "../MexTK/mex.h";
#include <stdbool.h>

#define PACK true

#ifdef PACK
#define packed(X) __attribute__((packed)) X
#else
#define packed(X) X
#endif

#define R13_OFFSET_ODB_ADDR -0x49e4					// Online data buffer
#define R13_OFFSET_SB_ADDR -0x503C					// Scene buffer, persists throughout scenes
#define R13_OFFSET_ONLINE_MODE -0x5060				// Byte, Selected online mode
#define R13_OFFSET_APP_STATE -0x505F					// Byte, App state / online status
#define R13_OFFSET_FORCE_MENU_CLEAR -0x505E			// Byte, Force menu clear
#define R13_OFFSET_NAME_ENTRY_MODE -0x505D			// Byte, 0 = normal, 1 = connect code
#define R13_OFFSET_SWITCH_TO_ONLINE_SUBMENU -0x49ec	// Function used to switch
#define R13_OFFSET_CALLBACK -0x5018					// Callback address
#define R13_OFFSET_ISPAUSE -0x5038					// byte, client paused bool (originally used for tournament mode @ 8019b8e4)
#define R13_OFFSET_ISWINNER -0x5037					// byte, used to know if the player won the previous match
#define R13_OFFSET_CHOSESTAGE -0x5036					// bool, used to know if the player has selected a stage
#define R13_OFFSET_NAME_ENTRY_INDEX_FLAG -0x5035		// byte, set to 1 if just entered name entry. Rsts direct code index.
#define R13_OFFSET_USE_PREMADE_TEXT -0x5014			// bool, used to make Text_CopyPremadeTextDataToStruct load text data from dolphin
#define R13_OFFSET_ISWIDESCREEN -0x5020				// bool, used to make Text_CopyPremadeTextDataToStruct load text data from dolphin

//###############################################################################
// EXI COMMAND Definitions
//###############################################################################
// For Slippi communication
#define SLIPPI_CMD_GetFrame, 0x76
#define SLIPPI_CMD_CheckForReplay  0x88
#define SLIPPI_CMD_CheckForStockSteal 0x89
#define SLIPPI_CMD_SendOnlineFrame 0xB0
#define SLIPPI_CMD_CaptureSavestate 0xB1
#define SLIPPI_CMD_LoadSavestate 0xB2
#define SLIPPI_CMD_GetMatchState 0xB3
#define SLIPPI_CMD_FindOpponent 0xB4
#define SLIPPI_CMD_SetMatchSelections 0xB5
#define SLIPPI_CMD_OpenLogIn 0xB6
#define SLIPPI_CMD_LogOut 0xB7
#define SLIPPI_CMD_UpdateApp 0xB8
#define SLIPPI_CMD_GetOnlineStatus 0xB9
#define SLIPPI_CMD_CleanupConnections 0xBA
#define SLIPPI_CMD_SendChatMessage 0xBB
#define SLIPPI_CMD_GetNewSeed 0xBC
#define SLIPPI_CMD_ReportMatch 0xBD
#define SLIPPI_CMD_SendNameEntryIndex 0xBE
#define SLIPPI_CMD_NameEntryAutoComplete 0xBF
// For Slippi file loads
#define SLIPPI_CMD_FileLength 0xD1
#define SLIPPI_CMD_FileLoad 0xD2
#define SLIPPI_CMD_GctLength 0xD3
#define SLIPPI_CMD_GctLoad 0xD4
// Misc
#define SLIPPI_CMD_GetDelay 0xD5
// For Slippi Premade Texts
#define SLIPPI_CMD_GetPremadeTextLength 0xE1
#define SLIPPI_CMD_GetPremadeText 0xE2

// ################################################################################
// # Match State Response Buffer
// ################################################################################

typedef struct packed(MatchStateResponseBuffer) {
	u8 connectionState;
	bool isLocalPlayerReady;
	bool isRemotePlayerReady;
	u8 localPlayerIndex;
	u8 remotePlayerIndex;
	u32 rngOffset;
	u8 delayFrames;
	u8 userChatMsgId;
	u8 oppChatMsgId;
	u8 chatMsgPlayerIndex;
	u32 VSLeftPlayers;
	u32 VSRightPlayers;
	char* localName[31];
	char* p1Name[31];
	char* p2Name[31];
	char* p3Name[31];
	char* p4Name[31];
	char* oppName[31];
	char* p1ConnectCode[10];
	char* p2ConnectCode[10];
	char* p3ConnectCode[10];
	char* p4ConnectCode[10];
	char* errorMessage[241];
	MatchInit gameInfoBlock;
} MatchStateResponseBuffer;

// ################################################################################
// # CSS Data Table
// ################################################################################
#define CSS_DATA_TABLE_BUFFER_ADDRESS 0x80005614

typedef struct packed(SlippiCSSDataTable) {
	MatchStateResponseBuffer* msrb;
	void* SlpCSSDatAddress;
	Text* textStructAddress;
	u8 spinner1;
	u8 spinner2;
	u8 spinner3;
	u16 frameCounter;
	bool prevLockInState;
	u8 prevConnectedState;
	u8 zButtonHoldTimer;
	bool chatWindowOpened;
	u16 chatLastInput;
	u8 chatMsgCount;
	u8 chatLocalMsgCount;
	u8 lastChatMsgIndex;
	u8 teamIndex;
	u8 teamCostumeIndex;
} SlippiCSSDataTable;

typedef struct SlippiCSSDataTableRef {
	SlippiCSSDataTable* dt;
} SlippiCSSDataTableRef;

const SlippiCSSDataTableRef* SLIPPI_CSS_DATA_REF = (SlippiCSSDataTableRef*) CSS_DATA_TABLE_BUFFER_ADDRESS; 

bool IsSlippiOnlineCSS(){
	return stc_scene_info->minor_curr == MNRKIND_TITLE && stc_scene_info->major_curr == MJRKIND_HANYUTESTCSS;
}