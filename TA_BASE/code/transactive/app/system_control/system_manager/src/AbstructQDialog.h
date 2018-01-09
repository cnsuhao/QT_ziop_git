#pragma once
#include "IView.h"
#include "app/system_control/system_manager/pmodel/src/IModel.h"
#include <QDialog>
#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace TA_Base_App
{
    struct AbstructQDialog : QDialog, virtual IView
    {
        Q_OBJECT

    protected:

        AbstructQDialog(bool is_modal = false)
            : m_isModal(is_modal)
        {
            QDialog::setAttribute(Qt::WA_DeleteOnClose);
        }

        ~AbstructQDialog()
        {
        }

        virtual void setupSignalSlotConnection() {}
        virtual void loadFromModel() {}

        template<typename T>
        void setupDialog(T* This)
        {
            This->ui.setupUi(This);
            This->m_model->attachView(This);
            This->loadFromModel();
            connect(This, SIGNAL(enableOkSignal(bool)), This->ui.buttonBox->button(QDialogButtonBox::Ok), SLOT(setEnabled(bool)));
            connect(This->ui.buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
            connect(This->ui.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
            connect(this, SIGNAL(retranslateSignal()), this, SLOT(retranslateSlot()));
            This->setupSignalSlotConnection();
            makeFunction(This, This->m_model);
        }

        template<typename T, typename Model> void makeFunction(T* This, Model* m_model)
        {
            m_onReject = boost::bind(&Model::onReject, m_model);
            m_onAccept = boost::bind(&Model::onAccept, m_model);
            m_retranslateUi = boost::bind(&T::UI::retranslateUi, This->ui, This);
        }

        virtual void display()
        {
            m_isModal ? QDialog::exec() : QDialog::show();
        }

        virtual void onModelEnableOkButton(bool enable)
        {
            emit enableOkSignal(enable);
        }

        virtual void onSwitchLanguage()
        {
            emit retranslateSignal();
        }

    protected slots:

        virtual void accept()
        {
            QDialog::accept();
            m_onAccept();
        }

        virtual void reject()
        {
            QDialog::reject();
            m_onReject();
        }

        void retranslateSlot()
        {
            m_retranslateUi();
        }

    signals:

        void enableOkSignal(bool enable);
        void retranslateSignal();

    private:

        bool m_isModal;
        boost::function<void()> m_onReject;
        boost::function<void()> m_onAccept;
        boost::function<void()> m_retranslateUi;
        boost::function<void()> m_detachView;
    };
}
