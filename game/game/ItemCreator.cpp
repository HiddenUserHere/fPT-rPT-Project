#include "stdafx.h"
#include "ItemCreator.h"

typedef				void( __cdecl *t_ChangeItemData ) ( DWORD item );
t_ChangeItemData	ChangeItData = ( t_ChangeItemData )0x00439C70;

typedef				void( __cdecl *t_CompressItemData ) ( DWORD item, DWORD pItem, DWORD sz );
t_CompressItemData	CompressItData = ( t_CompressItemData )0x00445480;

DWORD				pTableNewItem = 0;
DWORD				pTableNewItemSoma = 0x10;

void				*allocIT;
void				*allocITList;
extern	ItemData		*pItemTable;

void ItemCreator::refMem( DWORD addr, DWORD value )
{
	DWORD dwOld;
	VirtualProtect( ( void * )addr, 4, PAGE_EXECUTE_READWRITE, &dwOld );
	*( DWORD* )addr = value;
}

void ItemCreator::createItem( int idIt, const char *nomeIt, const char *codIt, const char *codIt2, int LarIt, int altIt, const char *dropIt, int mIt, 
							  int cityIt, int somIt, int rangeIt )
{
	*( DWORD* )( ( DWORD )allocIT + ItemV ) = idIt;											// ID
	MoveMemory( ( void * )( ( DWORD )allocIT + ItemV + 4 ), nomeIt, lstrlenA( nomeIt ) );		// Name
	MoveMemory( ( void * )( ( DWORD )allocIT + ItemV + 0x24 ), codIt, lstrlenA( codIt ) );	// sItemID 1
	MoveMemory( ( void * )( ( DWORD )allocIT + ItemV + 0x80 ), codIt2, lstrlenA( codIt2 ) );	// sItemID 2
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x34 ) = LarIt;									// iWidth
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x38 ) = altIt;									// iHeight
	MoveMemory( ( void * )( ( DWORD )allocIT + ItemV + 0x3C ), dropIt, lstrlenA( dropIt ) );	// Folder
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x7C ) = mIt;									// Hand
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0xC0 ) = cityIt;								// Weapon City
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0xC4 ) = somIt;									// iSound
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0xC8 ) = rangeIt;								// Range

	ItemV += 0x314;
}

