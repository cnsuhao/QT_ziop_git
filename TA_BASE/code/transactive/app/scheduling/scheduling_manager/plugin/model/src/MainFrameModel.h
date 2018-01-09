#ifndef MAIN_FRAME_MODEL_INCLUDE_H
#define MAIN_FRAME_MODEL_INCLUDE_H

#include "app/scheduling/scheduling_manager/plugin/model/src/Job.h"
#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"
#include "bus/generic_gui_entity/src/ISystemStateUpdateObserver.h"
#include "bus/generic_gui_entity/src/SystemControllerGroupUpdateSubscriber.h"

namespace TA_Base_Bus
{
	class RightsLibrary;
}

namespace TA_Base_App
{
    class MainFrameModel: public TA_Base_Bus::AbstractPModel,
		public TA_Base_Bus::ISystemStateUpdateObserver
    {
    public:
        MainFrameModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
        virtual ~MainFrameModel();


        virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args=NULL);

		
	   //TD16358++
       /**
        * notifySystemStateDegraded
        *
        * This is called when the system is in degraded mode.
        *
        */
        virtual void notifySystemStateDegraded();


       /**
        * notifySystemStateNormal
        *
        * This is called when the system has recovered from degraded mode 
        * (ie. system is running normally).
        *
        */
        virtual void notifySystemStateNormal();

		void subscribeGroupUpdate();

		int compareDateContent( Job* job1, Job* job2 );

		/**
		 * isRightsAgentContactable
		 *
		 * Attempts to check rights for RUN_APPLICATION on the Scheduling Manager
		 * to see if the Rights Agent is indeed contactable.
		 * 
		 * @return True if the Rights Agent was able to be contacted, false otherwise
		 */
		bool isRightsAgentContactable();

		void initRightLib();
    private:
        TA_Base_Bus::SystemControllerGroupUpdateSubscriber* m_groupUpdateSubscriber;

		// Make a local rights library to do Rights Agent communication testing
		TA_Base_Bus::RightsLibrary* m_rightsLibrary;
		unsigned long m_resourceKey;
    };
}

#endif //MAIN_FRAME_MODEL_INCLUDE_H