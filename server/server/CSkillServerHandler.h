#pragma once
class CSkillServerHandler
{
public:
    CSkillServerHandler();
    virtual ~CSkillServerHandler();

    static void             Init();
    static void             Load();

    static void             SendSkillTable( User * pcUser );

private:
    static void             LoadFighter();
    static void             LoadMechanician();
    static void             LoadArcher();
    static void             LoadPikeman();
    static void             LoadAtalanta();
    static void             LoadKnight();
    static void             LoadMagician();
    static void             LoadPriestess();
    static void             LoadAssassin();
    static void             LoadShaman();

    static std::vector<PacketSkillTable> vSkillTablePackets;

};

