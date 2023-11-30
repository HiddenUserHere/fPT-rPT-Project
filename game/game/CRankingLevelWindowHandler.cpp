#include "stdafx.h"
#include "CRankingLevelWindowHandler.h"

CRankingLevelWindowHandler::CRankingLevelWindowHandler()
{
	pWindow = std::make_shared<UI::Window>( Rectangle2D( 0, 0, 652, 547 ) );
	pWindow->Hide();
}

CRankingLevelWindowHandler::~CRankingLevelWindowHandler()
{
    pWindow = nullptr;
}

void CRankingLevelWindowHandler::Open()
{
	if ( pWindow->IsOpen() == FALSE )
	{
		pWindow->Show();
		MOUSEHANDLER->BlockMouse();

		AddWebElement();
	}
}

void CRankingLevelWindowHandler::Close()
{
	if ( pWindow->IsOpen() )
	{
		pWindow->Hide();
		MOUSEHANDLER->UnblockMouse();
	}
}

void CRankingLevelWindowHandler::ClearData()
{
//	if ( auto pWebView = pWindow->GetElement<UI::WebViewer>( WEBVIEW_Ranking ) )
//		pWebView->CallFunction( "clearTable();" );
}

void CRankingLevelWindowHandler::AddData( Ranking::RankingLevelData & sRanking )
{
	vRankings.push_back( sRanking );
}

void CRankingLevelWindowHandler::Init()
{
	BuildWindow();
}

void CRankingLevelWindowHandler::Update( float fTime )
{
	pWindow->Update( fTime );
}

void CRankingLevelWindowHandler::Render()
{
	pWindow->Render( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

void CRankingLevelWindowHandler::OnMouseMove( CMouse * pcMouse )
{
	pWindow->OnMouseMove( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CRankingLevelWindowHandler::OnMouseClick( CMouse * pcMouse )
{
    return pWindow->OnMouseClick( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CRankingLevelWindowHandler::OnMouseScroll( CMouse * pcMouse )
{
    return pWindow->OnMouseScroll( pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );
}

BOOL CRankingLevelWindowHandler::OnKeyPress( CKeyboard * pcKeyboard )
{
    return pWindow->OnKeyPress( pcKeyboard );
}

BOOL CRankingLevelWindowHandler::OnKeyChar( CKeyboard * pcKeyboard )
{
	return pWindow->OnKeyChar( pcKeyboard );
}

void CRankingLevelWindowHandler::OnResolutionChanged()
{
	//Base Rectangle
	int iWindowWidth = pWindow->GetWidth();
	int iWindowHeight = pWindow->GetHeight();
	int iWindowX = (RESOLUTION_WIDTH >> 1) - (iWindowWidth >> 1);
	int iWindowY = (RESOLUTION_HEIGHT >> 1) - (iWindowHeight >> 1);

	pWindow->SetPosition( iWindowX, iWindowY );
}

void CRankingLevelWindowHandler::BuildWindow()
{
	pWindow->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\window3.png" ) );

	//Header
	UI::ImageBox_ptr pHeaderImage = std::make_shared<UI::ImageBox>( Rectangle2D( 160, 0, 337, 51 ) );
	pHeaderImage->SetImage( UI::ImageLoader::LoadImage( "game\\images\\UI\\window\\text\\header_ranking.png" ) );
	pWindow->AddElement( pHeaderImage );

	//Base Rectangle
	int iWindowWidth = pWindow->GetWidth();
	int iWindowHeight = pWindow->GetHeight();
	int iWindowX = (RESOLUTION_WIDTH >> 1) - (iWindowWidth >> 1);
	int iWindowY = (RESOLUTION_HEIGHT >> 1) - (iWindowHeight >> 1);
	pWindow->SetPosition( iWindowX, iWindowY );

	//Button Close
	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>( Rectangle2D( 589, 14, 30, 28 ) );
	pButtonClose->SetActiveImage( UI::ImageLoader::LoadActiveImage( "game\\images\\hud\\social\\closebtn.png", "game\\images\\hud\\social\\closebtn_.png" ) );
	pButtonClose->SetEvent( UI::Event::Build( std::bind( &CRankingLevelWindowHandler::OnButtonCloseClick, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pButtonClose );

	pWindow->Hide();

	OnResolutionChanged();
}

void CRankingLevelWindowHandler::AddWebElement()
{
	/*
	if ( auto pWebView = pWindow->GetElement<UI::WebViewer>( WEBVIEW_Ranking ) )
		pWindow->DelElement( pWebView );

	UI::WebViewer_ptr pWeb = std::make_shared<UI::WebViewer>( Rectangle2D( 35, 54, 568, 428 ) );
	pWeb->SetID( WEBVIEW_Ranking );
	pWeb->LoadFromHTMLFile( "game\\npcs\\rankings\\level\\index.html" );
	pWeb->SetEventOnDOMReady( UI::Event::Build( std::bind( &CRankingLevelWindowHandler::OnDOMReady, this, std::placeholders::_1 ) ) );
	pWindow->AddElement( pWeb );
	*/
}

void CRankingLevelWindowHandler::OnButtonCloseClick( UIEventArgs eArgs )
{
	Close();
}

void CRankingLevelWindowHandler::OnDOMReady( UIEventArgs eArgs )
{
	auto ReplaceParamString = []( std::string str, const std::string strFind, const std::string strValue ) -> std::string
	{
		std::string strRet = str;
		size_t iFind = str.find( strFind );
		if ( iFind != std::string::npos )
			strRet = str.replace( str.begin() + iFind, str.begin() + iFind + strFind.length(), strValue.begin(), strValue.end() );

		return strRet;
	};
	/*
	if ( auto pWebView = pWindow->GetElement<UI::WebViewer>( WEBVIEW_Ranking ) )
	{
		pWebView->CallFunction( R"PROG(setHeader(["No", "Character Name", "", "Class", "Clan", "Level", "%"]);)PROG" );

		const char * FUNCTION_DATA = R"PROG(appendRow([{number}, "{name}", "<img src='img/class/{class_num}.gif'>", "{class_name}", "{clan_name}", "{level}", "{percent_exp}"]);)PROG";
		for ( auto & sRanking : vRankings )
		{
			std::string str = FUNCTION_DATA;
			str = ReplaceParamString( str, "{number}", std::to_string( sRanking.iNumber ) );
			str = ReplaceParamString( str, "{name}", sRanking.szCharacterName );
			str = ReplaceParamString( str, "{class_num}", std::to_string( sRanking.eCharacterClass ) );
			str = ReplaceParamString( str, "{class_name}", GetCharacterClassByFlag( CharacterClassToClassFlag( sRanking.eCharacterClass ) ) );
			str = ReplaceParamString( str, "{clan_id}", std::to_string( sRanking.iClanID ) );
			str = ReplaceParamString( str, "{clan_name}", sRanking.szClanName );
			str = ReplaceParamString( str, "{level}", std::to_string( sRanking.iCharacterLevel ) );
			str = ReplaceParamString( str, "{percent_exp}", FormatString( "%.4f%%", sRanking.fCharacterPercentEXP ) );

			pWebView->CallFunction( str.c_str() );
		}
	}
	*/
}
