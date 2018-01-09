/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/ServerMonitor.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This will monitor the first server listed in the HostNames list. If all servers are not contactable
  * then it will alert the ProcessManager so each application can be told.
  *
  * This class is a singleton
  */

#include "StdAfx.h"
#include "Utilities.h"
#include "DataCache.h"
#include "Signals.h"
#include "SessionSignal.h"
#include "app/system_control/control_station/src/ControlStationConstants.h"
#include "app/system_control/control_station/src/ProcessManager.h"
#include "app/system_control/control_station/src/ServerMonitor.h"
#include "app/system_control/control_station/src/ConsecutiveNetworkFailureAlarmPolicy.h"
#include "core/synchronisation/src/ThreadGuard.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/TAAssert.h"
#include "core/sockets/src/InetAddress.h"
#include "core/sockets/src/ICMPMessageCodec.h"
#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
#include "core/data_access_interface/entity_access/src/ControlStation.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/DataException.h"
#include <boost/foreach.hpp>
#include <process.h>
#include <time.h>

using namespace TA_Base_Core;

ServerMonitor* ServerMonitor::s_instance = NULL;
TA_Base_Core::NonReEntrantThreadLockable ServerMonitor::m_singletonLock;
const unsigned int ServerMonitor::PING_RETRIES_PER_CYCLE = 2; // See comments in header file

ServerMonitor::ServerMonitor()
    : m_isTerminating(false),
      m_isContactable(true),
      m_socket("icmp", true),
      m_pingTimeoutSeconds(5),
      m_pingRetries(2)
{
    setupSignalConnections();
}

void ServerMonitor::setupSignalConnections()
{
    SessionSignal::logined.connect(boost::bind(&ServerMonitor::start, this));
    SessionSignal::logouted.connect(boost::bind(&ServerMonitor::terminateAndWait, this));
}

ServerMonitor::~ServerMonitor()
{
    if (m_socket.isOpen())
    {
        m_socket.close();
    }
}

void ServerMonitor::cleanUp()
{
    delete s_instance ;
    s_instance = NULL;
}

void ServerMonitor::run()
{
    FUNCTION_ENTRY("run");

    m_pid = _getpid();
    retrievePingParameters();

    try
    {
#if defined( WIN32 )
        //
        // It is necessary to initialise winsock before using any of its
        // features, so we will do that here.  Of course this is only
        // necessary under "brain dead" operating systems.
        //
        int lvError = WSAStartup(MAKEWORD(2, 2), & winsockData);

        if (0 != lvError)
        {
            TA_THROW(TA_Base_Core::TransactiveException("Couldn't initialise Windows sockets"));
        }

#endif // !WIN32

        // Open the socket
        if (!m_socket.open())
        {
            LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Could not open socket for pinging");
            return;
        }

        // Create the PingableItems
        std::vector<PingableItem*> serversToPing;
        createPingableItemsFromHostnames(serversToPing);

        // Start the pinging
        while (!isTerminating())
        {
            pollHosts(serversToPing);
            doPostPollUpdate(serversToPing);

            if (!isTerminating())
            {
                Thread::sleep(4000);
            }
        }

        // Clean up the pingable item pointers
        for (PingableItem* item : serversToPing)
        {
            delete item;
        }
    }
    catch (...)
    {
        // Catch all exceptions as there is nothing outside this method to catch them.
        LOG_EXCEPTION("Unknown", "run() of ServerMonitor threw an unknown exception");
    }

    // Close the socket
    m_socket.close();

    FUNCTION_EXIT;
}

void ServerMonitor::terminate()
{
    FUNCTION_ENTRY("terminate");
    setIsTerminating(true);
    FUNCTION_EXIT;
}

bool ServerMonitor::isTerminating()
{
    ThreadGuard guard(m_terminateLock);
    return m_isTerminating;
}

void ServerMonitor::setIsTerminating(bool isTerminating)
{
    ThreadGuard guard(m_terminateLock);
    m_isTerminating = isTerminating;
}

