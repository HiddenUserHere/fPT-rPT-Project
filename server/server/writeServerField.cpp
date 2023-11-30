#include "stdafx.h"
#include "writeServerField.h"


//----- (writeServerField) -----------------------------------------------------
signed int __cdecl writeServerField()
{
	if ( !GAME_SERVER )
		return 1;
	/*
    //----- Data declarations
    signed int fieldNumberLocation;
    int fieldNumber;
    signed int totalFieldsNumber;
    int *filedSkillsBlock;

 //   STRINGCOPY(fieldField, "Data\\Maps\\");
    memset(firstFieldPointer, 0, 0xEA800);

    fieldNumber = 0;
    fieldNumberLocation = (signed int)fieldNumberZeroLocation;
    do
    {
        *(DWORD *)fieldNumberLocation = fieldNumber;
        fieldNumberLocation += 3752;
        ++fieldNumber;
    }
    while ( fieldNumberLocation < (signed int)lastField );

	int iCountMaps = 0;

	SQLConnection * pcDB = SQLCONNECTION( DATABASEID_GameDB );
				
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT COUNT(*) FROM MapList" ) )
		{
			if ( pcDB->Execute() && pcDB->Fetch() )
				pcDB->GetData( 1, PARAMTYPE_Integer, &iCountMaps, 0 );
		}
		pcDB->Close();
	}

	UINT iLoopOffset = 0;
	totalFieldsNumber = 0;
	
	if ( pcDB->Open() )
	{
		if ( pcDB->Prepare( "SELECT * FROM MapList ORDER BY ID ASC" ) )
		{
			if ( pcDB->Execute() && pcDB->Fetch() )
			{
				for ( int i = 0; i < iCountMaps; i++ )
				{
					char szShortName[256] = { 0 };
					int iLevelMap = 0;
					char szFilePath[256] = { 0 };
					pcDB->GetData( 3, PARAMTYPE_String, szShortName, 256 );
					pcDB->GetData( 5, PARAMTYPE_Integer, &iLevelMap, 0 );
					pcDB->GetData( 7, PARAMTYPE_String, szFilePath, 256 );

					field( (char *)(firstField + iLoopOffset), (char*)szFilePath, (char*)szShortName );
					if ( i == MAPID_NaviskoTown )
					{
						environment( (char *)(firstField + iLoopOffset), fldT_desert, skyT_desert_day_8, skyT_desert_evening_9, skyT_desert_night_10, bgmT_town_tempskron_stronghold, 1, 1, 1 );
						fieldCmd( (char *)(firstField + iLoopOffset), 22214, -1182 );
						respawn( (char *)(firstField + iLoopOffset), 22214, -1182 );
					}
					if ( i == MAPID_BlessCastle )
						environment( (char *)(firstField + iLoopOffset), fldT_castle, skyT_wilderness_day_1, skyT_wilderness_evening_2, skyT_wilderness_night_3, bgmT_stage_play3, 0, 1, 3 );
					*(UINT*)(0x08B80008 + (i * 4)) = iLevelMap;
					iLoopOffset += fieldOffset;
					totalFieldsNumber++;

					if ( !pcDB->Fetch() )
						break;
				}
			}
		}
		pcDB->Close();
	}

//----- (fields end) -----------------------------------------------------------

    if ( totalFieldsNumber > 0 )
    {
        filedSkillsBlock = (int *)filedSkillsBlockLocation;
        do
        {
            if ( *filedSkillsBlock == 3 )
                *filedSkillsBlock = 2;
            filedSkillsBlock += 938;
            --totalFieldsNumber;
        }
        while ( totalFieldsNumber );
    }
	*/
    return 1;
}

std::string SplitFilenameSTAGE( const std::string str )
{
	int found = str.find_last_of( "/\\" );
	return str.substr( 0, found )+ "\\";
}


