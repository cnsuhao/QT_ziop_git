#if !defined(TOPMOSTWINDOWMESSAGESENDER_H_INCLUDED)
#define TOPMOSTWINDOWMESSAGESENDER_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


namespace TA_Base_Bus
{

    class TopmostWindowMessageSender
    {
    public:

        static TopmostWindowMessageSender& instance();

        //void sendMessage( UINT messageID );
        //void postMessage( UINT messageID );

    private:

        //static BOOL CALLBACK WndEnumHandlerSendMessage(HWND hWnd, LPARAM lParam);
        //static BOOL CALLBACK WndEnumHandlerPostMessage(HWND hWnd, LPARAM lParam);
        //static BOOL CALLBACK ChildWndEnumHandlerSendMessage(HWND hWnd, LPARAM lParam);
        //static BOOL CALLBACK ChildWndEnumHandlerPostMessage(HWND hWnd, LPARAM lParam);
    };

}


#endif