void ServerMonitor::pollHosts(std::vector<PingableItem*>& pingableItems)
{
    FUNCTION_ENTRY("pollPoints");

    const unsigned int NUM_ENTITIES = pingableItems.size();

    //how many times have we retried to ping
    unsigned int numRetries = 0;

    for (PingableItem* item : pingableItems)
    {
        //This member (when true) is used to mark this item as needing
        //no further processing. We do this if we get a ping back for it
        //or we get sick of trying
        item->setProcessed(false);
    }

    // This unprocessedItems keeps a count of how many items have not returned
    // a successful ping. This may be updated every cycle so that successfully
    // pinged items are not pinged again until pollHosts() is called again.
    unsigned int unprocessedItems = NUM_ENTITIES;

    // Each cycle needs to be processed in a for loop iteration. Previous implementation
    // had the cycle check done within the while(responsesReceived < expectedResponses)
    // loop and this gave incorrect results because a timeout would result in only one fail count
    // to be set for a pingable item, even if it failed for each ping sent per cycle. The fail
    // count needs to added for every ping that is sent out and it timeouts.
    for (int i = 0; i < PING_RETRIES_PER_CYCLE && !m_isTerminating; ++i)
    {
        time_t timeAllSent = 0;
        time(&timeAllSent);

        //we expect to get a response back from all items until we find out different
        unsigned int expectedResponses = unprocessedItems;

        //the index of the item you are currently dealing with
        unsigned int itemIndex = 0;

        //number of responses to ping we have received
        unsigned int responsesReceived = 0;

        //while any points have not either responded or failed to send...
        while ((responsesReceived < expectedResponses) && !m_isTerminating)
        {
            //API REF:
            // wait for io on only one socket.  One of read or write must be
            // true otherwise function will return immediately with false.
            // static bool waitForOneIO(SocketType *socket, bool read, bool write,
            //                   bool &canRead, bool &canWrite,
            //                   unsigned int timeMillis);
            //
            // wait in 200ms blocks so we can bail out on
            // m_tellThreadToStop in a reasonably responsive manner.
            //this call sets the canRead and canWrite
            bool canRead = false;
            bool canWrite = false;

            if (TA_Base_Core::SocketSet<TA_Base_Core::IDatagramSocket>::waitForOneIO(&m_socket, true, itemIndex < NUM_ENTITIES, canRead, canWrite, 200))
            {
                if (canWrite)
                {
                    // try to send at least one...
                    bool triedToSendOne = false;

                    while (!triedToSendOne)
                    {
                        if (itemIndex < pingableItems.size())
                        {
                            PingableItem* pItem = pingableItems[itemIndex];

                            if (!pItem->hasBeenProcessed())
                            {
                                //note that the address obj is modified by the send method
                                TA_Base_Core::InetAddress& ina = pItem->getInternetAddressRef();

                                //construct a datagram
                                TA_Base_Core::Datagram datagram;

                                //fill out packet structure so that it is ECHO, i.e. a "ping"
                                TA_Base_Core::ICMPMessageCodec::getInstance()->constructECHO(datagram, m_pid, itemIndex);

                                //if the sending of this datagram fails...
                                //note that the address obj is modified by the send method
                                if (!m_socket.send(ina, datagram))
                                {
                                    // This failure probably means that the host doesn't exist, but there's no
                                    // harm in trying again later.

                                    const std::string& name = ina.getHostName();
                                    LOG_DEBUG("Failed to send to ping for %s", name.c_str());
                                }

                                triedToSendOne = true;
                            } //end: if (!pPoint->hasBeenProcessed())

                            itemIndex++;

                            // Have sent out a ping for all items
                            if (itemIndex >= NUM_ENTITIES)
                            {
                                //take note of the time when the last ping is sent
                                // we now wait m_pingTimeoutSeconds seconds for the remainder
                                // to be read
                                time(&timeAllSent);
                                //We add a bit of slack because time() truncates... how true
                                timeAllSent++;
                                break;  // out of while (!triedToSendOne)
                            }
                        }
                    } //end while (!triedToSendOne)
                } //end if (canWrite)

                //if a ping response has arrived...
                if (canRead)
                {
                    //These variables are populated by the receive method
                    TA_Base_Core::InetAddress outa;
                    TA_Base_Core::Datagram datagram;

                    //if we can read the responce from the socket...
                    if (m_socket.receive(outa, datagram))
                    {
                        unsigned short id = 0, index = 0;

                        //if we can get the data out of the package...
                        if (TA_Base_Core::ICMPMessageCodec::getInstance()->decodeECHO(datagram, id, index))
                        {
                            //if this is a datagram we sent...
                            if (id == m_pid && index < NUM_ENTITIES)
                            {
                                PingableItem* pItem = pingableItems[index];

                                // if this point has not already been processed...
                                if (!pItem->hasBeenProcessed())
                                {
                                    //The success is reported here so it does not need
                                    //to be set in the final loop below
                                    pItem->getAlarmPolicy()->addResult(true);
                                    pItem->setProcessed(true);

                                    --unprocessedItems;

                                    LOG_DEBUG("Host %s has been succesfully pinged", pItem->getInternetAddressRef().getHostName().c_str());

                                    responsesReceived++;
                                }
                            }
                            else
                            {
                                // an old or someone else's ping
                                LOG_DEBUG("decodeECHO() got bad id=%u or bad seq=%u", id, index);
                            }
                        }
                        else
                        {
                            // this isn't necessarily bad. It could be someone else's packet.
                            LOG_DEBUG("response failed checksum.");
                        }
                    }
                    else
                    {
                        LOG_DEBUG("receive() failed");
                    }
                } //end : if(canRead)
            } //end: if socket is ready to read or write
            else //if socket is not ready to either read or write...
            {
                time_t now;
                time(&now);

                double current = now;
                double sentTime = timeAllSent;
                double maxTime = m_pingTimeoutSeconds;

                //if m_pingTimeoutSeconds has elapsed sence all were sent...
                if (current - sentTime >= maxTime)
                {
                    LOG_DEBUG("Timed out.");
                    break; //break out of the main while loop
                }
            }
        } //end while (responsesReceived < expectedResponses)

        // now set any not received as a failure...
        if (responsesReceived < expectedResponses)
        {
            std::vector<PingableItem*>::iterator iter = pingableItems.begin();

            for (; iter != pingableItems.end(); ++iter)
            {
                if (!(*iter)->hasBeenProcessed())
                {
                    (*iter)->getAlarmPolicy()->addResult(false);
                }
            }
        }
    } //end for

    FUNCTION_EXIT;
}

