#pragma once
class CCharacterDataHandler
{
public:
	CCharacterDataHandler();
	virtual ~CCharacterDataHandler();

	BOOL						LoadCharacterInventory( User * pcUser, PacketCharacterRecordData & sPacket );

	BOOL						LoadCharacterDataFile( std::string strFile, PacketCharacterRecordData & sPacket );

	void						HandlePacket( User * pcUser, PacketSelectCharacter * psPacket );

	void						ProcessPacket( User * pcUser, PacketCharacterRecordData * psPacket );

	static BOOL					SetLoginKey( CharacterData * psCharacterData );

	DWORD						GetLoginKey( CharacterData * psCharacterData, DWORD dwHashName );

	static BOOL					ValidateStatusPoints( User * pcUser, CharacterData & sCharacterData, UINT iQuestFlag );

private:
	static int					GetSkillPointQuest( int iLevel, UINT iQuestFlag );
	static int					GetStatusPointQuest( User * pcUser, int iLevel, UINT iQuestFlag );
};

