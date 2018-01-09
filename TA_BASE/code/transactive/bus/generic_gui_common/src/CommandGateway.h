#ifndef I_COMMAND_GATEWAY_H__
#define I_COMMAND_GATEWAY_H__

#include "bus/generic_gui_common/src/PModelDataType.h"
#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include <string>
#include <map>
#include <set>

namespace TA_Base_Bus
{

    /** Interface for Command Sender. ICommandSender derived class send command of specific types  
     *	ICommandReciever derived class will register and process theses type of command.
     *  ICommandGateway is a mediator to transfer command
     *  
     *  __NOTE__: the memory allocation and deallocation is not defined, it should be determined by implementation of ICommandGateway
     */
    class ICommandSender
    {
    public:
        virtual ~ICommandSender(void)
        {
        }
    public:
        /**
         * send command of specific type. It also send the memory pointer which used to interpret the command content 
         *
         * @param cmdName: command name
         * @param args: pointer to the memory which need to access
         * @return void
         */
        virtual void sendCommand(const char * cmdName, EventArgs * args) = 0;
    };

    /** Interface for Command Receiver. 
     *	Register interest for specific command on ICommandGeteway, and process the command which send by ICommandSender
     *
     *  __NOTE__: the memory allocation and deallocation is not defined, it should be determined by implementation of ICommandGateway
     */
    class ICommandReceiver
    {
    public:
        virtual ~ICommandReceiver(void)
        {
        }
    public:
        /** Receive command and process specific command
         *	
         */
        virtual void notifyUpdate(const char * cmdName, EventArgs * args) = 0;
    };

    /** Interface for Command Gateway. As a mediator, it transfer commands from ICommandSender derived class to ICommandReceiver derived class
     *	
     *  __NOTE__: the memory allocation and deallocation is not defined, it should be determined by implementation of ICommandGateway
     */
    class ICommandGateway
    {
    public:
        virtual ~ICommandGateway(void)
        {
        }
    public:
        
        /** Register command receiver for specific command.
         * 
         *
         * @param cmdName
         * @param observer
         * @return void
         */
        virtual void registerObserver(const char * cmdName, ICommandReceiver* observer) = 0;

        /** Unregister command receiver for specific command it had registered
         * 
         *
         * @param cmdName
         * @param observer
         * @return void
         */
        virtual void unregisterObserver(const char * cmdName, ICommandReceiver* observer) = 0; 

        /** Unregister command receiver for all commands it had registered
         * 
         *
         * @param observer
         * @return void
         */
        virtual void unregisterObserver(ICommandReceiver* observer) = 0;

        /** transfer command from ICommandSender Class to interested ICommandReciver
         * 
         *
         * @param cmdName
         * @param args
         * @return void
         */
        virtual void transferCommand(const char * cmdName, EventArgs * args) = 0;
    };


}

#endif