//----- (field) ----------------------------------------------------------------
signed int field(char *thisField, LPCSTR filePath, char *shortName)
{
    int v0;
    signed int result;

    v0 = (int)thisField;
	STRINGCOPYLEN( ( LPSTR )( thisField + 4 ), 64, filePath );
    if ( shortName )
    {
		STRINGFORMATLEN( ( LPSTR )( v0 + 68 ), 64, "%s%s.tga", fieldMap, shortName );
		STRINGFORMATLEN( ( LPSTR )( v0 + 132 ), 64, "%s%st.tga", fieldTitle, shortName );
        result = 1;
    }
    else
    {
        *(BYTE *)(v0 + 68) = 0;
        *(BYTE *)(v0 + 132) = 0;
        result = 1;
    }
    return result;
}

//----- (animation) ------------------------------------------------------------
int animation(char *thisField, char *filePath, int bones)
{
    signed int v0;
    int result;

    v0 = *(DWORD *)(thisField + 3664);
    if ( v0 < 50 )
    {
        *(DWORD *)(thisField + 4 * v0 + 3264) = (int)filePath;
        *(DWORD *)(thisField + 4 * *(DWORD *)(thisField + 3664) + 3464) = bones;
        result = *(DWORD *)(thisField + 3664) + 1;
        *(DWORD *)(thisField + 3664) = result;
    }
    else
    {
        result = 0;
    }
    return result;
}

//----- (fieldCmd) -------------------------------------------------------------
signed int fieldCmd(char *thisField, int X, int Z)
{
    *(DWORD *)(thisField + 3668) = X;
    *(DWORD *)(thisField + 3672) = Z;
    return 1;
}

//----- (respawn) --------------------------------------------------------------
signed int respawn(char *thisField, int X, int Z)
{
    signed int v0;
    signed int result;

    v0 = *(DWORD *)(thisField + 3748);
    if ( v0 < 8 )
    {
        *(DWORD *)(thisField + 8 * v0 + 3684) = X;
        *(DWORD *)(thisField + 8 * ++*(DWORD *)(thisField + 3748 ) + 3680) = Z;
        result = 1;
    }
    else
    {
        result = 0;
    }
    return result;
}

//----- (teleport) -------------------------------------------------------------
int teleport(char *thisField, int X, int Z, int Y, int length, int width, int teleportLVL, int teleportType)
{
    signed int v0;
    int result;

    v0 = *(DWORD *)(thisField + 352);
    if ( v0 < 8 )
    {
        *(DWORD *)(thisField + 356) = v0;
        *(DWORD *)(160 * v0 + thisField + 360) = X << 8;
        *(DWORD *)(160 * *(DWORD *)(thisField + 352) + thisField + 368) = Y << 8;
        *(DWORD *)(160 * *(DWORD *)(thisField + 352) + thisField + 364) = Z << 8;
        *(DWORD *)(160 * *(DWORD *)(thisField + 352) + thisField + 372) = width << 8;
        *(DWORD *)(160 * *(DWORD *)(thisField + 352) + thisField + 376) = length << 8;
        *(DWORD *)(160 * *(DWORD *)(thisField + 352) + thisField + 508) = 0;
        *(DWORD *)(160 * *(DWORD *)(thisField + 352) + thisField + 512) = teleportLVL;
        *(DWORD *)(160 * *(DWORD *)(thisField + 352) + thisField + 516) = teleportType;
        result = *(DWORD *)(thisField + 352) + 1;
        *(DWORD *)(thisField + 352) = result;
    }
    else
    {
        result = 0;
    }
    return result;
}

//----- (teleportTo) -----------------------------------------------------------
int teleportTo(char *thisField, char *thatField, int X, int Z, int Y)
{
    signed int v0;
    int v1;
    int result;
    int v2;

    v1 = (int)(224 * *(DWORD *)(thisField + 356) + thisField + 360);
    v0 = *(DWORD *)(160 * *(DWORD *)(thisField + 356) + thisField + 508);
    if ( v0 < 8 )
    {
        *(DWORD *)(16 * (v0 + 2) + v1) = (int)thatField;
        v2 = v1 + 16 * v0;
        *(DWORD *)(v2 + 20) = X << 8;
        *(DWORD *)(v2 + 24) = Z << 8;
        *(DWORD *)(v2 + 28) = Y << 8;
        ++*(DWORD *)(v1 + 148);
        if ( thatField == thisField )
        {
            *(DWORD *)(thisField + 1640) = X << 8;
            *(DWORD *)(thisField + 1644) = Y << 8;
            *(DWORD *)(thisField + 1648) = Z << 8;
        }
        result = *(DWORD *)(v1 + 148);
    }
    else
    {
        result = 0;
    }
    return result;
}

