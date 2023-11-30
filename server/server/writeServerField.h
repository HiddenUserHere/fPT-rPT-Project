//----- Write Field by Vormav is licensed under a Creative Commons Attribution 3.0 Unported License.

//----- You are free:

//----- to Share — to copy, distribute and transmit the work
//----- to Remix — to adapt the work
//----- to make commercial use of the work


#ifndef __WRITESERVERFIELD_H__
#define __WRITESERVERFIELD_H__


#include <windows.h>

/*  To use this exported function of dll, include this header
 *  in your project.
 */


//----- Server offsets
int filedSkillsBlockLocationOffset  =  776;
int fieldNumberZeroLocationOffset   = 3676;
int fieldOffset                     = 3752;

//----- Those addresses might be different on some Priston Tale servers
LPSTR fieldField                    =  (LPSTR)0x0075AF78;
LPSTR fieldMap                      =  (LPSTR)0x0075AFF8;
LPSTR fieldTitle                    =  (LPSTR)0x0075AFB8;

void *firstFieldPointer             = (void *)0x0075B038;
PBYTE firstField                    =  (PBYTE)0x0075B038;
PBYTE lastField                     =  (PBYTE)0x00846694;




#endif // __WRITESERVERFIELD_H__

#ifndef __WRITEFIELD_H__
#define __WRITEFIELD_H__


//----- Variables definitions
#define filedSkillsBlockLocation     firstField	                     + filedSkillsBlockLocationOffset
#define fieldNumberZeroLocation      firstField	                     + fieldNumberZeroLocationOffset

//----- Fields definitions
#define fore_3                       firstField
#define fore_2                       fore_3                          + fieldOffset
#define fore_1	                     fore_2	                         + fieldOffset
#define village_2                    fore_1                          + fieldOffset
#define ruin_4                       village_2                       + fieldOffset
#define ruin_3                       ruin_4                          + fieldOffset
#define ruin_2                       ruin_3                          + fieldOffset
#define ruin_1                       ruin_2                          + fieldOffset
#define de_1                         ruin_1                          + fieldOffset
#define village_1                    de_1                            + fieldOffset
#define de_2                         village_1                       + fieldOffset
#define de_3                         de_2                            + fieldOffset
#define de_4                         de_3                            + fieldOffset
#define dun_1                        de_4                            + fieldOffset
#define dun_2                        dun_1                           + fieldOffset
#define dun_3                        dun_2                           + fieldOffset
#define office                       dun_3                           + fieldOffset
#define forever_fall_04              office                          + fieldOffset
#define forever_fall_03              forever_fall_04                 + fieldOffset
#define forever_fall_02              forever_fall_03                 + fieldOffset
#define forever_fall_01              forever_fall_02                 + fieldOffset
#define pilai                        forever_fall_01                 + fieldOffset
#define dun_4                        pilai                           + fieldOffset
#define dun_5                        dun_4                           + fieldOffset
#define tcave                        dun_5                           + fieldOffset
#define mcave                        tcave                           + fieldOffset
#define dcave                        mcave                           + fieldOffset
#define iron_1                       dcave                           + fieldOffset
#define iron_2                       iron_1                          + fieldOffset
#define ice_ura                      iron_2                          + fieldOffset
#define sod_1                        ice_ura                         + fieldOffset
#define ice1                         sod_1                           + fieldOffset
#define quest_IV                     ice1                            + fieldOffset
#define castle                       quest_IV                        + fieldOffset
#define greedy                       castle                          + fieldOffset
#define ice2                         greedy                          + fieldOffset
#define boss                         ice2                            + fieldOffset
#define lost                         boss                            + fieldOffset
#define losttemple                   lost                            + fieldOffset
#define fall_game                    losttemple                      + fieldOffset
#define dun_7                        fall_game                       + fieldOffset
#define dun_8                        dun_7                           + fieldOffset
#define dun_6                        dun_8                           + fieldOffset
#define dun_9                        dun_6                           + fieldOffset
#define mine_1                       dun_9                           + fieldOffset
#define town1                        mine_1                          + fieldOffset
#define fo1		                     town1                           + fieldOffset
#define fo2		                     fo1	                         + fieldOffset
#define fo3		                     fo2	                         + fieldOffset
//----- New field name               last filed name                 + offset from last field
//----- #define new_field            old_dun_3                       + fieldOffset