void ItemCreator::refIT( )
{
	refMem( 0x004583AF, ( DWORD )allocIT + 1 );
	refMem( 0x0041E667, ( DWORD )allocIT );
	refMem( 0x00433DAF, ( DWORD )allocIT );
	refMem( 0x00434CD8, ( DWORD )allocIT );
	refMem( 0x00434D14, ( DWORD )allocIT );
	refMem( 0x0043D2C6, ( DWORD )allocIT );
	refMem( 0x00441EBB, ( DWORD )allocIT );
	refMem( 0x00441EF7, ( DWORD )allocIT );
	refMem( 0x00449860, ( DWORD )allocIT );
	refMem( 0x0044BA10, ( DWORD )allocIT );
	refMem( 0x0044BCFE, ( DWORD )allocIT );
	refMem( 0x0044BE7D, ( DWORD )allocIT );
	refMem( 0x0044BFBF, ( DWORD )allocIT );
	refMem( 0x0044C065, ( DWORD )allocIT );
	refMem( 0x0044C105, ( DWORD )allocIT );
	refMem( 0x00458162, ( DWORD )allocIT );
	refMem( 0x004583F2, ( DWORD )allocIT );
	refMem( 0x0046AE29, ( DWORD )allocIT );
	refMem( 0x004ABA2F, ( DWORD )allocIT );
	refMem( 0x004ABAA5, ( DWORD )allocIT );
	refMem( 0x004B16DB, ( DWORD )allocIT );
	refMem( 0x004B1767, ( DWORD )allocIT );
	refMem( 0x004C4573, ( DWORD )allocIT );
	refMem( 0x004C4595, ( DWORD )allocIT );
	refMem( 0x004C4BC4, ( DWORD )allocIT );
	refMem( 0x004C4BE6, ( DWORD )allocIT );
	refMem( 0x004D4E4C, ( DWORD )allocIT );
	refMem( 0x004D4E6F, ( DWORD )allocIT );
	refMem( 0x004E4FA3, ( DWORD )allocIT );
	refMem( 0x004E4FF6, ( DWORD )allocIT );
	refMem( 0x004E5046, ( DWORD )allocIT );
	refMem( 0x004E50C3, ( DWORD )allocIT );
	refMem( 0x004E524C, ( DWORD )allocIT );
	refMem( 0x004E52BA, ( DWORD )allocIT );
	refMem( 0x00505C74, ( DWORD )allocIT );
	refMem( 0x00505D3F, ( DWORD )allocIT );
	refMem( 0x00505DA4, ( DWORD )allocIT );
	refMem( 0x00505DD1, ( DWORD )allocIT );
	refMem( 0x00509088, ( DWORD )allocIT );
	refMem( 0x005090EF, ( DWORD )allocIT );
	refMem( 0x00509150, ( DWORD )allocIT );
	refMem( 0x005091A8, ( DWORD )allocIT );
	refMem( 0x0050C540, ( DWORD )allocIT );
	refMem( 0x0050CA60, ( DWORD )allocIT );
	refMem( 0x0051F727, ( DWORD )allocIT );
	refMem( 0x0051F74F, ( DWORD )allocIT );
	refMem( 0x0051F9A2, ( DWORD )allocIT );
	refMem( 0x0051F9C4, ( DWORD )allocIT );
	refMem( 0x0054421D, ( DWORD )allocIT );
	refMem( 0x005DA023, ( DWORD )allocIT );
	refMem( 0x005DAA23, ( DWORD )allocIT );
	refMem( 0x005F74B3, ( DWORD )allocIT );
	refMem( 0x0062269A, ( DWORD )allocIT );
	refMem( 0x007ACE22, ( DWORD )allocIT );
	refMem( 0x00449825, ( DWORD )allocIT + 0x24 );
	refMem( 0x0044B9E4, ( DWORD )allocIT + 0x24 );
	refMem( 0x0044BCCB, ( DWORD )allocIT + 0x24 );
	refMem( 0x0044BE48, ( DWORD )allocIT + 0x24 );
	refMem( 0x0044BF8A, ( DWORD )allocIT + 0x24 );
	refMem( 0x0044C032, ( DWORD )allocIT + 0x24 );
	refMem( 0x0044C0D2, ( DWORD )allocIT + 0x24 );
	refMem( 0x0044EA0F, ( DWORD )allocIT + 0x24 );
	refMem( 0x0046ADF6, ( DWORD )allocIT + 0x24 );
	refMem( 0x004ABA4E, ( DWORD )allocIT + 0x24 );
	refMem( 0x004ABACB, ( DWORD )allocIT + 0x24 );
	refMem( 0x004B1721, ( DWORD )allocIT + 0x24 );
	refMem( 0x004C42A1, ( DWORD )allocIT + 0x24 );
	refMem( 0x004C437D, ( DWORD )allocIT + 0x24 );
	refMem( 0x004C45B9, ( DWORD )allocIT + 0x24 );
	refMem( 0x004C4642, ( DWORD )allocIT + 0x24 );
	refMem( 0x004C4C0D, ( DWORD )allocIT + 0x24 );
	refMem( 0x004E4FC5, ( DWORD )allocIT + 0x24 );
	refMem( 0x004E501F, ( DWORD )allocIT + 0x24 );
	refMem( 0x004E5092, ( DWORD )allocIT + 0x24 );
	refMem( 0x004E5273, ( DWORD )allocIT + 0x24 );
	refMem( 0x004E532E, ( DWORD )allocIT + 0x24 );
	refMem( 0x004E8688, ( DWORD )allocIT + 0x24 );
	refMem( 0x00505D7B, ( DWORD )allocIT + 0x24 );
	refMem( 0x00508F38, ( DWORD )allocIT + 0x24 );
	refMem( 0x00508FF0, ( DWORD )allocIT + 0x24 );
	refMem( 0x0050C562, ( DWORD )allocIT + 0x24 );
	refMem( 0x0050C5F0, ( DWORD )allocIT + 0x24 );
	refMem( 0x0050CA8E, ( DWORD )allocIT + 0x24 );
	refMem( 0x0050CB4D, ( DWORD )allocIT + 0x24 );
	refMem( 0x005DA9EE, ( DWORD )allocIT + 0x24 );
	refMem( 0x005F7481, ( DWORD )allocIT + 0x24 );
	refMem( 0x004AD71D, ( DWORD )allocIT + 0x34 );
	refMem( 0x004B1755, ( DWORD )allocIT + 0x34 );
	refMem( 0x004C43B1, ( DWORD )allocIT + 0x34 );
	refMem( 0x004C4676, ( DWORD )allocIT + 0x34 );
	refMem( 0x004C46B4, ( DWORD )allocIT + 0x34 );
	refMem( 0x004E50D2, ( DWORD )allocIT + 0x34 );
	refMem( 0x004E535C, ( DWORD )allocIT + 0x34 );
	refMem( 0x004E86B0, ( DWORD )allocIT + 0x34 );
	refMem( 0x0050CA82, ( DWORD )allocIT + 0x34 );
	refMem( 0x0050CB1F, ( DWORD )allocIT + 0x34 );
	refMem( 0x0050CB7B, ( DWORD )allocIT + 0x34 );
	refMem( 0x004B175E, ( DWORD )allocIT + 0x38 );
	refMem( 0x004C43B7, ( DWORD )allocIT + 0x38 );
	refMem( 0x004C467C, ( DWORD )allocIT + 0x38 );
	refMem( 0x004C46BA, ( DWORD )allocIT + 0x38 );
	refMem( 0x004E50B8, ( DWORD )allocIT + 0x38 );
	refMem( 0x004E5369, ( DWORD )allocIT + 0x38 );
	refMem( 0x004E86B6, ( DWORD )allocIT + 0x38 );
	refMem( 0x0050CA88, ( DWORD )allocIT + 0x38 );
	refMem( 0x0050CB13, ( DWORD )allocIT + 0x38 );
	refMem( 0x0050CB81, ( DWORD )allocIT + 0x38 );
	refMem( 0x004ABA55, ( DWORD )allocIT + 0x3C );
	refMem( 0x004ABAD2, ( DWORD )allocIT + 0x3C );
	refMem( 0x004B1728, ( DWORD )allocIT + 0x3C );
	refMem( 0x004C4384, ( DWORD )allocIT + 0x3C );
	refMem( 0x004C45C0, ( DWORD )allocIT + 0x3C );
	refMem( 0x004C4649, ( DWORD )allocIT + 0x3C );
	refMem( 0x004C4C14, ( DWORD )allocIT + 0x3C );
	refMem( 0x004E4FCC, ( DWORD )allocIT + 0x3C );
	refMem( 0x004E5026, ( DWORD )allocIT + 0x3C );
	refMem( 0x004E5099, ( DWORD )allocIT + 0x3C );
	refMem( 0x004E527A, ( DWORD )allocIT + 0x3C );
	refMem( 0x004E5335, ( DWORD )allocIT + 0x3C );
	refMem( 0x004E868F, ( DWORD )allocIT + 0x3C );
	refMem( 0x00505D86, ( DWORD )allocIT + 0x3C );
	refMem( 0x0050C569, ( DWORD )allocIT + 0x3C );
	refMem( 0x0050C5F7, ( DWORD )allocIT + 0x3C );
	refMem( 0x0050CA95, ( DWORD )allocIT + 0x3C );
	refMem( 0x0050CB54, ( DWORD )allocIT + 0x3C );
	refMem( 0x00433DD3, ( DWORD )allocIT + 0x7C );
	refMem( 0x004C7FF7, ( DWORD )allocIT + 0x7C );
	refMem( 0x00544242, ( DWORD )allocIT + 0x7C );
	refMem( 0x00505C95, ( DWORD )allocIT + 0x80 );
	refMem( 0x004ABA68, ( DWORD )allocIT + 0xFC );
	refMem( 0x004ABA82, ( DWORD )allocIT + 0xFC );
	refMem( 0x004ABA88, ( DWORD )allocIT + 0xFC );
	refMem( 0x004ABAE5, ( DWORD )allocIT + 0xFC );
	refMem( 0x004ABAFF, ( DWORD )allocIT + 0xFC );
	refMem( 0x004ABB05, ( DWORD )allocIT + 0xFC );
	refMem( 0x004ABB45, ( DWORD )allocIT + 0xFC );
	refMem( 0x004B0C9A, ( DWORD )allocIT + 0xFC );
	refMem( 0x004B1749, ( DWORD )allocIT + 0xFC );
	refMem( 0x004C45DA, ( DWORD )allocIT + 0xFC );
	refMem( 0x004C45F7, ( DWORD )allocIT + 0xFC );
	refMem( 0x004C4670, ( DWORD )allocIT + 0xFC );
	refMem( 0x004C46C0, ( DWORD )allocIT + 0xFC );
	refMem( 0x004C46D7, ( DWORD )allocIT + 0xFC );
	refMem( 0x004C4C2E, ( DWORD )allocIT + 0xFC );
	refMem( 0x004C4C4B, ( DWORD )allocIT + 0xFC );
	refMem( 0x004C4C51, ( DWORD )allocIT + 0xFC );
	refMem( 0x004E505E, ( DWORD )allocIT + 0xFC );
	refMem( 0x004E50ED, ( DWORD )allocIT + 0xFC );
	refMem( 0x004E5119, ( DWORD )allocIT + 0xFC );
	refMem( 0x004E511F, ( DWORD )allocIT + 0xFC );
	refMem( 0x004E5291, ( DWORD )allocIT + 0xFC );
	refMem( 0x004E52AB, ( DWORD )allocIT + 0xFC );
	refMem( 0x004E52B4, ( DWORD )allocIT + 0xFC );
	refMem( 0x004E5356, ( DWORD )allocIT + 0xFC );
	refMem( 0x00505E1C, ( DWORD )allocIT + 0xFC );
	refMem( 0x00505E46, ( DWORD )allocIT + 0xFC );
	refMem( 0x00505E66, ( DWORD )allocIT + 0xFC );
	refMem( 0x00505E6C, ( DWORD )allocIT + 0xFC );
	refMem( 0x0050C57C, ( DWORD )allocIT + 0xFC );
	refMem( 0x0050C596, ( DWORD )allocIT + 0xFC );
	refMem( 0x0050C5BB, ( DWORD )allocIT + 0xFC );
	refMem( 0x0050C617, ( DWORD )allocIT + 0xFC );
	refMem( 0x0050C61D, ( DWORD )allocIT + 0xFC );
	refMem( 0x0050CAB8, ( DWORD )allocIT + 0xFC );
	refMem( 0x0050CAD2, ( DWORD )allocIT + 0xFC );
	refMem( 0x0050CB19, ( DWORD )allocIT + 0xFC );
	refMem( 0x0050CB75, ( DWORD )allocIT + 0xFC );
	refMem( 0x0050CB96, ( DWORD )allocIT + 0xFC );
	refMem( 0x004C4356, ( DWORD )allocIT + 0x118 );
	refMem( 0x004C4621, ( DWORD )allocIT + 0x118 );
	refMem( 0x004C80A1, ( DWORD )allocIT + 0x118 );
	refMem( 0x004E506F, ( DWORD )allocIT + 0x118 );
	refMem( 0x004E530D, ( DWORD )allocIT + 0x118 );
	refMem( 0x004E8652, ( DWORD )allocIT + 0x118 );
	refMem( 0x0050C5CD, ( DWORD )allocIT + 0x118 );
	refMem( 0x0050CB28, ( DWORD )allocIT + 0x118 );
	refMem( 0x00449849, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x0044B9FD, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x0044BCE5, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x0044BE69, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x0044BFA9, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x0044C04F, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x0044C0EF, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x0044EA2C, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x0046AE19, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x004C44C6, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x00508F5B, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x0050900F, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x005DAA07, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x005F7496, ( DWORD )allocIT + ItemV + 0x20 );
	refMem( 0x0041E67D, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004B0D12, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004ABB69, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x00433DBE, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004581A1, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004ABA3E, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004ABABB, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004B16EB, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004C4582, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004C4BD3, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004D23E6, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004D4E5F, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004E4FB2, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004E525B, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x00505C83, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x00505D4E, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x00505E28, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x0050909F, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x0050C54F, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x0050CA6F, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x0051F73F, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x0051F9B1, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x0054422C, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x005DA099, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x006226F3, ( DWORD )allocIT + ItemV - 4 );
	refMem( 0x004AD775, ( DWORD )allocIT + ItemV - 8 );
	refMem( 0x004C80BE, ( DWORD )allocIT + ItemV + 0x118 );
}

