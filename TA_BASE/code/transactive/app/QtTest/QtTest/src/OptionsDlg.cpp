/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/OptionsDlg.cpp $
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

#pragma warning(disable:4786)

#include "OptionsDlg.h"
#include "pmodel/src/OptionsDlgModel.h"
#include "pmodel/src/SystemManagerConsts.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"
#include "bus/generic_gui_view/src/HelpLauncher.h"
#include "bus/generic_gui_pmod/src/PModelDataType.h"
#include "core/threads/src/Thread.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/generic_gui_view/src/TransActiveMessageConfigParser.h"
#include <QVariant>
#include <QMessageBox>
#include <QtGui/QApplication>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <vector>
#include <sstream>


namespace TA_Base_App
{

    OptionsDlg::OptionsDlg(TA_Base_Bus::ITransActiveWinApp * app, QWidget* pParent /*=NULL*/)
        : TA_Base_Bus::AbstractTransActiveDialog(app, pParent)
        , m_realPModel(NULL)
    {
        //{{AFX_DATA_INIT(OptionsDlg)
            // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
        ui.setupUi(this);
        //this->setAttribute( Qt::WA_DeleteOnClose);
        connect (ui.btnTest,SIGNAL(clicked()),this,SLOT(clicktest()));

        ui.plainTextEdit->setPlainText( "MessageBox id" );
        ui.textEdit->setPlainText( "MessageBox caption" );
        ui.textEdit_2->setPlainText( "MessageBox parameters" );
    }


    OptionsDlg::~OptionsDlg()
    {
        // QApplication::exit()
    }


    void OptionsDlg::setupPModel()
    {
        m_realPModel = dynamic_cast<OptionsDlgModel *> (m_pModel);
    }


    void OptionsDlg::clicktest()
    {
        //中4E2D  文6587
        //中%E4%B8%AD   文%E6%96%87

        QString qstr=ui.textEdit->toPlainText();
        std::string str=qstr.toStdString();

        QString qstr2;
        qstr2=QString::fromStdString(str);
        //qstr2.fromUtf8(str.c_str());

        ui.plainTextEdit->setPlainText(qstr2);

        int i=0;
        i++;
    }


    void OptionsDlg::initUIControl()
    {
    }


    std::string OptionsDlg::getMyPModelName()
    {
        return PMODEL_OPTIONS_DLG;
    }


    void OptionsDlg::on_aboutButton_clicked()
    {
        TA_Base_Bus::HelpLauncher::getInstance().displayAboutBox();
    }


    void OptionsDlg::on_messageBoxButton_clicked()
    {
        TA_Base_Bus::TransActiveMessage transactiveMessage;
        std::string id = ui.plainTextEdit->toPlainText().toStdString();
        std::string caption = ui.textEdit->toPlainText().toStdString();
        std::string parameterStr = ui.textEdit_2->toPlainText().toStdString();
        std::vector<std::string> parameters;
        boost::split( parameters, parameterStr, boost::bind( std::equal_to<char>(), '\n', _1 ), boost::token_compress_on );

        for ( size_t i = 0; i < parameters.size(); ++i )
        {
            if ( ! parameters[i].empty() )
            {
                transactiveMessage << parameters[i];
            }
        }

        try
        {
            TA_Base_Bus::TransActiveMessageConfig* config = getMessageConfig( boost::lexical_cast<int>( id ) );

            if ( config != NULL  )
            {
                transactiveMessage.showMsgBox( config->id, config->resID + " " + caption );
            }
            else
            {
                QMessageBox( QMessageBox::Warning, "", QString::fromStdString( "cannot find id: " + id ) ).exec();
            }
        }
        catch ( ... )
        {
            QMessageBox( QMessageBox::Critical, "", QString::fromStdString( "invalid id: " + id ) ).exec();
        }
    }

}