//----- Function prototypes
signed int field( char *thisField, LPCSTR filePath, char *shortName );
int animation( char *thisField, char *filePath, int bones );
signed int fieldCmd( char *thisField, int X, int Z );
signed int respawn( char *thisField, int X, int Z );
int teleport( char *thisField, int X, int Z, int Y, int length, int width, int teleportLVL, int teleportType );
int teleportTo( char *thisField, char *thatField, int X, int Z, int Y );
signed int fieldLoad( char *thisField, char *thatField, int X, int Z, int Y );
signed int subFieldLoad( char *thisField, int thatField, int X, int Z, int Y );
int sound( char *thisField, int X, int Y, int Z, int range, int soundType );
signed int environment( char *thisField, int fieldType, int daySky, int eveningSky, int nightSky, int bgmType, int mobSpawnTime, int fieldLVL, int skillsBlock );
int miniGame( char *thisField, int a1, int screenUpDown, int zoomInOut, int moveStart, int moveEnd );


//----- Fields types
enum fieldType
{
	fldT_town = 0x100, // can't use weapons or skills, you can setup shop here
	fldT_forest = 0x200,
	fldT_desert = 0x300,
	fldT_ruins = 0x400,
	fldT_dungeon = 0x500, // overwrites sky to black
	fldT_iron = 0x600,
	fldT_unused_1 = 0x700, // unused in kPT version 1.98.9
	fldT_office = 0x800, // place for banned players
	fldT_ice = 0x900,
	fldT_castle = 0xA00, // event "Bless Castle"
	fldT_mini_game = 0xB00  // 2D walking style, ARROWS = movements, CTRL = attack
};

//----- Sky types
//----- Defined in Effect\\Sky\\test16.txt and Effect\\Sky\\test32.txt
//----- English translation provided by bobsobol
enum skyType
{
	skyT_rainy_sky_7 = 0, // Rainy sky
	skyT_forest_night_6 = 1, // Forest at night
	skyT_forest_day_4 = 2, // Forest daytime
	skyT_forest_evening_5 = 3, // Forest in the evening
	skyT_desert_day_8 = 4, // Desert daytime
	//skyT_desert_evening_9       =  5, // Desert in the evening // same as 32
	skyT_desert_night_10 = 6, // Desert at night
	skyT_wilderness_day_0 = 7, // Wilderness daytime
	skyT_wilderness_day_1 = 8, // Wilderness daytime // have same settings as 7
	skyT_wilderness_night_3 = 9, // Wilderness at night
	skyT_unused_sky_1 = 10, // Unused in kPT version 1.98.9
	skyT_wilderness_evening_2 = 11, // Wilderness in the evening
	skyT_unused_sky_2 = 12, // Unused in kPT version 1.98.9
	skyT_unused_sky_3 = 13, // Unused in kPT version 1.98.9
	skyT_unused_sky_4 = 14, // Unused in kPT version 1.98.9
	skyT_unused_sky_5 = 15, // Unused in kPT version 1.98.9
	skyT_unused_sky_6 = 16, // Unused in kPT version 1.98.9
	skyT_in_forest_night_13 = 17, // In the forest at night
	skyT_in_forest_day_11 = 18, // In the forest
	skyT_in_forest_evening_12 = 19, // In the forest in the evening
	skyT_iron_night_16 = 20, // Iron at night
	skyT_iron_night_17 = 21, // Iron at night // broken in kPT version 1.98.9, taking settings from 20
	skyT_iron_day_14 = 22, // Iron daytime
	skyT_iron_evening_15 = 23, // Iron in the evening
	skyT_sod_sky_day_18 = 24, // Sword sky daytime // SoD 2 round 7-8 sky
	skyT_sod_moonlight_night_19 = 25, // Sword moonlight night // SoD 2 round 4-6 Lunar stage sky
	skyT_unused_sky_7 = 26, // Unused in kPT version 1.98.9
	skyT_unused_sky_8 = 27, // Unused in kPT version 1.98.9
	skyT_unused_sky_9 = 28, // Unused in kPT version 1.98.9
	skyT_unused_sky_10 = 29, // Unused in kPT version 1.98.9
	skyT_unused_sky_11 = 30, // Unused in kPT version 1.98.9
	skyT_unused_sky_12 = 31, // Unused in kPT version 1.98.9
	skyT_desert_evening_9 = 32, // Desert in the evening // SoD 2 round 4-6 Solar stage sky
	skyT_sod_black_20 = 33, // Sword black // SoD 2 round 1-3 sky
	skyT_iron_boss_sky_21 = 34, // Iron Boss sky
	skyT_ice_day_23 = 35, // Ice Daytime
	skyT_ice_evening_24 = 36, // Ice in the evening
	skyT_ice_night_25 = 37, // Ice at night
	skyT_south_island_day_26 = 38, // South Island daytime
	skyT_south_island_evening_27 = 39, // South Island in the evening
	skyT_south_island_night_28 = 40, // South Island at night
	skyT_lost_day_29 = 41, // Lost daytime
	skyT_lost_evening_30 = 42, // Lost in the evening
	skyT_lost_night_31 = 43, // Lost in the evening
	skyT_lost_temple_day_32 = 44, // Lost temple
	skyT_lost_temple_evening_33 = 45, // Lost temple
	skyT_lost_temple_night_34 = 46  // Lost temple
};

