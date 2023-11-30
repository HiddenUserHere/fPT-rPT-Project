#pragma once
class CCarnivalWindowHandler
{
public:
    CCarnivalWindowHandler();
    virtual ~CCarnivalWindowHandler();

    void                Init();

    void                OnResolutionChanged();

    void                Open();
    void                Close();

    UIWINDOW_DEFAULT_HEADER_CREATOR;

private:
    UI::Window_ptr          pWindow1 = nullptr;

    DWORD               dwCloseTime = 0;

    void                BuildWindow1();

    void                OnButtonCloseClick( UIEventArgs eArgs );
    void                OnButtonRewardClick( UIEventArgs eArgs );

    void				OnAcceptRewardClick();
};

