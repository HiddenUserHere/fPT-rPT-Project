#pragma once

namespace UglyRefactor
{

class PTModelVersion
{
public:
    enum class EPTModelVersion : int
    {
        PTMODELVERSION_Invalid = 0xFFFF,
        PTMODELVERSION_0062 = 0x0062,
        PTMODELVERSION_0063 = 0x0063,
        PTMODELVERSION_0064 = 0x0064,
        PTMODELVERSION_0065 = 0x0065,
        PTMODELVERSION_0066 = 0x0066,
    };

	EPTModelVersion	eVersion = EPTModelVersion::PTMODELVERSION_0062;

    PTModelVersion( EPTModelVersion e ) { eVersion = e; };
    PTModelVersion( const std::string strVersionHeader ) 
	{
		const UCHAR szModelHeaderLOCAL[] = {0xA5, 0x82, 0xAC, 0x58, 0x15, 0x98, 0x29, 0x8C, 0x85, 0x9F, 0x12, 0xC8, 0x84, 0x1F, 0x74, 0x8C, 0x8F, 0x42, 0x8F, 0xA8, 0x1B, 0x84, 0xAA, 0x00};

		eVersion = EPTModelVersion::PTMODELVERSION_Invalid;

		if ( STRINGCOMPARE( strVersionHeader.c_str(), AY_OBFUSCATE( "SMD Model data Ver 0.62" ) ) )
			eVersion = EPTModelVersion::PTMODELVERSION_0062;
		else if ( memcmp( strVersionHeader.c_str(), (char *)szModelHeaderLOCAL, 24 ) == 0 )
			eVersion = EPTModelVersion::PTMODELVERSION_0063;
		else if ( STRINGCOMPARE( strVersionHeader.c_str(), AY_OBFUSCATE( "SMD Model data Ver 0.64" ) ) )
			eVersion = EPTModelVersion::PTMODELVERSION_0064;
        else if ( STRINGCOMPARE( strVersionHeader.c_str(), AY_OBFUSCATE( "SMD Model data Ver 0.65" ) ) )
            eVersion = EPTModelVersion::PTMODELVERSION_0065;
        else if ( STRINGCOMPARE( strVersionHeader.c_str(), AY_OBFUSCATE( "SMD Model data Ver 0.66" ) ) )
            eVersion = EPTModelVersion::PTMODELVERSION_0066;
	};

	PTModelVersion() {};

	~PTModelVersion() {};

	bool			IsValid() { return (eVersion != EPTModelVersion::PTMODELVERSION_Invalid); }

	bool			IsEncrypted() { return (eVersion == EPTModelVersion::PTMODELVERSION_0064) || (eVersion == EPTModelVersion::PTMODELVERSION_0066); };

	bool			HasInfluences() { return (eVersion == EPTModelVersion::PTMODELVERSION_0065) || (eVersion == EPTModelVersion::PTMODELVERSION_0066); }

	bool			IsFloatValues() { return (eVersion == EPTModelVersion::PTMODELVERSION_0065) || (eVersion == EPTModelVersion::PTMODELVERSION_0066); };

