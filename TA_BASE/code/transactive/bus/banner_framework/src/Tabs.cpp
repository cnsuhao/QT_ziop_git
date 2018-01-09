/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/banner_framework/src/Tabs.cpp $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  *
  */
// Tabs.cpp: implementation of the Tabs class.
//
//////////////////////////////////////////////////////////////////////

#include "constants.h"
#include "bus/banner_framework/src/Tabs.h"
#include "bus/banner_framework/src/BannerFrameworkDialog.h"
#include "bus/banner_framework/src/FlashIconButton.h"

#include "core/exceptions/src/BannerFrameworkException.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/TAAssert.h"

#include "bus/banner_framework/src/PrintScreen.h"
#include <QPushButton>
#include <QLayout>
#include <QVBoxLayout>
#include <QRect>

#define IDC_PRINT_BTN 20005

using TA_Base_Core::DebugUtil;

namespace TA_Base_Bus
{
    const int Tabs::BORDER = 3;
	const int Tabs::BUTTON_VERTICAL_SPACING = 2;
    const int Tabs::BUTTON_HEIGHT = 21;
    const int Tabs::MAX_NUMBER_OF_BUTTONS = 10;
    const int Tabs::BUTTON_COLOUR_OFFSET_WHEN_HOT = 30;

	const std::string Tabs::RPARAM_DISABLE_PRINT = "Disableprintbutton";


    Tabs::Tabs(int width, int height, BannerFrameworkDialog* pParent)
    :
    m_bannerDlg(pParent),
    m_width(width),
    m_height(height),
	m_btnPrint(0)
    {
        TA_ASSERT(m_bannerDlg != NULL, "Parent dialog cannot be null");
		this->setLayout(new QVBoxLayout());

		// Load icons
		m_iconCache[BANNER_CONST::BTN_ICO_ALARM_ON] = new QPixmap(":/res/bitmaps/iscs.bmp");
		m_iconCache[BANNER_CONST::BTN_ICO_ALARM_OFF] = new QPixmap(":/res/bitmaps/iscsg.bmp");
		m_iconCache[BANNER_CONST::BTN_ICO_ALARM_ON_DOWN] = new QPixmap(":/res/bitmaps/iscsd.bmp");
		m_iconCache[BANNER_CONST::BTN_ICO_ALARM_OFF_DOWN] = new QPixmap(":/res/bitmaps/iscsdg.bmp");

		m_iconCache[BANNER_CONST::BTN_ICO_CALL_ON] = new QPixmap(":/res/bitmaps/call.bmp");
		m_iconCache[BANNER_CONST::BTN_ICO_CALL_OFF] = new QPixmap(":/res/bitmaps/callg.bmp");
		m_iconCache[BANNER_CONST::BTN_ICO_CALL_ON_DOWN] = new QPixmap(":/res/bitmaps/calld.bmp");
		m_iconCache[BANNER_CONST::BTN_ICO_CALL_OFF_DOWN] = new QPixmap(":/res/bitmaps/calldg.bmp");

		m_iconCache[BANNER_CONST::BTN_ICO_INCIDENT_ON] = new QPixmap(":/res/bitmaps/iscs.bmp");
		m_iconCache[BANNER_CONST::BTN_ICO_INCIDENT_OFF] = new QPixmap(":/res/bitmaps/iscsg.bmp");
		m_iconCache[BANNER_CONST::BTN_ICO_INCIDENT_ON_DOWN] = new QPixmap(":/res/bitmaps/iscsd.bmp");
		m_iconCache[BANNER_CONST::BTN_ICO_INCIDENT_OFF_DOWN] = new QPixmap(":/res/bitmaps/iscsdg.bmp");

		m_iconCache[BANNER_CONST::BTN_ICO_PLAN_ON] = new QPixmap(":/res/bitmaps/plan.bmp");
		m_iconCache[BANNER_CONST::BTN_ICO_PLAN_OFF] = new QPixmap(":/res/bitmaps/plang.bmp");
		m_iconCache[BANNER_CONST::BTN_ICO_PLAN_ON_DOWN] = new QPixmap(":/res/bitmaps/pland.bmp");
		m_iconCache[BANNER_CONST::BTN_ICO_PLAN_OFF_DOWN] = new QPixmap(":/res/bitmaps/plandg.bmp");

		m_iconCache[BANNER_CONST::BTN_ICO_ATS_ON] = new QPixmap(":/res/bitmaps/ats.bmp");
		m_iconCache[BANNER_CONST::BTN_ICO_ATS_OFF] = new QPixmap(":/res/bitmaps/atsg.bmp");
		m_iconCache[BANNER_CONST::BTN_ICO_ATS_ON_DOWN] = new QPixmap(":/res/bitmaps/atsd.bmp");
		m_iconCache[BANNER_CONST::BTN_ICO_ATS_OFF_DOWN] = new QPixmap(":/res/bitmaps/atsdg.bmp");

		m_iconCache[BANNER_CONST::BTN_ICO_ALARMKRTC_ON] = new QPixmap(":/res/bitmaps/iscskrtc.bmp");
		m_iconCache[BANNER_CONST::BTN_ICO_ALARMKRTC_OFF] = new QPixmap(":/res/bitmaps/iscsgkrtc.bmp");
		m_iconCache[BANNER_CONST::BTN_ICO_ALARMKRTC_ON_DOWN] = new QPixmap(":/res/bitmaps/iscsdkrtc.bmp");
		m_iconCache[BANNER_CONST::BTN_ICO_ALARMKRTC_OFF_DOWN] = new QPixmap(":/res/bitmaps/iscsdgkrtc.bmp");

    }


    Tabs::~Tabs()
    {
    }


