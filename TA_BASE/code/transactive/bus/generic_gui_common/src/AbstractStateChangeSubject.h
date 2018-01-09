#ifndef AbstractStateChangeSubject_H_
#define AbstractStateChangeSubject_H_


#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
 #include "bus/generic_gui_common/src/PModelDataType.h"
#include "bus/generic_gui_common/src/IStateChangeSubject.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"
#include "core/message/public_interfaces/ItaAuditing.h"
#include "core/utilities/src/DebugUtil.h"

#include "boost/shared_ptr.hpp"
#include <string>
#include <map>
#include <set>

namespace TA_Base_Bus
{

/** Simple Implementation for Interface IStateChangeSubject
     *	
     */
    template<class ObserverType>
    class AbstractStateChangeSubject : public IStateChangeSubject<ObserverType, TA_Base_Bus::EventArgs>
    {
    public:
        //friend TA_Base_Bus_Test::AbstractStateChangeSubjectTest;  ///< for unit test

        AbstractStateChangeSubject() 
        {
        }

        virtual ~AbstractStateChangeSubject() 
        {
        }

        void registerObserver(ObserverType* observer, const char * updateType = NULL)
        {
            TA_Base_Core::ThreadGuard guard(m_observerLock);
            if (NULL == updateType)
            {
                m_observersForAllUpdate.insert(observer);

                //remove unnecessary special update registration
                removeObserverFromSpecialUpdate(observer);
            }
            else
            {
                //has it been already registered for all update type
                if (m_observersForAllUpdate.find(observer) != m_observersForAllUpdate.end())
                {
                    return;
                }

                if (! exists(observer, updateType) )
                {
                    m_observersForOneUpdate.insert(std::make_pair(updateType, observer));
                }
            }
        }

        void unregisterObserver(ObserverType* observer, const char * updateType = NULL)
        {
            TA_Base_Core::ThreadGuard guard(m_observerLock);
            if (NULL == updateType)
            {
                m_observersForAllUpdate.erase(observer);
            }
            else
            {
                std::pair<UpdateMultiMap::iterator, UpdateMultiMap::iterator> keyRange = m_observersForOneUpdate.equal_range(updateType);

                UpdateMultiMap::iterator it;
                for (it = keyRange.first;  it != keyRange.second;  ++it)
                {
                    if (it->second == observer)
                    {
                       m_observersForOneUpdate.erase(it);   
                       break;
                    }
                }
            }
        }

        virtual void notifyObservers (const char *updateType, TA_Base_Bus::EventArgs *args=NULL)
        {
            TA_Base_Core::ThreadGuard guard(m_observerLock);

            {
                std::set<ObserverType*>::iterator it;

                // notify duty update
                for( it = m_observersForAllUpdate.begin(); it != m_observersForAllUpdate.end(); ++it )
                {       
                    (*it)->notifyUpdate(updateType, args);
                }
            }

            if(updateType != NULL)
            {
                UpdateMultiMap::iterator it;
                std::pair<UpdateMultiMap::iterator, UpdateMultiMap::iterator> keyRange = m_observersForOneUpdate.equal_range(updateType);
                for (it = keyRange.first;  it != keyRange.second;  ++it)
                {
                    ObserverType * observer = it->second;
                    TA_ASSERT(m_observersForAllUpdate.find(observer) == m_observersForAllUpdate.end(), "should not register all update type");

                    observer->notifyUpdate(updateType, args);
                }
            }
        }

    protected:
        /** remove observer from special update registration 
         *	
         */
        void removeObserverFromSpecialUpdate(ObserverType* observer)
        {
            UpdateMultiMap::iterator it;
            for (it = m_observersForOneUpdate.begin();  it != m_observersForOneUpdate.end(); )
            {
                if (it->second == observer)
                {
                    m_observersForOneUpdate.erase(it++);   
                }
                else
                {
                    it++;
                }
            }

        }

        /** exist the registration or not
         *	
         */
        bool exists(ObserverType* observer, const char * updateType)
        {
            if( updateType == NULL)
            {
                return m_observersForAllUpdate.find(observer) != m_observersForAllUpdate.end();  
            }

            UpdateMultiMap::iterator it;
            std::pair<UpdateMultiMap::iterator, UpdateMultiMap::iterator> keyRange = m_observersForOneUpdate.equal_range(updateType);

            for (it = keyRange.first;  it != keyRange.second;  ++it)
            {
                if (it->second == observer)
                {
                    return true;   
                }
            }

            return false;

        }


    protected:
        TA_Base_Core::NonReEntrantThreadLockable m_observerLock;  ///< observer lock
        
        typedef std::multimap<std::string, ObserverType*> UpdateMultiMap;  ///<  multi-map for observer update

        std::set<ObserverType*> m_observersForAllUpdate;  ///< observer collection for all update
        UpdateMultiMap m_observersForOneUpdate; ///< observer collection for special update


    };
}
#endif