//----- Background music types
//----- Located in wav\\BGM
enum bgmType
{
	bgmT_PLAY_WHEN_TRIGGERED = -1, // Playing music when SoD 2 starts or Babel spawns
	//bgmT_field_desert_pilgrim     =  0, // Field - Desert - Pilgrim.bgm // same as 0
	bgmT_town_tempskron_stronghold = 1, // Town - Tempskron_Stronghold.bgm
	bgmT_town_1_tempskron_ricarten = 2, // Town 1 - Tempskron_Ricarten - When wind comes-o.bgm
	bgmT_wind_loop = 3, // wind loop.bgm
	bgmT_forest_darkwood = 4, // Field - Forest - DarkWood.bgm
	bgmT_dungeon_gloomy_heart = 5, // Dungeon - Gloomy Heart.bgm
	bgmT_town_2_morion_philliy = 6, // Town 2 - Morion_Philliy - Voyage above the Clouds.bgm
	bgmT_stage_play1 = 7, // Stage_Play1.bgm
	//bgmT_stage_play1              =  8, // Stage_Play1.bgm // same as 7
	bgmT_stage_play3 = 9, // Stage_Play3.bgm
	bgmT_field_desert_pilgrim = 10, // Field - Desert - Pilgrim.bgm
	bgmT_ice_1 = 11, // Ice 1.bgm
	bgmT_unused_bgm_1 = 12  // Unused in kPT version 1.98.9
};

//----- Sounds types
//----- Located in wav\\Ambient
enum soundType
{
	sndT_stone_mill = 0, // stone-mill.wav
	sndT_dungeon_pumping_b = 1, // dungeon_pumping_b.wav
	sndT_dungeon_pumping_l = 2, // dungeon_pumping_l.wav
	sndT_dungeon_spin_b = 3, // dungeon_spin_b.wav
	sndT_lake_1 = 4, // lake 1.wav
	sndT_mystic_1 = 5, // mystic 1.wav
	sndT_mystic_2 = 6, // mystic 2.wav
	sndT_ship_swaing_l = 7, // ship_swaing_l.wav
	sndT_stream = 8, // stream.wav
	sndT_town_bell_1 = 9, // town_bell 1.wav
	sndT_town_bell_2 = 10, // town_bell 2.wav
	sndT_town_spin_b = 11, // town_spin_b.wav
	sndT_town_spin_l = 12, // town_spin_l.wav
	sndT_waterfall = 13, // waterfall.wav
	sndT_stone_mill2 = 14, // stone-mill2.wav
	sndT_watermill_1 = 15, // watermill 1.wav
	sndT_windmill_1 = 16, // windmill 1.wav
	sndT_windmill_2 = 17, // windmill 2.wav
	sndT_windmill_3 = 18, // windmill 3.wav
	sndT_windmill_4 = 19, // windmill 4.wav
	sndT_owl_1 = 20, // owl 1.wav
	sndT_dungeon_big_bolt_1 = 21, // dungeon_big-bolt 1.wav
	sndT_dungeon_stonepole = 22, // dungeon_stonepole.wav
	sndT_zombiemill_1 = 23, // zombiemill 1.wav
	sndT_dungeon_weight = 24, // dungeon_weight.wav
	sndT_dungeon_device_side = 25, // dungeon_device_side.wav
	sndT_dungeon_device_axe = 26, // dungeon_device_axe.wav
	sndT_dungeon_cage_1 = 27, // dungeon_cage 1.wav
	sndT_unused_sound_1 = 28, // Unused in kPT version 1.98.9
	sndT_unused_sound_2 = 29  // Unused in kPT version 1.98.9
};



#endif // __WRITEFIELD_H__