	std::string		GetStringVersion()
	{
        const UCHAR szModelHeaderLOCAL[] = {0xA5, 0x82, 0xAC, 0x58, 0x15, 0x98, 0x29, 0x8C, 0x85, 0x9F, 0x12, 0xC8, 0x84, 0x1F, 0x74, 0x8C, 0x8F, 0x42, 0x8F, 0xA8, 0x1B, 0x84, 0xAA, 0x00};

		if ( IsEncrypted() == false )
		{
			if ( eVersion == EPTModelVersion::PTMODELVERSION_0062 )
				return std::string( AY_OBFUSCATE( "SMD Model data Ver 0.62" ) );
			else if ( eVersion == EPTModelVersion::PTMODELVERSION_0063 )
				return std::string( (char*)szModelHeaderLOCAL );
			else if ( eVersion == EPTModelVersion::PTMODELVERSION_0065 )
				return std::string( AY_OBFUSCATE( "SMD Model data Ver 0.65" ) );

			return std::string( AY_OBFUSCATE( "Invalid" ));
		}
		
		if ( eVersion == EPTModelVersion::PTMODELVERSION_0062 )
			return std::string( AY_OBFUSCATE( "SMD Model data Ver 0.62" ) );
		else if ( eVersion == EPTModelVersion::PTMODELVERSION_0063 )
			return std::string( (char*)szModelHeaderLOCAL );
		else if ( eVersion == EPTModelVersion::PTMODELVERSION_0064 )
			return std::string( AY_OBFUSCATE( "SMD Model data Ver 0.64" ) );
		else if ( eVersion == EPTModelVersion::PTMODELVERSION_0065 )
			return std::string( AY_OBFUSCATE( "SMD Model data Ver 0.65" ) );
		else if ( eVersion == EPTModelVersion::PTMODELVERSION_0066 )
			return std::string( AY_OBFUSCATE( "SMD Model data Ver 0.66" ) );

		return std::string( AY_OBFUSCATE( "Invalid" ) );
	};

};

class PTStageVersion
{
public:
    enum class EPTStageVersion : int
    {
        PTSTAGEVERSION_Invalid = 0xFFFF,
        PTSTAGEVERSION_0072 = 0x0072,
        PTSTAGEVERSION_0073 = 0x0073,
        PTSTAGEVERSION_0074 = 0x0074,
        PTSTAGEVERSION_0075 = 0x0075,
		PTSTAGEVERSION_0076 = 0x0076,
		PTSTAGEVERSION_0077 = 0x0077,
	};

    EPTStageVersion	eVersion = EPTStageVersion::PTSTAGEVERSION_0072;

	PTStageVersion( EPTStageVersion e ) { eVersion = e; };
	PTStageVersion( const std::string strVersionHeader )
    {
        eVersion = EPTStageVersion::PTSTAGEVERSION_Invalid;

        if ( STRINGCOMPARE( strVersionHeader.c_str(), AY_OBFUSCATE( "SMD Stage data Ver 0.72" ) ) )
            eVersion = EPTStageVersion::PTSTAGEVERSION_0072;
        else if ( STRINGCOMPARE( strVersionHeader.c_str(), AY_OBFUSCATE( "SMD Stage data Ver 0.74" ) ) )
            eVersion = EPTStageVersion::PTSTAGEVERSION_0074;
        else if ( STRINGCOMPARE( strVersionHeader.c_str(), AY_OBFUSCATE( "SMD Stage data Ver 0.75" ) ) )
            eVersion = EPTStageVersion::PTSTAGEVERSION_0075;
		else if ( STRINGCOMPARE( strVersionHeader.c_str(), AY_OBFUSCATE( "SMD Stage data Ver 0.76" ) ) )
			eVersion = EPTStageVersion::PTSTAGEVERSION_0076;
		else if ( STRINGCOMPARE( strVersionHeader.c_str(), AY_OBFUSCATE( "SMD Stage data Ver 0.77" ) ) )
			eVersion = EPTStageVersion::PTSTAGEVERSION_0077;
	};

	PTStageVersion() {};

    ~PTStageVersion() {};

    bool			IsValid() { return (eVersion != EPTStageVersion::PTSTAGEVERSION_Invalid); }

    bool			IsEncrypted() { return (eVersion == EPTStageVersion::PTSTAGEVERSION_0074) || (eVersion == EPTStageVersion::PTSTAGEVERSION_0076); };

	bool			HasNormals() { return (eVersion >= EPTStageVersion::PTSTAGEVERSION_0076); }

