#pragma once
#include "bus/generic_gui_view/src/TransActiveDialog.h"
#include <qobjectdefs.h>


namespace TA_Base_App
{

    template<typename IView, typename Ui, typename Model>
    struct MyUi
    {
        MyUi()
        {
            ui.setupUi(this);
            m_model->setView(this);
        }

        Model* m_model;
        Ui ui;
    };


    class MyAbstractDialog : public TA_Base_Bus::AbstractDialog
    {
        Q_OBJECT

    public:

        MyAbstractDialog(TA_Base_Bus::ITransActiveApp* app)
            : AbstractDialog(app, "", NULL)
        {
            setupSlotConnection();
            loadFromModel();
        }

        virtual void show()
        {
            QDialog::exec();
        }

    public slots:

        virtual void accept()
        {
            QDialog::accept();
        }

        virtual void reject()
        {
            QDialog::reject();
        }

    protected:

        virtual std::string getMyPModelName() {}
        virtual void setupPModel() {}
        virtual void initUIControl() {}

    public:

        virtual void setupSlotConnection() = 0;
        virtual void loadFromModel() = 0;
    };
}




#if 0
template<typename AbstractDialog, typename IView, typename Ui, typename Model>
struct MyAbstractDialog : AbstractDialog, IView
{
    Q_OBJECT

public:

    MyAbstractDialog(TA_Base_Bus::ITransActiveApp* app, Model* model)
        : AbstractDialog(app, "", NULL),
          m_model(model)
    {
        ui.setupUi(this);
        connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
        connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

        setupSlotConnection();
        m_model->setView(this);
        loadFromModel();
    }

    ~MyAbstractDialog()
    {
        m_model->setView(NULL);
    }

    virtual void show()
    {
        QDialog::exec();
    }

public slots:

    virtual void accept()
    {
        QDialog::accept();
        m_model->onAccept();
    }

    virtual void reject()
    {
        QDialog::reject();
        m_model->onReject();
    }

protected:

    virtual std::string getMyPModelName() {}
    virtual void setupPModel() {}
    virtual void initUIControl() {}

public:

    virtual void setupSlotConnection() = 0;
    virtual void loadFromModel() = 0;

    Model* m_model;
    Ui ui;

public:

};
#endif
