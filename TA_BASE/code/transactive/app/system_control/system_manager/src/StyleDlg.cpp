/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/StyleDlg.cpp $
* @author:  Dominique Beauregard
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2012/02/06 16:15:14 $
* Last modified by:  $Author: haijun.li $
*
* StyleDlg is associated with the dialog that allows
* operators to set the data refresh rate.
*
*/
#include "StyleDlg.h"
#include "SystemManagerApp.h"
#include "QtUtilities.h"
#include "StyleDlgModel.h"
#include <QPushButton>
#include <QStyleFactory>
#include <boost/foreach.hpp>


namespace TA_Base_App
{
    StyleDlg::StyleDlg(StyleDlgModel* model)
        : AbstructQDialogTemplate(model)
    {
        setupDialog(this);
    }


    void StyleDlg::setupSignalSlotConnection()
    {
        connect(ui.language, SIGNAL(currentIndexChanged(int)), this, SLOT(onLanguageIndexChanged(int)));
        connect(ui.style, SIGNAL(currentIndexChanged(int)), this, SLOT(onStyleIndexChanged(int)));
        connect(ui.stylesheet, SIGNAL(currentIndexChanged(int)), this, SLOT(onStyleSheetIndexChanged(int)));
    }


    void StyleDlg::loadFromModel()
    {
        ui.language->addItems(QtUtilities::convertTo<QStringList>(m_model->ui.languages));
        ui.style->addItems(QStyleFactory::keys());
        ui.stylesheet->addItems(QtUtilities::convertTo<QStringList>(m_model->ui.stylesheets));
        ui.language->setCurrentIndex(m_model->ui.language_index);
        ui.style->setCurrentIndex(m_model->ui.style_index);
        ui.stylesheet->setCurrentIndex(m_model->ui.stylesheet_index);
    }


    void StyleDlg::onLanguageIndexChanged(int index)
    {
        m_model->ui.language_index = index;
        const char* languages[] = { "en_US", "zh_CN" };
        emit dynamic_cast<SystemManagerApp*>(qApp)->changeLanguage(languages[index]);

        BOOST_FOREACH(IView * view, allViews())
        {
            view->onSwitchLanguage();
        }
    }


    void StyleDlg::onStyleIndexChanged(int index)
    {
        m_model->ui.style_index = index;
        qApp->setStyle(QStyleFactory::create(ui.style->currentText()));
    }


    void StyleDlg::onStyleSheetIndexChanged(int index)
    {
        m_model->ui.stylesheet_index = index;
        emit dynamic_cast<SystemManagerApp*>(qApp)->changeStyleSheet(ui.stylesheet->currentText().toStdString() + ".qss");
    }
}
