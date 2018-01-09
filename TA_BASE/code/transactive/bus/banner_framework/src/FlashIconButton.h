/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/banner_framework/src/FlashIconButton.h $
  * @author:  J. Chevalley
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  * 
  * This class (FlashIconButton) combines standard CButton behaviour with the ability to flash the buttons icon
  * 
  */

#if !defined(AFX_FlashIconButton_H__E025A080_7617_4E81_9FA6_B4138EEF1451__INCLUDED_)
#define AFX_FlashIconButton_H__E025A080_7617_4E81_9FA6_B4138EEF1451__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <QPushButton.h>
#include <QTimer.h>
#include <QPixmap.h>

namespace TA_Base_Bus
{
    class Tabs; // Forward declaration
    struct BannerIcon;

    class FlashIconButton : public QPushButton
    {
		Q_OBJECT
    public:

	    /**
	      * FlashButton CTOR
	      *
	      * Provides the following defaults:
	      *		SetFlashPeriod(500),  // default to 1/2 second
	      *
	      */
		FlashIconButton(const TA_Base_Bus::BannerIcon& bannerIcon, int buttonId, std::map<int, QPixmap*>& iconCache);

	    /**
	      * FlashButton DTOR
	      *
	      */
	    virtual ~FlashIconButton();

    // Implementation
	    
	    /**
	      * isChecked
	      * 
	      * Reports if the button is checked or un-checked.
	      * This method should be used instead of the  CButton GetCheck() method.
	      * See class description above for info on why standard CButton check functionality
	      * is not used.
	      *
	      * @return  true if button is checked, false if it is not
	      *
	      */
	    bool isChecked();


	    /**
	      * ToggleFlash
	      * 
	      * toggles the flash state between flashing or not flashing.
	      *
	      */
	    void ToggleFlash(void);


	    /**
	      * StartFlashing
	      * 
	      * Start the button flashing.
	      * Note that if it is already flashing this .
	      *
	      */
	    virtual void StartFlashing(void);


	    /**
	      * StopFlashing
	      * 
	      * Stop the button flashing.
	      */
	    virtual void StopFlashing(void);


	    /**
	      * SetFlashPeriod
	      * 
	      * Set the flash period, in milliseconds.
	      *
	      * @param flashPeriodMillisec is the flash period, in milliseconds.
	      *
	      */
	    virtual void SetFlashPeriod(unsigned long flashPeriodMillisec);

	    void setCheck(bool checkIt);

		int getButtonID() { return m_buttonId; }
	    // Generated message map functions
	public slots:
	    void OnTimer();
	 
    private:
	    bool GetNextFlashState(void);

    private:
		// The timer  for updates and flashing 
		QTimer m_timer;

	    bool m_bFlashing;
	    bool m_currentFlashStateOn;
	    unsigned long m_flashPeriodMillisec;
	 
		QPixmap* m_onImage;
		QPixmap* m_offImage;
		QPixmap* m_offDownImage;
		QPixmap* m_onDownImage;

	    int  m_buttonId;
	    //See class comments as to why I am not using the standard CButton "checked" functionality	
	    bool m_isChecked;
    };

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

}
#endif // !defined(AFX_FlashIconButton_H__E025A080_7617_4E81_9FA6_B4138EEF1451__INCLUDED_)