//----- (fieldLoad) ------------------------------------------------------------
signed int fieldLoad(char *thisField, char *thatField, int X, int Z, int Y)
{
    int v0;
    signed int v1;
    signed int result;

    v0 = (int)thisField;
    v1 = *(DWORD *)(thisField + 220);
    if ( v1 < 8 )
    {
        *(DWORD *)(16 * (v1 + 14) + v0) = X;
        *(DWORD *)(16 * *(DWORD *)(v0 + 220) + v0 + 232) = Y;
        *(DWORD *)(16 * *(DWORD *)(v0 + 220) + v0 + 228) = Z;
        *(DWORD *)(16 * ++*(DWORD *)(v0 + 220) + v0 + 220) = (int)thatField;
        subFieldLoad(thatField, v0, X, Z, Y);
        result = 1;
    }
    else
    {
        result = 0;
    }
    return result;
}

//----- (subFieldLoad) ---------------------------------------------------------
signed int subFieldLoad(char *thisField, int field, int X, int Z, int Y)
{
    signed int v0;
    signed int result;

    v0 = *(DWORD *)(thisField + 220);
    if ( v0 < 8 )
    {
        *(DWORD *)(16 * (v0 + 14) + thisField) = X;
        *(DWORD *)(16 * *(DWORD *)(thisField + 220) + thisField + 232) = Y;
        *(DWORD *)(16 * *(DWORD *)(thisField + 220) + thisField + 228) = Z;
        *(DWORD *)(16 * ++*(DWORD *)(thisField + 220) + thisField + 220) = field;
        result = 1;
    }
    else
    {
        result = 0;
    }
    return result;
}

//----- (sound) ----------------------------------------------------------------
int sound(char *thisField, int X, int Y, int Z, int range, int soundType)
{
    signed int v0;
    int result;

    v0 = *(DWORD *)(thisField + 4724);
    if ( v0 < 80 )
    {
        *(DWORD *)(thisField + 20 * v0 + 1652) = X << 8;
        *(DWORD *)(thisField + 20 * *(DWORD *)(thisField + 3252) + 1656) = Y << 8;
        *(DWORD *)(thisField + 20 * *(DWORD *)(thisField + 3252) + 1660) = Z << 8;
        *(DWORD *)(thisField + 20 * *(DWORD *)(thisField + 3252) + 1664) = range;
        *(DWORD *)(thisField + 20 * (*(DWORD *)(thisField + 3252) + 83) + 8 ) = soundType;
        result = *(DWORD *)(thisField + 3252) + 1;
        *(DWORD *)(thisField + 3252) = result;
    }
    else
    {
        result = 0;
    }
    return result;
}

//----- (environment) ----------------------------------------------------------
signed int environment(char *thisField, int fieldType, int daySky, int eveningSky, int nightSky, int bgmType, int mobSpawnTime, int fieldLVL, int skillsBlock)
{
    *(DWORD *)(thisField + 196)  = fieldType;
    *(DWORD *)(thisField + 200)  = daySky;
    *(DWORD *)(thisField + 204)  = eveningSky;
    *(DWORD *)(thisField + 208)  = nightSky;
    *(DWORD *)(thisField + 212)  = bgmType;
    *(DWORD *)(thisField + 216)  = mobSpawnTime;
    *(DWORD *)(thisField + 3256) = fieldLVL;
    *(DWORD *)(thisField + 3680) = skillsBlock;
    return 1;
}

//----- (miniGame) -------------------------------------------------------------
int miniGame(char *thisField, int a1, int screenUpDown, int zoomInOut, int moveStart, int moveEnd)
{
    *(DWORD *)(thisField + 3732) = a1; /* Unknown */
    *(DWORD *)(thisField + 3736) = screenUpDown;
    *(DWORD *)(thisField + 3740) = zoomInOut;
    *(DWORD *)(thisField + 3744) = moveStart;
    *(DWORD *)(thisField + 3748) = moveEnd;
    return 1;
}