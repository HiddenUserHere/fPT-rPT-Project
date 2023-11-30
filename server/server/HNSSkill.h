#pragma once
#ifndef __SKILLS_PT
#define __SKILLS_PT

#include "stdafx.h"
#include "SQLSkill.h"
#include "packetserver.h"
#include "Fighter.h"
#include "Archer.h"
#include "Priestess.h"
#include "Mechanician.h"
#include "Magician.h"
#include "Pikeman.h"
#include "Knight.h"
#include "Atalanta.h"
#include "Assassin.h"
#include "Shaman.h"



extern CFighter									* pcFighter;
extern CMechanician								* pcMechanician;
extern CArcher									* pcArcher;
extern CPikeman									* pcPikeman;
extern CAtalanta								* pcAtalanta;
extern CKnight									* pcKnight;
extern CMagician								* pcMagician;
extern CPriestess								* pcPriestess;
extern CAssassin								* pcAssassin;
extern CShaman									* pcShaman;

#define FIGHTERSKILL							pcFighter
#define MECHANICIANSKILL						pcMechanician
#define ARCHERSKILL								pcArcher
#define PIKEMANSKILL							pcPikeman
#define ATALANTASKILL							pcAtalanta
#define KNIGHTSKILL								pcKnight
#define MAGICIANSKILL							pcMagician
#define PRIESTESSSKILL							pcPriestess
#define ASSASSINSKILL							pcAssassin
#define SHAMANSKILL								pcShaman


enum ForcesOrb
{
	Lucidy				= 0x03060100,
	Sereneo				= 0x03060200,
	Fadeo				= 0x03060300,
	Sparky				= 0x03060400,
	Raident				= 0x03060500,
	Transparo			= 0x03060600,
	Murky				= 0x03060700,
	Devine				= 0x03060800,
	Celesto				= 0x03060900,
	Mirage				= 0x03060A00,
	Inferna				= 0x03060B00,
	Enigma				= 0x03060C00,
	Bellum				= 0x03060D00,
	Oredo				= 0x03060E00,

};



#endif