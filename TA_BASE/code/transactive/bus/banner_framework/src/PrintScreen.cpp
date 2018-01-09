

#include "core/utilities/src/DebugUtil.h"
#include "bus\banner_framework\src\PrintScreen.h"
#include <QList>
#include <QScreen>
#include <QGuiApplication>
#include <QApplication>
#include <QDesktopWidget>
#include <QPoint>

#include <QtPrintSupport\QPrintDialog.h>
#include <QtPrintSupport\QPrinter.h>
#include <QPainter.h>

namespace TA_Base_Bus
{
	// On Construction capture the screen
	PrintScreen::PrintScreen()
	{
		// Determine Screen information
		m_numOfScreens = QGuiApplication::screens().size();
		QRect rec = QApplication::desktop()->screenGeometry();
		m_SrcHeight = rec.height();
		m_ScrWidth = rec.width();

		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Screen info: width = %ld, height = %ld and Number of Screen = %d", m_ScrWidth, m_SrcHeight, m_numOfScreens);

		int screenPos = 1;
		//Determined what screen we are
		QPoint pt = QCursor::pos();
	
		if (m_numOfScreens > 1)
		{	// Multiple screen
			for (int i=0; i< m_numOfScreens; i++)
			{
				if (pt.x() >((i*m_ScrWidth) + 1) && pt.x() < ((i + 1)*m_ScrWidth))
				{
					break;
				}
				screenPos++;
			}
		}

		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Capture print screen at monitor = %d", screenPos);

		//Capture the correct screen
		captureScreen(screenPos);
	}

	PrintScreen::~PrintScreen()
	{
		// Destroys the objects
	}

	void PrintScreen::printObject()
	{
		QPrinter printer(QPrinter::HighResolution);		
		QPrintDialog* printDlg = new QPrintDialog(&printer);
		printDlg->setWindowTitle("Print Document");
		
		if (printDlg->exec() != QDialog::Accepted)
			return;

		QPainter painter;

		painter.begin(&printer);
		painter.drawPixmap(0, 0, m_pixMap);
		painter.end();
	}

	void PrintScreen::captureScreen(const int screenNum)
	{
		QList<QScreen *> screens = QGuiApplication::screens();
		if (screens.size() != 0 && screens.size() >= screenNum)
		{
			m_pixMap = screens[screenNum - 1]->grabWindow(0);
		}
	}

}