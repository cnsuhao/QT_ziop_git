#pragma once
#include "IModel.h"
#include <set>

namespace TA_Base_App
{
    template<typename View, typename SyncData, typename Service>
    struct AbstractModel : IModel
    {
        typedef View View;
        typedef SyncData SyncData;
        typedef Service Service;
        typedef AbstractModel<View, SyncData, Service> MyAbstractModel;

        AbstractModel(Service* service)
            : m_service(service),
              m_accepted(false)
        {
        }

        virtual ~AbstractModel() { delete m_service; }
        virtual void initSyncData() {}
        virtual void beforeAttachView() {}
        virtual void onDetachView() {}
        virtual void calculateStatus(SyncData& data) {}
        virtual void notifyChangesToView(const SyncData& o, const SyncData& n) {}

        template<typename T>
        void setupModel(T* This)
        {
            m_service->setModel(This);
            initSyncData();
        }

        virtual void calculateInitialStatus()
        {
            calculateStatusOnModelChanged();
        }

        virtual void calculateStatusOnViewChanged()
        {
            calculateStatus(ui);
        }

        virtual void calculateStatusOnModelChanged()
        {
            calculateStatus(pm);
        }

        virtual void attachView2(View* view)
        {
            attachView(view);
        }

        virtual void attachView(View* view)
        {
            if (m_views.empty())
            {
                beforeAttachView();
                onAttachView();
            }

            m_views.insert(view);
        }

        virtual void detachView(View* view)
        {
            m_views.erase(view);

            if (m_views.empty())
            {
                onAttachView();
            }
        }

        virtual void onAttachView()
        {
            m_accepted = false;
            calculateInitialStatus();
            ui = pm;
        }

        virtual void onChanged()
        {
            SyncData old = ui;
            calculateStatusOnViewChanged();
            calculateStatusOnModelChanged();
            ui = pm;

            if (!m_views.empty())
            {
                notifyChangesToView(old, ui);
            }
        }

        virtual void onViewChanged()
        {
            SyncData old = ui;
            calculateStatusOnViewChanged();

            if (!m_views.empty())
            {
                notifyChangesToView(old, ui);
            }
        }

        virtual void onModelChanged()
        {
            SyncData old = ui;
            calculateStatusOnModelChanged();
            ui = pm;

            if (!m_views.empty())
            {
                notifyChangesToView(old, ui);
            }
        }

        virtual void onServiceChanged()
        {
            onModelChanged();
        }

        virtual void onAccept()
        {
            pm = ui;
            m_accepted = true;
        }

        virtual void onReject()
        {
            ui = pm;
            m_accepted = false;
        }

        virtual bool isAccepted()
        {
            return m_accepted;
        }

    public:

        SyncData ui;
        SyncData pm;
        Service* m_service;
        bool m_accepted;
        std::set<View*> m_views;
    };
}

#define NOTIFY(element, function) if (o.element != n.element) { foreachCall(m_views, function, n.element); }
#define NOTIFY2(element1, element2, function) if (o.element1 != n.element1 || o.element2 != n.element2) { foreachCall(m_views, function, n.element1, n.element2); }
