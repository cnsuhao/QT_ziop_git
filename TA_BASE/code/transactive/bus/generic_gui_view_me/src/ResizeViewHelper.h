#ifndef TRANSACTIVE_VIEW_HELPER_H__
#define TRANSACTIVE_VIEW_HELPER_H__

#include "bus/generic_gui_pmod/src/PModelDataType.h"
#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include <boost/shared_ptr.hpp>

class QRect;
class QEvent;
class QMainWindow;
class QResizeEvent;
class QDesktopWidget;


namespace TA_Base_Bus
{

    class ResizedViewHelper
    {
    public:

        ResizedViewHelper( ITransActiveWinApp * pApp, QMainWindow* window );

        /**
          * move to specified work area
          *
          * This method is create for Move dialog to the work area,
          * do not cover the banner and tool bar (otherwise the work area is the schamatic area)
          * the return value windowRect is the modified dialog position
          */
        void moveToWorkArea();
        void onResizeEvent( QResizeEvent* e );
        void onEvent( QEvent* e );

    private:

        QMainWindow* m_window;
        ITransActiveWinApp* m_winApp;
        boost::shared_ptr<QRect> m_lastRect;
        boost::shared_ptr<QDesktopWidget> m_desktop;
    };

}

#endif
