#include "MFCControlHelper.h"
//#include "bus/generic_gui_pmod/src/GenericGuiConstants.h"
#include "core/utilities/src/DebugUtil.h"

namespace TA_Base_Bus
{


    //bool MFCControlHelper::setWindowPosition( CWnd * parentWnd, WPARAM wParam, LPARAM lParam )
    //{
    //    TA_ASSERT(parentWnd != NULL, "window handle is not NULL");

    //    CWnd* childWindow=NULL;
    //    if (lParam == TA_GenericGui::MINIMISE)
    //    {
    //        parentWnd->ShowWindow(SW_MINIMIZE);
    //        return true;     // just return straightaway
    //    }

    //    if(lParam!= TA_GenericGui::REPOSITION_NO_RESTORE)
    //    {
    //        if(parentWnd->IsIconic())
    //        {
    //            parentWnd->ShowWindow(SW_RESTORE);
    //        }
    //    }

    //    switch (lParam)
    //    {
    //    case TA_GenericGui::MAXIMISE:
    //        parentWnd->ShowWindow(SW_MAXIMIZE);
    //        break;
    //    case TA_GenericGui::FOCUS:
    //    case TA_GenericGui::NO_REPOSITION:
    //        break;
    //    case TA_GenericGui::REPOSITION:
    //    case TA_GenericGui::REPOSITION_NO_RESTORE:
    //        {
    //            TA_Rect* rect = reinterpret_cast<TA_Rect*>(wParam);

    //            TA_ASSERT(rect != NULL, "Cannot reposition window with NULL rect");
    //            TA_ASSERT((rect->right >= rect->left ) && (rect->bottom >= rect->top), "The rect is not valid");
    //            parentWnd->MoveWindow(rect);

    //            childWindow = parentWnd->GetLastActivePopup();
    //            if(childWindow->GetDlgCtrlID() != parentWnd->GetDlgCtrlID())
    //            {
    //                childWindow->CenterWindow(parentWnd);
    //            }
    //        }
    //        break;
    //    case TA_GenericGui::CENTRE:
    //        parentWnd->CenterWindow(CWnd::GetDesktopWindow());
    //        break;
    //    default:
    //        TA_ASSERT(false, "Invalid enumerated type passed for setting window position");
    //    }
    //    if(childWindow!=NULL && childWindow->GetDlgCtrlID() != parentWnd->GetDlgCtrlID())
    //    {
    //        childWindow->SetForegroundWindow();
    //        // Set the child window to be the top most so it is given focus and then move it back again.
    //        childWindow->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOSENDCHANGING);
    //        childWindow->SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOSENDCHANGING);
    //    }
    //    else
    //    {
    //        parentWnd->SetForegroundWindow();
    //        // Set the window to be the top most so it is given focus and then move it back again.
    //        parentWnd->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOSENDCHANGING);
    //        parentWnd->SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOSENDCHANGING);
    //    }
    //    return true;
    //}
}