    void Tabs::createEx()
    {	 
		this->create(); // Creates a default window
		this->setParent(m_bannerDlg);
		QRect rect(0, 0, m_width, m_height - 2);
		this->setGeometry(rect);
		this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::MSWindowsFixedSizeDialogHint);
		
		/*QPalette pal = this->palette();
		pal.setColor(this->backgroundRole(), Qt::blue);
		setAutoFillBackground(true);
		setPalette(pal);*/

		this->setVisible(true);
		if (!TA_Base_Core::RunParams::getInstance().isSet(RPARAM_DISABLE_PRINT.c_str()))
		{
			addPrintButton();
		}
    }


	void Tabs::setHeight(int nHeight)
	{
		m_height = nHeight;
		QRect rect(0, 0, m_width, m_height - 2);
		this->setGeometry(rect);
	}


	void Tabs::addButton(const std::string& buttonId, const BannerIcon& bannerIcon)
    {
        FUNCTION_ENTRY("addButton");

        TA_ASSERT(m_buttons.size() < MAX_NUMBER_OF_BUTTONS, "Too many buttons added to tab!");
    
        // resize the window to hold the extra button
        int numberOfButtons = m_buttons.size() + 1;
	    int buttonWidth = m_width - (BORDER * 2);
        int windowHeight = BUTTON_HEIGHT * numberOfButtons + 1 * numberOfButtons + 1;

        // create the button
	    QPoint topLeft(BORDER, BORDER); //looks better if down two border widths
	    UINT buttonStyle = WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_NOTIFY;
	    QSize buttonSize(buttonWidth, BUTTON_HEIGHT);

		QRect buttonArea(topLeft,  buttonSize);
		buttonArea.moveTo(0, (BUTTON_HEIGHT + BUTTON_VERTICAL_SPACING)*m_buttons.size());
        
        int ctrlId = ID_TAB1 + m_buttons.size();	    
		FlashIconButton* button = new FlashIconButton(bannerIcon, ctrlId, m_iconCache);
		button->setObjectName(QString::fromStdString(buttonId));
		button->setGeometry(buttonArea);
		button->setParent(this);
		button->setVisible(true);
		this->layout()->addWidget(button);

		connect(button, SIGNAL(clicked()), this, SLOT(onTabClicked()));
		connect(button, SIGNAL(doubleClicked()), this, SLOT(onTabDoubleClicked()));

        m_buttons[ buttonId ] = button;
        m_buttonIDs[ ctrlId ] = buttonId;

        LOG_GENERIC( SourceInfo, DebugUtil::DebugInfo, "Adding button ID %s CtrlID %d", buttonId.c_str(), ctrlId );

        FUNCTION_EXIT;
    }

	void Tabs::addPrintButton()
	{
		int nleft = 0;
		nleft = (m_width/2) - 14;

		QRect buttonArea(QPoint(nleft, BORDER), QSize(28, 28));
		buttonArea.moveTo(0, (m_height - 34));

		m_btnPrint = new PrintButton();
		m_btnPrint->setGeometry(buttonArea);
		m_btnPrint->setParent(this);

		m_btnPrint->intialize();

		connect(m_btnPrint, SIGNAL(clicked()), this, SLOT(OnPrint()));
	}

    void Tabs::move(const QPoint& pt)
    {		
		this->setGeometry(pt.x(), pt.y(), 0, 0);    
		this->setVisible(true);
		//ShowWindow(SW_SHOW);
    }


	void Tabs::flashButton(const std::string& buttonId, bool flash)
    {
        FUNCTION_ENTRY("flashButton");

		std::map< std::string, FlashIconButton* >::iterator itButton = m_buttons.find(buttonId);
        if ( itButton == m_buttons.end() )
        {
            LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "Button with ID %d, doesn't exist!", buttonId );

            FUNCTION_EXIT;
            return;
        }

	    if ( true == flash )
	    {
		    itButton->second->StartFlashing();	
	    }
	    else
	    {
		    itButton->second->StopFlashing();
	    }
    }


	void Tabs::selectButton(const std::string& buttonId)
    {
		for (std::map< std::string, FlashIconButton* >::iterator itButton = m_buttons.begin();
             itButton != m_buttons.end();
             ++itButton )
        {
            itButton->second->setCheck(false);
        }
    
        m_buttons[ buttonId ]->setCheck(true);
    }


    void Tabs::onTabClicked()
    {
		QPushButton* button = qobject_cast<QPushButton*>(sender());
		FlashIconButton* fbutton = m_buttons[button->objectName().toStdString()];
		int ctrlId = fbutton->getButtonID();
        if (m_buttonIDs.find(ctrlId) == m_buttonIDs.end())
        {
            LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "Tab with control ID %d doesn't exist", ctrlId );
            return;
        }

		m_bannerDlg->raise();
        m_bannerDlg->showPageByID( m_buttonIDs[ctrlId] );
    }


    void Tabs::onTabDoubleClicked()
    {
		QPushButton* button = qobject_cast<QPushButton*>(sender());
		FlashIconButton* fbutton = m_buttons[button->objectName().toStdString()];
		int ctrlId = fbutton->getButtonID();
        if (m_buttonIDs.find(ctrlId) == m_buttonIDs.end())
        {
            LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "Tab with control ID %d doesn't exist", ctrlId );
            return;
        }

        m_bannerDlg->onTabDoubleClick( m_buttonIDs[ctrlId] );
    }

	void Tabs::OnPrint()
	{
		PrintScreen objPrintScreen;
		m_btnPrint->setPrintableObject(&objPrintScreen);
		m_btnPrint->print();
	}

} // namespace TA_Base_Bus