/*
Finaliza a Tabela;
*/
void ItemCreator::endIT( )
{
	*( DWORD* )( ( DWORD )allocIT + ItemV ) = 0x0A;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 4 ) = 0x64;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 8 ) = 0x03E8;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x0C ) = 0x2710;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x10 ) = 0x0186A0;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x14 ) = 0x0F4240;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x18 ) = 0x989680;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x1C ) = 0x05F5E100;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x20 ) = 0x3B9ACA00;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x24 ) = 0x2710;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x2C ) = 0x12;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x30 ) = 7;
	*( DWORD* )( ( DWORD )allocIT + ItemV + 0x34 ) = 0x0C;
};




void ItemCreator::ReadItems( )
{
	allocIT = VirtualAlloc( NULL, ( 0x314 * ( iTotalItems + 1 ) ), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
	ITEMTABLE_ITEM_COUNT = iTotalItems;
	ItemV = 0;

	FILE * fp;
	fopen_s( &fp, "game\\items\\items.dat", "rb" );
	if ( fp == NULL )
	{
		MessageBox( 0, "Error Code: -5\nSend this error to the Forum.", "Error", MB_ICONEXCLAMATION );
		ExitProcess( 0 );
	}

	fseek( fp, 0, SEEK_END );
	int iSzTable = ftell( fp ) - 0x80;
	fseek( fp, 0, SEEK_SET );

	int itempos = 0x10;

	UINT AllocIT2 = (UINT)VirtualAlloc( NULL, iSzTable, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );

	fseek( fp, 0, SEEK_SET );
	fread( (void*)AllocIT2, 1, iSzTable, fp );

	fclose( fp );

	CompressedItem * psItem = (CompressedItem*)AllocIT2;

	ZeroMemory( (void*)allocIT, ( 0x314 * ( iTotalItems + 1 ) ) );

	class ItemData * psItemTable = (class ItemData*)allocIT;

	int iSize = iSzTable / sizeof( CompressedItem );
	for ( int i = 0; i < iSize; i++ )
	{
		if ( !psItem[ i ].sItemID.ToItemID() )
			break;

		ZeroMemory( &psItemTable[ i ], sizeof( class ItemData ) );
		psItemTable[i].sBaseItemID.SetItemID( psItem[i].sItemID.ToItemID() );
		STRINGCOPY( psItemTable[ i ].szBaseName, psItem[ i ].szBaseName );
		STRINGCOPY( psItemTable[ i ].szInventoryName, psItem[ i ].szInventoryName );
		psItemTable[ i ].iWidth = psItem[ i ].iWidth;
		psItemTable[ i ].iHeight = psItem[ i ].iHeight;
		STRINGCOPY( psItemTable[ i ].szCategory, psItem[ i ].szDropFolder );
		psItemTable[ i ].iItemSlotFlag = (EItemSlotFlag)psItem[ i ].iClass;
		STRINGCOPY( psItemTable[i].szModelName, psItem[i].szDropName );
		psItemTable[ i ].iVisibleItemType = (EItemSlotFlag)psItem[ i ].iModelPosition;
		psItemTable[ i ].iInvItemSound = (EItemInvSound)psItem[ i ].iSound;
		psItemTable[ i ].iItemWpnDmg = psItem[ i ].iWeaponClass;
		ItemV += 0x314;
	}
	
	endIT( );
	refIT( );

	pItemTable = ( ItemData* )allocIT;
}


UINT ItemCreator::findItemPtr( UINT ItemID )
{
	for ( int i = 0; i < iTotalItems; i++ )
	{
		if ( *( UINT* )( ( UINT )allocIT + ( 0x314 * i ) ) == ItemID )
			return ( UINT )allocIT + ( 0x314 * i );
		if ( *( UINT* )( ( UINT )allocIT + ( 0x314 * i ) ) == 0 )
			return 0;
	}
	return 0;
}

UINT ItemCreator::findItemPos( UINT ItemID )
{
	for ( int i = 0; i < iTotalItems; i++ )
	{
		if ( *( UINT* )( ( UINT )allocIT + ( 0x314 * i ) ) == ItemID )
			return i;
		if ( *( UINT* )( ( UINT )allocIT + ( 0x314 * i ) ) == 0 )
			return 0;
	}
	return 0;
}

void ItemCreator::Init()
{
	ReadItems();
}

void __cdecl ItemCreatorInitHook( )
{
	std::shared_ptr<ItemCreator> pItemC = std::make_shared<ItemCreator>( );
	pItemC->Init( );
}

