#ifndef CORBA_OBJECT_REPOSITORY_H__
#define CORBA_OBJECT_REPOSITORY_H__


#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include <string>
#include <map>

namespace TA_Base_Bus
{
	template <class T>
	class CorbaObjectRepository
	{
	private:

		CorbaObjectRepository(void)
		{

		}

		virtual ~CorbaObjectRepository(void)
		{
            cleanup();
		}
	public:

        void cleanup()
        {
			TA_Base_Core::ThreadGuard guard(m_mapLock);

			ObjectMap::iterator it;
			for(it=m_objects.begin();it!=m_objects.end; it++)
			{
                T * pObject = *it;
                if (pObject != NULL)
                {
                    delete pObject;
                }
                *it = NULL;
			}

        }

		T * getObject(const std::string& objectName)
		{
			TA_Base_Core::ThreadGuard guard(m_mapLock);
			if (m_objects.find(objectName) != m_objects.end())
			{
				return m_objects[objectName];
			}
			return  NULL;
		}
		
		bool exists(const std::string& objectName)
		{
			TA_Base_Core::ThreadGuard guard(m_mapLock);
			if (m_objects.find(objectName) == m_objects.end())
			{
				return false;
			}
			T * pObject = m_objects[objectName];
			if (pObject != NULL)
			{
				return false;
			}
			return true;
		}

		bool addInstance(const std::string& objectName, T * pObject)
		{
			TA_Base_Core::ThreadGuard guard(m_mapLock);
			if (m_objects.find(objectName) == m_objects.end())
			{
				m_objects[objectName] = T;
				return true;
			}
			return false;
		}

		void removeInstance(const std::string& objectName)
		{
			TA_Base_Core::ThreadGuard guard(m_mapLock);

			T * pObject = m_objects[objectName];
			if (pObject != NULL)
			{
				delete pObject;
				m_objects[objectName] = NULL;
			}
			
		}

	private:
		typedef std::map<std::string, T*> ObjectMap;
		ObjectMap m_objects;

		// Static variables used to implement the singleton pattern.
		static CorbaObjectRepository * m_instance;
		static TA_Base_Core::NonReEntrantThreadLockable m_singletonLock;

		TA_Base_Core::NonReEntrantThreadLockable m_mapLock;

	};

	template<class T>
		TA_Base_Core::NonReEntrantThreadLockable CorbaObjectRepository<T>::m_singletonLock;
	
	template<class T>
		CorbaObjectRepository<T>* CorbaObjectRepository<T>::m_instance = NULL;



}
#endif