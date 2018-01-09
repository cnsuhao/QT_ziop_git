#include "bus/generic_gui_view/src/ResizeViewHelper.h"
#include "core/utilities/src/DebugUtil.h"
#include <QMainWindow>
#include <QEvent>
#include <QDesktopWidget>
#include <QResizeEvent>


namespace TA_Base_Bus
{

    ResizedViewHelper::ResizedViewHelper( ITransActiveWinApp * pApp, QMainWindow* window )
        : m_window(window),
          m_winApp(pApp)
    {
        m_desktop.reset( new QDesktopWidget );
        m_lastRect.reset( new QRect );
    }


    void ResizedViewHelper::moveToWorkArea()
    {
        QRect oldRect = m_window->frameGeometry();

        if ( oldRect == *m_lastRect )
        {
            return;
        }

        *m_lastRect = oldRect;

        TA_Rect workingArea_;
        m_winApp->getApplicationController()->getWorkAreaFromControlStation( TA_Rect::fromQRect( oldRect ), workingArea_ );

        {
            // test
            workingArea_.top = 100;
            workingArea_.bottom = 1100;
        }

        if ( workingArea_.empty() )
        {
            return;
        }

        QRect workingArea = TA_Rect::toQRect<QRect>( workingArea_ );
        QRect newRect = oldRect;

        if ( workingArea.height() < newRect.height() )
        {
            newRect.setBottom( newRect.top() + workingArea.height() - 1 );
        }

        if ( newRect.top() < workingArea.top() )
        {
            int delta = workingArea.top() - newRect.top();
            newRect.setTop( newRect.top() + delta );
            newRect.setBottom( newRect.bottom() + delta );
        }

        if ( workingArea.bottom() < newRect.bottom() )
        {
            int delta = newRect.bottom() - workingArea.bottom();
            newRect.setTop( newRect.top() - delta );
            newRect.setBottom( newRect.bottom() - delta );
        }

        if ( newRect.topLeft() != oldRect.topLeft() )
        {
            m_window->move( newRect.topLeft() );
        }

        if ( newRect.height() < oldRect.height()  )
        {
            int delta = oldRect.height() - newRect.height();
            QSize newSize = m_window->size();
            newSize.setHeight( newSize.height() - delta );
            m_window->resize( newSize );
        }
    }


    void ResizedViewHelper::onResizeEvent( QResizeEvent* e )
    {
        int desktopAvailableHeight = m_desktop->availableGeometry().height();
        int nonClientAreaHeight = m_window->frameGeometry().height() - m_window->height();

        int newHeight = e->size().height();
        int oldHeight = e->oldSize().height();

        if ( oldHeight < newHeight )
        {
            bool isOldHeightMaximized = ( desktopAvailableHeight <= nonClientAreaHeight + oldHeight );
            bool isNewHeightMaximized = ( desktopAvailableHeight <= nonClientAreaHeight + newHeight );

            if ( isNewHeightMaximized && !isOldHeightMaximized )
            {
                moveToWorkArea();
            }
        }
    }


    void ResizedViewHelper::onEvent( QEvent* e )
    {
        if ( e->type() == QEvent::NonClientAreaMouseMove )
        {
            moveToWorkArea();
        }
    }

}
