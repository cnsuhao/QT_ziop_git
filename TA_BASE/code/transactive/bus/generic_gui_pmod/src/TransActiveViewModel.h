#ifndef TRANSACTIVE_DIALOG_MODEL_H__
#define TRANSACTIVE_DIALOG_MODEL_H__

#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "bus/generic_gui_pmod/src/PModelDataType.h"
#include "core/exceptions/src/TransactiveException.h"

#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include "core/synchronisation/src/ThreadGuard.h"
#include "core/threads/src/IWorkItem.h"
#include <set>


namespace TA_Base_Bus
{
    /** Implementation for ITransActivePModel
      *    Presentation model class, as a middle man between view class and business objects, handles response to user events, delegates most of the request to business objects.
      *   The class has the following functions:
      *
      *   -    Monitor state change of business objects, notify view object to reflect these changes
      *   -    Process presentation logic which is independent of any specific user interface implementation
      *   -    Should be very thin, forwards most of request to backend business objects
      *   -    For the sake of portability, should only use simple data type as its member variable.
      *
      *   Every view should have its corresponding presentation model object. Indeed, complex UI control could have presentation model object, too.
      *   For view that need to control its display, its corresponding presentation model class should be derived from AbstractTransActivePModel;
      *   for normal view, it should be derived from AbstractPModel
      *
      */
    class AbstractPModel : public ITransActivePModel
    {
    public:
        /** Constructor function
          *
          *
          */
        AbstractPModel(IApplicationController* appController, const char * pmodelName);

        /** destructor function
          *
          */
        virtual ~AbstractPModel(void);

    // ---------------------BEGIN Interface ITransActiveViewModel virtual method -------------------------
    public:
        virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args=NULL) {};

        virtual void allocViewResource() {};
        virtual void cleanup();

        virtual void attachView(ITransActiveView * pView, const char * updateType = NULL)
        {
            m_viewObservers.registerObserver(pView, updateType);
        };

        virtual void detachView(ITransActiveView * pView, const char * updateType = NULL)
        {
            m_viewObservers.unregisterObserver(pView, updateType);
        }

        virtual std::string getName()
        {
            return m_pmodelName;
        }

		 ITransActiveApp * getWinApp()
		 {
			 return m_appController->getWinApp();
		 }

		 ITransActiveViewManager * getViewManager()
		 {
			 return m_appController->getWinApp()->getViewManager();
		 }

        // ---------------------END Interface ITransActiveViewModel virtual method -------------------------
		 void executeWorkAync(TA_Base_Core::IWorkItemPtr workItem);

    protected:
        /** Release view level resource which allocation by this presentation model
          *
          *
          *  @return void
          */
        void releaseViewResource() {};

        virtual void notifyViews(const char * updateType, TA_Base_Bus::EventArgs * args=NULL)
        {
            m_viewObservers.notifyObservers(updateType, args);
        }

    protected:

        IApplicationController * m_appController;  ///<  pointer to application controller

        std::string m_pmodelName; ///<  presentation model name

        AbstractStateChangeSubject<ITransActiveView> m_viewObservers;
    };
}

#endif
