
const int iLogAccountBase = 500;


enum
{
	MSGLOGINID_UnauthorizedMaintenanceLogin = 0,
	MSGLOGINID_Maintenance = -8,
	MSGLOGINID_UnknownErrorLogin = 0,
	MSGLOGINID_IncorrectPassword = -2,
	MSGLOGINID_IncorrectAccount = -1,
	MSGLOGINID_BlockedAccount = -3,
	MSGLOGINID_LoggedAccount = -4,
	MSGLOGINID_YouAreBanned = -6,
	MSGLOGINID_TempBanned = -18,
	MSGLOGINID_NotActivatedAccount = -16,
};

enum ELogAccount
{
	ACCLOGID_LoginSuccess							= iLogAccountBase + 1,
	ACCLOGID_IncorrectAccount						= iLogAccountBase + 2,
	ACCLOGID_IncorrectPassword						= iLogAccountBase + 3,
	ACCLOGID_BlockedAccount							= iLogAccountBase + 4,
	ACCLOGID_NotActivatedAccount					= iLogAccountBase + 5,
	ACCLOGID_CharacterSelectSend					= iLogAccountBase + 6,
	ACCLOGID_CharacterCreated						= iLogAccountBase + 7,
	ACCLOGID_CharacterDeleted						= iLogAccountBase + 9,
	ACCLOGID_UnblockedAccount						= iLogAccountBase + 10,
	ACCLOGID_CharacterSave							= iLogAccountBase + 11,
	ACCLOGID_CharacterExpLose						= iLogAccountBase + 12,
	ACCLOGID_TempBanAccount							= iLogAccountBase + 13,
	ACCLOGID_CharacterQuestGot						= iLogAccountBase + 301,
	ACCLOGID_CharacterQuestCancel					= iLogAccountBase + 302,
	ACCLOGID_CharacterQuestFinished					= iLogAccountBase + 303,
	ACCLOGID_CharacterQuestEXP						= iLogAccountBase + 304,
	ACCLOGID_CharacterQuestEXPLose					= iLogAccountBase + 305,
	ACCLOGID_CharacterItemTimerBeginner				= iLogAccountBase + 401,
	ACCLOGID_CharacterItemTimerChemist				= iLogAccountBase + 402,
	ACCLOGID_CharacterNameKicked					= iLogAccountBase + 501,

	// PostBox
	ACCLOGID_ReceivedPostBox						= iLogAccountBase + 8,

};

//											Definitions
#define LOGIN_SUCCESS_LOGACCTEXT						"[%s] (Login Success) -> Password[%s] MacAddr[%s] PCName[%s] SerialHD[%d] VideoName[%s] VideoGUID[%d] HardwareID[%s] FolderID[%d]"
#define INCORRECT_ACCOUNT_LOGACCTEXT					"Unknown_Account (Incorrect Account) -> AccountName[%s] Password[%s] MacAddr[%s] PCName[%s] SerialHD[%d] VideoName[%s] VideoGUID[%d] HardwareID[%s] FolderID[%d]"
#define INCORRECT_PASSWORD_LOGACCTEXT					"%s (Incorrect Password) -> Password \"%s\""
#define TRYLOGIN_ACCNOTACTIVATED_LOGACCTEXT				"%s (Account not activated) -> Password \"%s\""
#define TRYLOGIN_ACCBLOCKED_LOGACCTEXT					"%s (Account is banned) -> Password \"%s\""
#define UNBANNED_ACCBLOCKED_LOGACCTEXT					"%s (Account is unbanned) -> Password \"%s\""

#define CHARACTERSELECTSEND_LOGACCTEXT					"%s (Character Select Time)"
#define CHARACTERCREATED_LOGACCTEXT						"%s (New Character Created) -> Name \"%s\""
#define CHARACTEREXP_RECEIVED_LOGCHARTEXT				"(%s) (Received Experience) [%lld] Level[%d] Map[%d] Party[%s]"
#define CHARACTEREXP_RECEIVEDMAX_LOGCHARTEXT			"(%s) (Received Max Experience) [%lld] Level[%d] Map[%d] Party[%s]"
#define CHARACTEREXP_PENALITY_LOGCHARTEXT				"(%s) (Lose Experience in Penality) [%lld] Level[%d] Map[%d] Party[%s]"

// PostBox
#define POSTBOX_GETITEMLOG								"(%s) (Received Item) [%s] [%d]"