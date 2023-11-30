#pragma once
class CRankingLevelWindowHandler
{
private:
    UI::Window_ptr                  pWindow = nullptr;

    std::vector<Ranking::RankingLevelData> vRankings;

public:
    CRankingLevelWindowHandler();
    virtual ~CRankingLevelWindowHandler();

    void                            Open();

    void                            Close();

    void                            ClearData();
    void                            AddData( Ranking::RankingLevelData & sRanking );

    void                            Init();

    void                            Update( float fTime );

    void                            Render();

    void							OnMouseMove( class CMouse * pcMouse );
    BOOL							OnMouseClick( class CMouse * pcMouse );
    BOOL							OnMouseScroll( class CMouse * pcMouse );

    BOOL							OnKeyPress( class CKeyboard * pcKeyboard );
    BOOL							OnKeyChar( class CKeyboard * pcKeyboard );

    void                            OnResolutionChanged();

private:
    void                            BuildWindow();

    void                            AddWebElement();

    void                            OnButtonCloseClick( UIEventArgs eArgs );

    void                            OnDOMReady( UIEventArgs eArgs );

    enum
    {
        WEBVIEW_Ranking = 500,
    };
};

