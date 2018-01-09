#ifndef ITEM_REPOSITORY_H__
#define ITEM_REPOSITORY_H__

#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include "core/synchronisation/src/ThreadGuard.h"
#include <string>
#include <map>

namespace TA_Base_Bus
{

    /** Interface for Item repository.
      *    NameType is the type of key name, T is the item store in the repository
      *  It is not implemented as a factory. so you should create instance before add to the repository
      */
    template <class NameType, class T>
    class IItemRepository
    {
    public:

        /** add item to the repository. If duplicate item name exists, it will return false
          *
          *
          * @param itemName
          * @param item
          * @return bool
          */
        virtual bool addItem(const NameType & itemName, T * item) = 0;

        /** remove Item from the repository. It will delete the item
          *
          *
          * @param itemName
          * @return void
          */
        virtual T * removeItem(const NameType & itemName) = 0;

        /** get Item from the repository.  if not exist, return NULL
          *
          *
          * @param itemName
          * @return T *
          */
        virtual T * getItem(const NameType & itemName, bool createIfNew) = 0;

        /**
          * determine if exists same item.
          *
          * @param itemName
          * @return bool
          */
        virtual bool exists(const NameType & itemName) = 0;

    };

    /** Implementation for Item repository. Thread safe, used lock to synchronize internal map access
      *
      */
    template <class NameType, class T>
    class ItemRepository : public IItemRepository<NameType, T>
    {
    public:
        /// map type to store item
        typedef std::map<NameType, T*> ItemMap;

        virtual ~ItemRepository(void)
        {
            cleanup();
        }
    private:
        /** interface method, cleanup the repository. delete all the items hold by repository, the destructor function will call it
          *
          *
          * @return void
          */
        void cleanup()
        {
            TA_Base_Core::ThreadGuard guard(m_mapLock);

            ItemMap::iterator it;
            for(it=m_items.begin();it!=m_items.end(); it++)
            {
                T * pItem = (*it).second;
                if (pItem != NULL)
                {
                    delete pItem;
                }
            }
            m_items.clear();
        }

    public:
        T * getItem(const NameType & itemName, bool createIfNew)
        {
            TA_Base_Core::ThreadGuard guard(m_mapLock);
            if (m_items.find(itemName) != m_items.end())
            {
                return m_items[itemName];
            }

            T * pItem = NULL;
            if (createIfNew)
            {
                pItem = createItem(itemName);
                m_items[itemName] = pItem;
            }
            return  pItem;
        }

        bool exists(const NameType & itemName)
        {
            TA_Base_Core::ThreadGuard guard(m_mapLock);
            if (m_items.find(itemName) == m_items.end())
            {
                return false;
            }
            return true;
        }

        bool addItem(const NameType & itemName, T * pItem)
        {
            TA_Base_Core::ThreadGuard guard(m_mapLock);
            if (m_items.find(itemName) == m_items.end())
            {
                m_items[itemName] = pItem;
                return true;
            }
            return false;
        }

        T* removeItem(const NameType & itemName)
        {
            TA_Base_Core::ThreadGuard guard(m_mapLock);

            T * pItem = m_items[itemName];
            m_items.erase(itemName);
            return pItem;

        }

    private:
        ItemMap m_items;  ///< store all the items in the map

        TA_Base_Core::NonReEntrantThreadLockable m_mapLock;  ///<  lock for repository map

    };


}

#endif