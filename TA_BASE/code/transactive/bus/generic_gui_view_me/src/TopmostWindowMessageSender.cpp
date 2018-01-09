#include "bus/generic_gui_view/src/StdAfx.h"
#include "bus/generic_gui_view/src/TopmostWindowMessageSender.h"


namespace TA_Base_Bus
{

    TopmostWindowMessageSender& TopmostWindowMessageSender::instance()
    {
        static TopmostWindowMessageSender instance_;
        return instance_;
    }


    //void TopmostWindowMessageSender::sendMessage( UINT messageID )
    //{
    //    ::EnumWindows(WndEnumHandlerSendMessage, messageID);
    //}

    //void TopmostWindowMessageSender::postMessage( UINT messageID )
    //{
    //    ::EnumWindows(WndEnumHandlerPostMessage, messageID);
    //}


    //BOOL CALLBACK TopmostWindowMessageSender::WndEnumHandlerPostMessage(HWND hWnd, LPARAM lParam)
    //{
    //    DWORD processId = ::GetCurrentProcessId();
    //    DWORD messageID = static_cast<DWORD>(lParam);

    //    // find out which process created this window
    //    DWORD dwThisWindowProcessID;
    //    ::GetWindowThreadProcessId(hWnd, &dwThisWindowProcessID);

    //    // if it's our process
    //    if ( processId == dwThisWindowProcessID )
    //    {
    //        ::PostMessage( hWnd, messageID, 0, 0 );
    //        ::EnumChildWindows(hWnd, WndEnumHandlerPostMessage, lParam);
    //    }

    //    return TRUE;
    //}


    //BOOL CALLBACK TopmostWindowMessageSender::WndEnumHandlerSendMessage(HWND hWnd, LPARAM lParam)
    //{
    //    DWORD processId = ::GetCurrentProcessId();
    //    DWORD messageID = static_cast<DWORD>(lParam);

    //    // find out which process created this window
    //    DWORD dwThisWindowProcessID;
    //    ::GetWindowThreadProcessId(hWnd, &dwThisWindowProcessID);

    //    // if it's our process
    //    if ( processId == dwThisWindowProcessID )
    //    {
    //        ::SendMessage( hWnd, messageID, 0, 0 );
    //        ::EnumChildWindows(hWnd, ChildWndEnumHandlerSendMessage, lParam);
    //    }

    //    return TRUE;
    //}

    //BOOL CALLBACK TopmostWindowMessageSender::ChildWndEnumHandlerSendMessage(HWND hWnd, LPARAM lParam)
    //{
    //    DWORD messageID = static_cast<DWORD>(lParam);
    //    ::SendMessage(hWnd, messageID, 0, 0);
    //    return TRUE;
    //}

    //BOOL CALLBACK TopmostWindowMessageSender::ChildWndEnumHandlerPostMessage(HWND hWnd, LPARAM lParam)
    //{
    //    DWORD messageID = static_cast<DWORD>(lParam);
    //    ::PostMessage(hWnd, messageID, 0, 0);
    //    return TRUE;
    //}

}
