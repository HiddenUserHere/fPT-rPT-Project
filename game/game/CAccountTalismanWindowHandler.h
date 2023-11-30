#pragma once



class CAccountTalismanWindowHandler
{
private:
    enum
    {
        WINDOW_Main,

        Window_Error,
        WINDOW_Success,


    };

public:
    CAccountTalismanWindowHandler();
    ~CAccountTalismanWindowHandler();

    void                            Init();

    void                            Update();


};

