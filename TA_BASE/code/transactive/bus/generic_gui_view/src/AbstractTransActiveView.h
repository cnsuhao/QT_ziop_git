#pragma once
#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "core/utilities/src/TAAssert.h"
#include <boost/shared_ptr.hpp>

namespace TA_Base_Bus
{
    /** Abstract View class under GenericGUI framework
      *
      */
    class AbstractTransActiveView: public ITransActiveView
    {
    public:

        /** Constructor function
          *
          *
          *  @param pApp
          *  @return
          */
        AbstractTransActiveView(ITransActiveApp* pApp, std::string viewType)
            : m_pModel(NULL),
              m_pWinApp(pApp),
              m_hasPrivatePModel(false),
              m_viewID(""),
              m_isMainView(false),
              m_viewType(viewType)
        {
        }

        //AbstractTransActiveView()
        //    : m_pModel(NULL)
        //    , m_pWinApp(NULL)
        //    , m_hasPrivatePModel(false)
        //{
        //    //TODO:
        //    //m_pWinApp = dynamic_cast<ITransActiveApp*> (AfxGetApp());
        //    TA_ASSERT(m_pWinApp != NULL, "TransActiveApp is valid");
        //}

        ~AbstractTransActiveView()
        {
            if (m_hasPrivatePModel && m_pModel != NULL)
            {
                m_pModel->cleanup();
                delete m_pModel;
                m_pModel = NULL;
            }

            //TODO: how to handle public PModel
        };

    public:

        /** get pointer to ITransActiveApp
          *
          *
          *  @return ITransActiveApp *
          */
        ITransActiveApp* getTransActiveWinApp()
        {
            return m_pWinApp;
        }

        virtual TA_Rect getViewPosition()
        {
            TA_Rect rect;
            TA_SetRectEmpty(rect);
            /*HWND hwnd = getViewHandle();

            if (hwnd != NULL)
            {
                GetWindowRect(hwnd, &rect);
            }*/
            return rect;
        }

        virtual bool setVeiwPosition(const TA_Rect& rect)
        {
            return false;
        }

        virtual void setViewID(const char* viewID)
        {
            m_viewID = viewID;
        }

        virtual bool isMainView() {return m_isMainView;};

        virtual void setMainView(bool isMainView) {m_isMainView = isMainView;};

        virtual std::string getViewID()
        {
            return m_viewID.c_str();
        }

        virtual std::string getViewType()
        {
            return m_viewType.c_str();
        }

        virtual bool init() {return true;};
        virtual void showView(bool modal) {};
        virtual int doModal() { return 0; };
        virtual void autoResize() {};
        virtual void hideView() {};
        virtual bool closeView() {return false;};
        virtual void translateUI() {};
        virtual void doNotifyMessage(unsigned long messageID, TRA_WPARAM wParam, TRA_LPARAM lParam) {};
        virtual void postMessage(unsigned long messageID, TRA_WPARAM  wParam, TRA_LPARAM lParam) {};
        virtual void sendMessage(unsigned long messageID, TRA_WPARAM  wParam, TRA_LPARAM lParam) {};

    public:

        virtual void notifyUpdate(const char* updateType, TA_Base_Bus::EventArgs* args = NULL) {};
        virtual void requestUserReply(TA_Base_Bus::UIReplyEventArgsPtr uiEvent) {};

    protected:

        /** create and init presentation model
          *
          *  __NOTE__: no need to override
          *  @return void
          */
        void attachPModel()
        {
            m_hasPrivatePModel = hasPrivatePModel();

            if (!m_hasPrivatePModel)
            {
                if (m_pModel == NULL)
                {
                    const std::string pmodelName = getMyPModelName();
                    m_pModel = m_pWinApp->getApplicationController()->getPModel(pmodelName);
                }
            }
            else
            {
                TA_ASSERT(m_pModel != NULL, "Presentation model should be created in advance if it is not shared by multi views");

                //TODO: xinsong: need to alloc view resource during presentation model creation
                //m_pModel->allocViewResource();
            }

            TA_ASSERT(m_pModel != NULL, "PModel has been initialized successfully");

            //do initial registration for pmodel data update in this function
            //attach view if need receive dynamic update from pmodel
            setupPModel();
        }

        /** do special setting before attach to presentation model
          *    __NOTE__: do initial registration for pmodel data update in this function
          */
        virtual void setupPModel() = 0;

    private:

        virtual bool hasPrivatePModel()
        {
            const std::string pmodelName = getMyPModelName();

            //if the PModel is owned by the view, it should be created beforehand
            return (pmodelName.empty());
        }

        std::string m_viewID;
        std::string m_viewType;

    protected:

        ITransActivePModel* m_pModel;     ///< pointer to corresponding model object
        ITransActiveApp* m_pWinApp;  ///< pointer to ITransActiveApp
        bool m_hasPrivatePModel;        ///< has own private PModel or shared PModel
        bool m_isMainView;
    };

    /** Abstract FormView class under GenericGUI framework
      *
      */
    class AbstractTransActiveFormView: public AbstractTransActiveView
    {
    public:

        /** Constructor function
          *
          */
        AbstractTransActiveFormView(ITransActiveApp* pApp, std::string viewType)
            : AbstractTransActiveView(pApp, viewType)
        {
        }
        ~AbstractTransActiveFormView()
        {
            m_pWinApp->getViewManager()->removeView(this);
        }

    protected:

        /** update view after init view and presentation model
          *
          *
          *  @return void
          */
        virtual void updateView() {};

        /** do extra UI initialization during view creation
          *
          *
          *  @return void
          */
        virtual void initUIControl() {};

        /** synchronize presentation data between view layer and presentation model layer
          *
          *
          *  @return void
          */
        virtual void syncUIDataToPModel() {};

        /**
          * OnInitDialog
          *
          * __NOTE__: no need to override in derived class
          */
        virtual bool init()
        {
            attachPModel();

            TA_ASSERT(m_pModel != NULL, "Presentation Model should be initialized sucessfully");

            // CDialog::OnInitDialog();

            //  UINT idd = (LONG)(LONG_PTR)this->m_lpszTemplateName;
            //   TATranslateDialog( idd, this );

            LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to init UI Control");

            initUIControl();

            //behavior similar to onInitGenericGUICompleted
            {
                LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to update view");

                //CWaitCursor cur;
                //LockWindowUpdate();
                updateView();
                //UnlockWindowUpdate();
                //UpdateWindow();
            }

            LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to sync data to presentation model");
            syncUIDataToPModel();
            return TRUE;  // return TRUE  unless you set the focus to a control
        }
    };
}
