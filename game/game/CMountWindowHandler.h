#pragma once
class CMountWindowHandler
{
private:
    struct MountItemGame
    {
        int                     iMountID;

        std::string             strName;

        EMountType              eMountType;
        EMountRareType          eRareType;
        BOOL                    bIsSelected;

        std::vector<Item>       vItems;
    };

    UI::Window_ptr              pWindow1 = nullptr;

    UINT                        uMountWheelID = 0;

    std::vector<MountItemGame *> vMountItemsData;

    enum
    {
        WINDOW_Main,
        BUTTON_Close,

        LIST_Mounts,

        ITEMBOX_Item1,
        ITEMBOX_Item2,
        ITEMBOX_Item3,

        IMAGEBOX_Item1,
        IMAGEBOX_Item2,
        IMAGEBOX_Item3,

        IMAGEBOX_MountBox,
        IMAGEBOX_MountBoxSelected,
        IMAGEBOX_MountImage,

        TEXT_MountName,

        WINDOW_Mounts = 500,
    };

    int                         iCurrentMountWindowID = -1;

    EMountType                  eCurrentMountType = EMountType::MOUNTTYPE_None;

public:
    CMountWindowHandler();
    virtual ~CMountWindowHandler();

    std::vector<MountItemGame *> & GetMounts() { return vMountItemsData; }

    EMountType                  GetMountType() { return eCurrentMountType; }

    BOOL                        IsOpen() { return pWindow1->IsOpen(); }

    void                        ClearMounts();

    void                        Init();

    void                        Render();

    void                        Update( float fTime );

    void						OnMouseMove( class CMouse * pcMouse );
    BOOL						OnMouseClick( class CMouse * pcMouse );
    BOOL						OnMouseScroll( class CMouse * pcMouse );

    BOOL						OnKeyPress( class CKeyboard * pcKeyboard );
    BOOL						OnKeyChar( class CKeyboard * pcKeyboard );

    void						OnResolutionChanged();

    void                        Open();

    void                        Close();

    void                        AddMount( int iID, const std::string strMountName, const std::string strMountImageFile, EMountType eMountType, EMountRareType eRareType, Item * psaItems, int iCount, BOOL bIsSelected );

private:
    void                        BuildWindow1();

    void                        AddMountWindow( int iID, const std::string strMountName, const std::string strMountImageFile, EMountType eMountType, EMountRareType eRareType, BOOL bIsSelected );

    void                        AddEquipmentWindow( int iID, const std::string strMountName, EMountType eMountType, EMountRareType eRareType, Item * psaItems, int iCount );

    void                        OnButtonCloseClick( UIEventArgs eArgs );

    void                        OnButtonMountClick( int iID, EMountType eMountType, UIEventArgs eArgs );

    void                        OnButtonSelectMountWindow( int iID, EMountType eMountType, UIEventArgs eArgs );

    void                        OnGetItemSlot( int iImageID, int iItemBoxID, UIEventArgs eArgs );
    void                        OnPutItemSlot( int iImageID, int iItemBoxID, UIEventArgs eArgs );

    void                        ReorderItems( MountItemGame * psMountItemsData );
};

