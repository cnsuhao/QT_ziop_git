#ifndef GENERIC_GUI_COMMON_IStateChangeSubject_H_
#define GENERIC_GUI_COMMON_IStateChangeSubject_H_


#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include <string>
#include <map>
#include <set>

namespace TA_Base_Bus
{


    /** Interface for subject that send domain data change to IStateChangeObserver
     *	
     */

    template<class ObserverType, class EventArgsType>
    class IStateChangeSubject
    {
    public:
        virtual ~IStateChangeSubject() {};


        /** Register observer
         * 
         */
        virtual void registerObserver(ObserverType *, const char * updateType = NULL) = 0;
        
        /** Unregister observer 
         *
         */
        virtual void unregisterObserver(ObserverType *, const char * updateType = NULL) = 0;

        /** Notify observers when domain data changed
         * 
         */
        virtual void notifyObservers(const char * updateType, EventArgsType * args=NULL) = 0;

    protected:


    };
}
#endif