    std::string		GetStringVersion()
    {
        if ( IsEncrypted() == false )
        {
            if ( eVersion == EPTStageVersion::PTSTAGEVERSION_0072 )
                return std::string( AY_OBFUSCATE( "SMD Stage data Ver 0.72" ) );
			else if ( eVersion == EPTStageVersion::PTSTAGEVERSION_0075 )
				return std::string( AY_OBFUSCATE( "SMD Stage data Ver 0.75" ) );

            return std::string( AY_OBFUSCATE( "Invalid" ) );
        }

        if ( eVersion == EPTStageVersion::PTSTAGEVERSION_0072 )
            return std::string( AY_OBFUSCATE( "SMD Stage data Ver 0.72" ) );
        else if ( eVersion == EPTStageVersion::PTSTAGEVERSION_0074 )
            return std::string( AY_OBFUSCATE( "SMD Stage data Ver 0.74" ) );
        else if ( eVersion == EPTStageVersion::PTSTAGEVERSION_0075 )
            return std::string( AY_OBFUSCATE( "SMD Stage data Ver 0.75" ) );
		else if ( eVersion == EPTStageVersion::PTSTAGEVERSION_0076 )
			return std::string( AY_OBFUSCATE( "SMD Stage data Ver 0.76" ) );
		else if ( eVersion == EPTStageVersion::PTSTAGEVERSION_0077 )
			return std::string( AY_OBFUSCATE( "SMD Stage data Ver 0.76" ) );

        return std::string( AY_OBFUSCATE( "Invalid" ) );
    };

};



bool SearchObjLocalPos( UnitData * pcUnitData, PTMesh * pcMesh, Point3D * psMeshPositionOut );
bool SearchObjLocalPosEx( UnitData * pcUnitData, PTMesh * pcMesh, Point3D * psMeshPositionOut, int iLength );

class AssaSkillTriumphOfValhallaPart : public EffectBaseFollow
{
public:
	void Start( UnitData * pcUnitDataEffect, int iTime );
	void StartHallOfValhalla( UnitData * pcUnitDataEffect, int iTime );
	void MainFunction();

private:
	int			ParticleID;
	int			ParticleIDExt;
	BOOL		MyCharacterFlag;

	PTMesh * pcMesh;
	PTMesh * pcMeshEx;
	BOOL		EndFlag;
};

class AssaSkill3SparkShield : public EffectBaseFollow
{
public:
	void Start( UnitData * pcUnitDataEffect, int iTime );
	void MainFunction();
private:
	int		ParticleID;
	BOOL	MyCharacterFlag;
	BOOL    EndFlag;
	PTMesh * pcMesh;
};

//Divine Shield
class AssaSkill3DivineInhalation : public EffectBaseFollow
{
public:
	void Start( UnitData * pcUnitDataEffect, int iTimeEffect );
	void MainFunction();
private:
	int		ParticleID;
	BOOL	MyCharacterFlag;
	BOOL    EndFlag;
	PTMesh * pcMesh;
};

class AssaSkill3HolyIncantationPartHand : public EffectBaseFollow
{
public:
	void Start( UnitData * pcUnitDataEffect, int iTimeEffect );
	void MainFunction();
private:
	int		ParticleID;
	int		ParticleIDExt;
	BOOL	MyCharacterFlag;
	BOOL    EndFlag;
	PTMesh * pcMesh;
};

class AssaSkillTriumphOfValhallaShot : public EffectBaseFollow
{
public:
	void Start( UnitData * pcUnitDataEffect, UnitData * pcUnitDataTarget, X3D::Vector3 * psVelocity, int iTimeEffect );
	void MainFunction();

private:
	int			ParticleID;
	X3D::Vector3	sVelocity;
	UnitData * pcTarget;
	X3D::Vector3   sPosition;
	int			iDurationParticle;

	PTMesh * pcMesh;
};


void AssaParticle_DivineInhalationStart( UnitData * pcUnitDataEffect, int iTimeEffect );
void SkillSaintGodlyShield( UnitData * pcUnitDataEffect, float fTimeEffect );
BOOL AssaParticleClanMonsterHit1( UnitData * pcUnitDataEffect );

BOOL LoadStageOld( Stage * pcStage, const char * pszFileName );
BOOL SaveStageOld( Stage * pcStage, const char * pszFileName );

BOOL LoadModelOld( EXEModel * pcModel, const std::string strFileName, const std::string strNodeName );
BOOL SaveModelOld( EXEModel * pcModel, const std::string strFileName );

BOOL LoadMeshOld( EXEMesh * pcMesh, HANDLE hFile, PTModelVersion sModelVersion, EXEModel * pcBoneContainer = nullptr, bool bIsBone = false );
BOOL SaveMeshOld( EXEMesh * pcMesh, HANDLE hFile, PTModelVersion sModelVersion );

void EncryptAll();

void ExpansionSkillRender( UnitData * pcUnitData );

void ImportASEData( EXEMesh * pcMesh, const char * pszLineBuffer );
void ImportASEDataStageMesh( Stage * pcStage, const char * pszLineBuffer );
};