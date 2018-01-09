/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/OptionsDlg.h $
  * @author:  Dominique Beauregard
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * OptionsDlg is associated with the dialog that allows
  * operators to set the data refresh rate.
  *
  */

#if !defined(AFX_OPTIONSDLG_H__679184BF_2D48_11D5_B140_0050BAB0C7E8__INCLUDED_)
#define AFX_OPTIONSDLG_H__679184BF_2D48_11D5_B140_0050BAB0C7E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Resource.h"
#include "ui_QTForm.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"

namespace TA_Base_App
{
    class OptionsDlgModel;

    class OptionsDlg : public TA_Base_Bus::AbstractTransActiveDialog
    {
        Q_OBJECT
    public:

        /**
          * Constructor
          */
        OptionsDlg(TA_Base_Bus::ITransActiveWinApp * app, QWidget* pParent = NULL);
        virtual  ~OptionsDlg();

        virtual std::string getViewType()
        {
            return "A";
        }


    public slots:

        void clicktest();
        void on_aboutButton_clicked();
        void on_messageBoxButton_clicked();

    protected:

        virtual std::string getMyPModelName();
        void setupPModel();

    protected:

        void initUIControl();
        OptionsDlgModel * m_realPModel;

    private:

        Ui_testForm ui;
    };

} // namespace TA_Base_App

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDLG_H__679184BF_2D48_11D5_B140_0050BAB0C7E8__INCLUDED_)