void ServerMonitor::doPostPollUpdate(std::vector<PingableItem*>& pingableItems)
{
    FUNCTION_ENTRY("doPostPollUpdate");

    if (m_isTerminating)
    {
        return;
    }

    bool newIsContactableState = false;

    for (PingableItem* item : pingableItems)
    {
        if (item->getAlarmPolicy()->pointIsPingable())
        {
            newIsContactableState = true;
            break;
        }
    }

    // Check if this is a change in state
    if (m_isContactable != newIsContactableState)
    {
        LOG_INFO("Server is %s.", newIsContactableState ? "CONTACTABLE" : "UNCONTACTABLE");
        m_isContactable = newIsContactableState;
        Signals::handleServerStateChangedSignal(m_isContactable);
    }

    FUNCTION_EXIT;
}

void ServerMonitor::retrievePingParameters()
{
    FUNCTION_ENTRY("retrievePingParameters");

    TA_Base_Core::ControlStationPtr entity = DataCache::getInstance().getControlStationEntity();
    m_pingTimeoutSeconds = entity->getPingTimeoutSeconds();
    m_pingRetries = entity->getPingRetries();

    FUNCTION_EXIT;
}

void ServerMonitor::createPingableItemsFromHostnames(std::vector<PingableItem*>& pingableItems)
{
    FUNCTION_ENTRY("createPingableItemsFromHostnames");

    pingableItems.clear();
    std::string hosts = TA_Base_Core::RunParams::getInstance().get(TA_Base_App::TA_ControlStation::HOST_NAMES.c_str());
    LOG_DEBUG("ServerMonitor::createPingableItemsFromHostnames - Hosts are: %s", hosts.c_str());
    std::vector<std::string> hostnames;
    boost::split(hostnames, hosts, boost::is_any_of(", :;|"), boost::token_compress_on);

    for (const std::string& hostname : hostnames)
    {
        PingableItem* item = new PingableItem(hostname);
        item->setMaxConsecutiveFails(m_pingRetries);
        pingableItems.push_back(item);
    }

    FUNCTION_EXIT;
}
