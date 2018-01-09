/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File:  $
  * @author:  
  * @version: $Revision:  $
  *
  * Last modification: $DateTime:  $
  * Last modified by:  $Author:  $
  * 
  *
  */

#if defined( WIN32 )
#pragma warning ( disable : 4503 ) 
#endif // defined( WIN32 )

#include <sstream>

#include "app/data_management/archive_manager/pmodel/src/BurnMediaPModel.h"
#include "app/data_management/archive_manager/pmodel/src/BasicHelper.h"
#include "app/data_management/archive_manager/pmodel/src/GuiConstants.h"
#include "app/data_management/archive_manager/pmodel/src/TransactiveUserMsgEventArgs.h"
#include "app/data_management/archive_manager/pmodel/src/GenericEventArgs.h"
#include "app/data_management/archive_manager/pmodel/src/PostMessageEventArgs.h"
#include "bus/security/access_control/actions/src/AccessControlledActions.h"
#include "bus/generic_gui_view/src/TransactiveMessage.h"
#include "cots/Nero/inc/NeroAPIglue.h"
#include "core/exceptions/src/ArchiveException.h"
#include "core/message/types/ArchiveAudit_MessageTypes.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/DebugUtil.h"


namespace TA_Base_App
{
    
	BurnMediaPModel::BurnMediaPModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		: TA_Base_Bus::AbstractPModel(appController, pmodelName),
		   m_files(NULL),m_logString("")
	{

	}
        
    BurnMediaPModel::~BurnMediaPModel()
    {
        
    }
        
	void BurnMediaPModel::setDetails(const std::string& volumeLabel, const std::string& cdLabel, const WriteArchiveHelper::FileDetailsVector* files,bool bCdBurn)
	{
		m_volumeLabel =  volumeLabel;
		m_cdLabel = cdLabel;
		m_files =  files ;
		m_bCdBurn = bCdBurn;
	}

    void BurnMediaPModel::run()
    {
        // Make sure no exceptions escape from the run() method.

        try
        {
            // Set the state to PROCESSING here. 
            // The state will be set to IDLE in the OnCloseDlg method.

            m_stateManager.setState( StateManager::PROCESSING );

            //  Display an hour-glass cursor.

            HCURSOR hOldCursor = SetCursor( LoadCursor( NULL, IDC_APPSTARTING ) );

            try
            {
                // Add all the files to be burnt to m_neroIsoItemsList.

                for( WriteArchiveHelper::FileDetailsVector::const_iterator it = m_files->begin(); it != m_files->end(); ++it )
                {
					// TD16705 changed ?YYYY_MM_DD*.dmp to ?DDMMYY*.dmp
					//std::string strFile = formatStringHelper( (*it).name );

					addFile( (*it).nameWithPath, (*it).name );
                }
              
                // Check that we do actually have files to write.  No point in setting 
                // up the hardware etc if there are no files to burn.  
        
				if( 0 == m_neroIsoItemsList.size())
                {  
					std::string reason = IGuiObserver::getInstance()->getStringResource(UE050090_REASON_NO_FILES);
					std::string error = IGuiObserver::getInstance()->getStringResourceFormated(UE050090_WRITE_FAILED,m_volumeLabel,reason);
					TransactiveUserMsgEventArgs args(IDS_UE_050091,m_volumeLabel,reason);
					notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),&args); 
                   
                    TA_ArchiveManager::sendAuditMessageIgnoreErrors( TA_Base_Core::ArchiveAudit::CopyArchiveFailed,error );

					PostMessageEventArgs postArgs(WM_CLOSE_DLG,IDABORT);
					notifyViews(POST_MESSAGE_INVOKE.c_str(),&postArgs);		
                    return;
                }
       
                // Prepare for the write, includes setting up the hardware and the DVD configuration.
                // The associated call to neroApiFree() is in OnDestroy(). 

                neroApiInit();

                // If the user has cancelled - don't go any further.

                if ( StateManager::STOPPING == m_stateManager.getState() )
                {
   					PostMessageEventArgs postArgs(WM_CLOSE_DLG,IDCANCEL);
					notifyViews(POST_MESSAGE_INVOKE.c_str(),&postArgs);	
                    return;
                }

                // Write away...

                doBurn();

                // If the user has cancelled - don't go any further.

                if ( StateManager::STOPPING == m_stateManager.getState() )
                {
					PostMessageEventArgs postArgs(WM_CLOSE_DLG,IDCANCEL);
					notifyViews(POST_MESSAGE_INVOKE.c_str(),&postArgs);
                    return;
                }
            
                // Write successful.

				TransactiveUserMsgEventArgs args(IDS_UI_050046,m_volumeLabel);
				args.setCaption(CaptionWriteComplete);
				notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),&args); 
				
				PostMessageEventArgs postArgs(WM_CLOSE_DLG,IDOK);
				notifyViews(POST_MESSAGE_INVOKE.c_str(),&postArgs);
                return;
            }
            catch ( TA_Base_Core::ArchiveException& e )
            {
                LOG_EXCEPTION_CATCH( SourceInfo, "ArchiveException", e.what() );
                TA_ArchiveManager::sendAuditMessageIgnoreErrors( TA_Base_Core::ArchiveAudit::CopyArchiveFailed, e.what() ); 
                
				TransactiveUserMsgEventArgs args(IDS_UE_050130,e.what());
				notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),&args); 
                
				PostMessageEventArgs postArgs(WM_CLOSE_DLG,IDABORT);
				notifyViews(POST_MESSAGE_INVOKE.c_str(),&postArgs);
                return;
            }
            catch ( TA_Base_Core::TransactiveException& e )
            {
                LOG_EXCEPTION_CATCH( SourceInfo, "TransactiveException", e.what() );
				std::string error = IGuiObserver::getInstance()->getStringResourceFormated(UE050090_WRITE_FAILED,m_volumeLabel,e.what() );
                TA_ArchiveManager::sendAuditMessageIgnoreErrors( TA_Base_Core::ArchiveAudit::CopyArchiveFailed, std::string( error ) ); 
                
				TransactiveUserMsgEventArgs args(IDS_UE_050091,m_volumeLabel,e.what());
				notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),&args); 
                
				PostMessageEventArgs postArgs(WM_CLOSE_DLG,IDABORT);
				notifyViews(POST_MESSAGE_INVOKE.c_str(),&postArgs);
                return;
            }
            catch ( ... )
            {
				std::string reason = IGuiObserver::getInstance()->getStringResource(UE050090_REASON_UNKNOWN_ERROR);
				std::string error = IGuiObserver::getInstance()->getStringResourceFormated(UE050090_WRITE_FAILED,m_volumeLabel,reason );
				LOG_EXCEPTION_CATCH( SourceInfo, "Unknown Exception", error );
                
                TA_ArchiveManager::sendAuditMessageIgnoreErrors( TA_Base_Core::ArchiveAudit::CopyArchiveFailed, std::string( error ) );         
				
				TransactiveUserMsgEventArgs args(IDS_UE_050091,m_volumeLabel,reason);
				notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),&args); 
                
				PostMessageEventArgs postArgs(WM_CLOSE_DLG,IDABORT);
				notifyViews(POST_MESSAGE_INVOKE.c_str(),&postArgs);
                return;
            }

            // If the user has cancelled - deal with it.

            if ( StateManager::STOPPING == m_stateManager.getState() )
            {
				PostMessageEventArgs postArgs(WM_CLOSE_DLG,IDCANCEL);
				notifyViews(POST_MESSAGE_INVOKE.c_str(),&postArgs);
                return;
            }
        }
        catch( TA_Base_Core::TransactiveException& e )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "TransactiveException", e.what() ); 
        }
        catch( ... )
        {
            LOG_EXCEPTION_CATCH( SourceInfo, "Unknown Exception", "Unknown exception caught in BurnMediaPModel::run()" ); 
        }
    }

    
    void BurnMediaPModel::terminate()
    {
		// The TransitionManager transition delay time
		unsigned int transitionDelaytimeSecs = 1000;

        if ( TA_Base_Core::Thread::THREAD_STATE_RUNNING == getCurrentState() )
        {
            // If we are in the middle of writing we have to wait for an appropriate
            // time before we terminate.

            if ( StateManager::PROCESSING == m_stateManager.getState() )
            {
				TransactiveUserMsgEventArgs args(IDS_UI_050055);
				notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),&args); 

                m_stateManager.setState( StateManager::STOPPING );
            }
            else if ( StateManager::STOPPING == m_stateManager.getState() )
            {
                // We're still waiting for the current day to finish restoring.
				TransactiveUserMsgEventArgs args(IDS_UI_050055);
				notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),&args); 
            }
        }
        
        // Wait until it has completed.
        // We could put a count in here so we don't get stuck, but how long should we count for?

        while( StateManager::IDLE != m_stateManager.getState() )
        {
            sleep( transitionDelaytimeSecs );  // 1 sec
        }
    }

	//*********************************** NERO

    void BurnMediaPModel::neroApiInit()
    {	
        m_logString = "Opening NeroAPI.DLL";
		notifyViews(APPEND_STRING_LOG_DISPLAY.c_str());//TODO

		LOGBEFORECOTS("NeroAPIGlueConnect" );

		
        if ( !NeroAPIGlueConnect(NULL) )
        {
			std::string error = IGuiObserver::getInstance()->getStringResourceFormated(UE050090_WRITE_FAILED,m_volumeLabel,UE050090_REASON_DLL_OPEN_FAILED );
			TA_THROW( TA_Base_Core::ArchiveException( error ) );
        }
        LOGAFTERCOTS("NeroAPIGlueConnect");
		m_logString = "Retrieving version information.";
		notifyViews(APPEND_STRING_LOG_DISPLAY.c_str());
        WORD majhi, majlo, minhi, minlo;

		LOGBEFORECOTS("NeroGetAPIVersionEx" );		
        NeroGetAPIVersionEx(&majhi, &majlo, &minhi, &minlo, NULL);
		LOGAFTERCOTS("NeroGetAPIVersionEx");

        std::stringstream strVersion;
        strVersion << "Nero API version " << majhi << "." << majlo << "." << minhi << "." << minlo;
		m_logString = strVersion.str() ;
		notifyViews(APPEND_STRING_LOG_DISPLAY.c_str());
              
		m_logString = "Filling NERO_SETTINGS structure";
		notifyViews(APPEND_STRING_LOG_DISPLAY.c_str());
        memset(&m_neroSettings, 0, sizeof(m_neroSettings));
        m_neroSettings.nstIdle.ncCallbackFunction = neroIdleCallback;
        m_neroSettings.nstIdle.ncUserData = this;
        m_neroSettings.nstUserDialog.ncCallbackFunction = neroUserDialog;
        m_neroSettings.nstUserDialog.ncUserData = this;
        m_neroSettings.nstNeroFilesPath = strdup( "NeroFiles");
        m_neroSettings.nstVendor = strdup( "ahead");
        m_neroSettings.nstSoftware = strdup( "Nero - Burning Rom");
        m_neroSettings.nstLanguageFile = strdup( "Nero.txt");
        
        memset(&m_neroProgress , 0, sizeof(m_neroProgress ));
        m_neroProgress.npAbortedCallback = neroAbortedCallback;
        m_neroProgress.npAddLogLineCallback = neroAddLogLine;
        m_neroProgress.npDisableAbortCallback = NULL;
        m_neroProgress.npProgressCallback = neroProgressCallback;
        m_neroProgress.npSetPhaseCallback = neroSetPhaseCallback;
        m_neroProgress.npSetMajorPhaseCallback=neroSetMajorPhaseCallback; // NULL;
        m_neroProgress.npSubTaskProgressCallback=NULL;
        m_neroProgress.npUserData = this;
        m_neroDeviceInfos = NULL;

        LOGBEFORECOTS("NeroInit" );		
        NEROAPI_INIT_ERROR initErr = NeroInit (&m_neroSettings, NULL);
		LOGAFTERCOTS("NeroInit");

        if (NEROAPI_INIT_OK != initErr)
        {
			std::string reason;
            switch (initErr)
            {
            case NEROAPI_INIT_INVALID_ARGS:
                reason = IGuiObserver::getInstance()->getStringResource( UE050090_REASON_INVALID_ARGS );
                break;
            case NEROAPI_INIT_INVALID_SERIAL_NUM:
                reason = IGuiObserver::getInstance()->getStringResource( UE050090_REASON_INVALID_SER_NO );
                break;
            case NEROAPI_INIT_DEMOVERSION_EXPIRED:
                reason = IGuiObserver::getInstance()->getStringResource( UE050090_REASON_EXPIRED );
                break;
            case NEROAPI_INIT_ALREADY_INITIALISED:
                reason = IGuiObserver::getInstance()->getStringResource( UE050090_REASON_RUNNING );
                break;
            case NEROAPI_INIT_CANNOT_LOCK:
                reason = IGuiObserver::getInstance()->getStringResource( UE050090_REASON_UNLOCKABLE );
                break;
            default:
                reason = IGuiObserver::getInstance()->getStringResourceFormated( UE050090_REASON_UNKNOWN, initErr );
                break;
            }
			std::string error = IGuiObserver::getInstance()->getStringResourceFormated(UE050090_WRITE_FAILED, m_volumeLabel, reason);
            TA_THROW( TA_Base_Core::ArchiveException( error ) );
        }
        
		m_logString = "Initialization of the NeroAPI successful.";
		notifyViews(APPEND_STRING_LOG_DISPLAY.c_str());
        
		// Changed media type to MEDIA_DVD_ANY
		//
		LOGBEFORECOTS("NeroGetAvailableDrivesEx" );		
		if(m_bCdBurn)
		{
			m_neroDeviceInfos = NeroGetAvailableDrivesEx (MEDIA_CD, NULL);
		}
		else
		{
			m_neroDeviceInfos = NeroGetAvailableDrivesEx (MEDIA_DVD_ANY, NULL);
		}
		LOGAFTERCOTS("NeroGetAvailableDrivesEx");

		std::string dvdStr = "DVD-RW";
		if(m_bCdBurn)
		{
			dvdStr = "CD-RW";
		}

        if (!m_neroDeviceInfos || (0 >= m_neroDeviceInfos->nsdisNumDevInfos) )
        {
			std::string reason;			
            reason = IGuiObserver::getInstance()->getStringResourceFormated( UE050090_REASON_NO_DRIVES, dvdStr );
			std::string error = IGuiObserver::getInstance()->getStringResourceFormated(UE050090_WRITE_FAILED, m_volumeLabel, reason);
            TA_THROW( TA_Base_Core::ArchiveException( error ) );
        }
        
		m_logString = "Found the following devices:";
		notifyViews(APPEND_STRING_LOG_DISPLAY.c_str());

        std::stringstream stemp;
        std::string chosenDrive;
        NERO_SCSI_DEVICE_INFO* pNsdiWritable = 0;
        for (DWORD dDeviceCounter = 0; dDeviceCounter < m_neroDeviceInfos->nsdisNumDevInfos; dDeviceCounter++)
        {
            NERO_SCSI_DEVICE_INFO nsdi = m_neroDeviceInfos->nsdisDevInfos[dDeviceCounter];
            stemp.str( "" );
            stemp << nsdi.nsdiDriveLetter << ": " << nsdi.nsdiDeviceName << ", " << nsdi.nsdiHostAdapterName;
            stemp << " on " << nsdi.nsdiHostAdapterNo << ":" << nsdi.nsdiDeviceID << ", type ";
            stemp << nsdi.nsdiCapabilities << "(0x%X)";
			m_logString = stemp.str();
			notifyViews(APPEND_STRING_LOG_DISPLAY.c_str());
            if ( (NEA_SCSI_DEVTYPE_WORM == nsdi.nsdiDevType) &&
                 ( (int) nsdi.nsdiHostAdapterNo >= 0) )
            {
                //  Use this DVD-RW drive
                //  Note: the Nero ImageDrive (virtual DVD-RW drive) has adapterNo == -1
                pNsdiWritable = &m_neroDeviceInfos->nsdisDevInfos[dDeviceCounter];
                chosenDrive = nsdi.nsdiDriveLetter;
                //  Could 'break' here if we didn't want to list the rest of the devices
            }
        }

        if ( 0 == pNsdiWritable )
        {		
			std::string reason = IGuiObserver::getInstance()->getStringResourceFormated( UE050090_REASON_NO_DRIVES, dvdStr );
			std::string error = IGuiObserver::getInstance()->getStringResourceFormated(UE050090_WRITE_FAILED, m_volumeLabel, reason);
            TA_THROW( TA_Base_Core::ArchiveException( error ) );
        }

        // Open a connection to the DVD writing device.
		LOGBEFORECOTS("NeroOpenDevice" );
		m_neroDeviceHandle = NeroOpenDevice( pNsdiWritable);
		LOGAFTERCOTS("NeroOpenDevice");
        if ( 0 == m_neroDeviceHandle )
        {
			std::string reason = IGuiObserver::getInstance()->getStringResourceFormated( UE050090_REASON_OPEN_FAILED, pNsdiWritable->nsdiDeviceName );
			std::string error = IGuiObserver::getInstance()->getStringResourceFormated(UE050090_WRITE_FAILED, m_volumeLabel, reason);
            TA_THROW( TA_Base_Core::ArchiveException( error ) );
        }

		m_logString = "Drive " + chosenDrive + ": selected for archiving.";
		notifyViews(APPEND_STRING_LOG_DISPLAY.c_str());
    }


    void BurnMediaPModel::neroApiFree()
    {
        if (m_neroDeviceInfos)
        {
			LOGBEFORECOTS("NeroFreeMem" );			
			NeroFreeMem(m_neroDeviceInfos);
			LOGAFTERCOTS("NeroFreeMem");
        }
		LOGBEFORECOTS("NeroClearErrors" );		
        NeroClearErrors();
		LOGAFTERCOTS("NeroClearErrors");

		LOGBEFORECOTS("NeroDone" );		
        NeroDone();
        LOGAFTERCOTS("NeroDone");

		LOGBEFORECOTS("NeroAPIGlueDone" );		
        NeroAPIGlueDone();
		LOGAFTERCOTS("NeroAPIGlueDone");
    }


    void BurnMediaPModel::doBurn()
    {
		if(m_bCdBurn)
			m_logString = "Preparing to write CD...";
		else
			m_logString = "Preparing to write DVD...";
		notifyViews(APPEND_STRING_LOG_DISPLAY.c_str());
		if (m_neroIsoItemsList.empty() )
        {
			std::string error = IGuiObserver::getInstance()->getStringResourceFormated(UE050090_WRITE_FAILED, m_volumeLabel, UE050090_REASON_NO_FILES);
			TA_THROW( TA_Base_Core::ArchiveException( error ) );
        }

        //  Build ISO/UDF item tree

        NERO_WRITE_CD neroWriteCD;
        ZeroMemory( &neroWriteCD, sizeof( NERO_WRITE_CD));
		if(m_bCdBurn)
			 neroWriteCD.nwcdMediaType = MEDIA_CD;
		else
			neroWriteCD.nwcdMediaType = MEDIA_DVD_ANY;

		//"070412 to 070413" 
		LOGBEFORECOTS("NeroCreateIsoTrackEx" );		
		neroWriteCD.nwcdIsoTrack = NeroCreateIsoTrackEx( (NERO_ISO_ITEM*)m_neroIsoItemsList.front(), //todo check 
            m_volumeLabel.c_str(), NCITEF_CREATE_ISO_FS|NCITEF_USE_JOLIET );
		LOGAFTERCOTS("NeroCreateIsoTrackEx");
        
//		neroWriteCD.nwcdIsoTrack = NeroCreateIsoTrackEx( (NERO_ISO_ITEM*)m_neroIsoItemsList.GetHead(), 
//            m_volumeLabel.c_str(), NCITEF_CREATE_UDF_FS );
//

        neroWriteCD.nwcdArtist = ARTIST.c_str();

        // KT 10 Mar 04: PW #3136 Added reformatted volume label for CD (CDLabel).

        neroWriteCD.nwcdTitle = m_cdLabel.c_str();

        // Burn the DVD
        // NBF_SIMULATE = Simulate writing before actually doing it.  This reduces the chances of errors that waste DVD's.
        // NBF_WRITE = Actually do the write after simulating.
        // NBF_BUF_UNDERRUN_PROT = Underrun protection enables a safer burn mode.
        // NBF_DETECT_NON_EMPTY_CDRW = Detects a non empty CDRW and calls neroUserDialog with DLG_NON_EMPTY_CDRW.
        // NBF_DAO = Disk At Once
		LOGBEFORECOTS("NeroBurn" );		
        int iRes = NeroBurn( m_neroDeviceHandle, NERO_ISO_AUDIO_CD, &neroWriteCD, 
            NBF_SIMULATE | NBF_WRITE | NBF_BUF_UNDERRUN_PROT | NBF_DETECT_NON_EMPTY_CDRW | NBF_DAO, 
            0, &m_neroProgress);
		LOGBEFORECOTS("NeroBurn" );
        // Clean up
		LOGBEFORECOTS("NeroFreeIsoTrack" );		
        NeroFreeIsoTrack( neroWriteCD.nwcdIsoTrack );
		LOGAFTERCOTS("NeroFreeIsoTrack");

		LOGBEFORECOTS("NeroCloseDevice" );		
        NeroCloseDevice( m_neroDeviceHandle);
		LOGAFTERCOTS("NeroCloseDevice");

        // If the user has cancelled don't tell them what
        // happened during the write.

        if ( StateManager::STOPPING == m_stateManager.getState() )
        {
			notifyViews(SHOW_CLOSE_BUTTON_BURNMEDIA_DLG.c_str());
            return;
        }
        
        // Don't let the user cancel from here on.  Otherwise they
        // might not get the message about labelling the DVD.
		notifyViews(DISABLECANCELBTN_BURNDLG.c_str());

        // Report what happened
        
        //\\appendStringToLogDisplay( NeroGetErrorLog() );

        // Tell the user how the burn process was finished

		std::string reason( "" );
        switch(iRes)
        {
        case NEROAPI_BURN_OK:
            reason = "Write successful";
            break;
        case NEROAPI_BURN_UNKNOWN_CD_FORMAT:
			reason = IGuiObserver::getInstance()->getStringResource(UE050090_REASON_UNKNOWN_CD_FORMAT);
            break;
        case NEROAPI_BURN_INVALID_DRIVE:
			reason = IGuiObserver::getInstance()->getStringResource(UE050090_REASON_INVALID_DRIVE);
            break;
        case NEROAPI_BURN_FAILED:
            reason = IGuiObserver::getInstance()->getStringResource( UE050090_REASON_WRITE_FAILED );
            break;
        case NEROAPI_BURN_FUNCTION_NOT_ALLOWED:
           reason = IGuiObserver::getInstance()->getStringResource( UE050090_REASON_FN_NOT_ALLOWED );
            break;
        case NEROAPI_BURN_DRIVE_NOT_ALLOWED:
            reason = IGuiObserver::getInstance()->getStringResource( UE050090_REASON_DRIVES_NOT_ALLOWED );
            break;
        default:
            reason = IGuiObserver::getInstance()->getStringResource( UE050090_REASON_UNKNOWN_ERROR );
            break;
        }
  		m_logString = reason ;
		notifyViews(APPEND_STRING_LOG_DISPLAY.c_str());

		bool success = false;
		if(NEROAPI_BURN_OK == iRes)
			success = true;
		GenericEventArgs args;
		args.Param1 = &(success);
		notifyViews(BURN_FINISHED_STATUS.c_str(),&args);
		if(!success)
		{
			std::string error = IGuiObserver::getInstance()->getStringResourceFormated(UE050090_WRITE_FAILED,m_volumeLabel,reason);
            TA_THROW( TA_Base_Core::ArchiveException( error ) );
		}
    }


    void NERO_CALLBACK_ATTR BurnMediaPModel::neroAddLogLine(void *pUserData, NERO_TEXT_TYPE type, const char *text)
    {
        TA_Base_App::BurnMediaPModel* pModel = static_cast< TA_Base_App::BurnMediaPModel* >( pUserData );
        if ( pModel == 0 )
        {
            LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, 
                "Failed to cast void* to BurnMediaPModel*. Can't process Nero add log line callback." );
        }
        else
        {
            std::string log( "Log: " + std::string( text ) );
			pModel->m_logString = log ;
			pModel->notifyViews(APPEND_STRING_LOG_DISPLAY.c_str());
        }
    }


    void NERO_CALLBACK_ATTR BurnMediaPModel::neroSetPhaseCallback(void *pUserData, const char *text)
    {
        TA_Base_App::BurnMediaPModel* pModel = static_cast< TA_Base_App::BurnMediaPModel* >( pUserData );
        if ( pModel == 0 )
        {
            LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, 
                "Failed to cast void* to BurnMediaPModel*. Can't process Nero set phase callback." );
            return;
        }
        else
        {
			std::string textStr = text;
			GenericEventArgs args;
			args.Param1 = &textStr;
			pModel->notifyViews(SET_STAGE_STR.c_str(),&args);
        }
    }


    void NERO_CALLBACK_ATTR BurnMediaPModel::neroSetMajorPhaseCallback(void *pUserData, NERO_MAJOR_PHASE phase,void *reserved)
    {
        TA_Base_App::BurnMediaPModel* pModel = static_cast< TA_Base_App::BurnMediaPModel* >( pUserData );
        if ( pModel == 0 )
        {
            LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, 
                "Failed to cast void* to BurnMediaPModel*. Can't process Nero set major phase callback." );
            return;
        }
        else
        {
            std::string log( "Major phase: ");
            switch( phase )
            {
                case NERO_PHASE_UNSPECIFIED:
                    log += "Unspecified";
                    break;
                case NERO_PHASE_START_CACHE:
                    log += "Start cache";
                    break;
                case NERO_PHASE_DONE_CACHE:
                    log += "Done cache";
                    break;
                case NERO_PHASE_START_TEST:
                    log += "Start test";
                    break;
                case NERO_PHASE_DONE_TEST:
                    log += "Done test";
                    break;
                case NERO_PHASE_START_SIMULATE:
                    log += "Start simulate";
                    break;
                case NERO_PHASE_DONE_SIMULATE:
                    log += "Done simulate";
                    break;
                case NERO_PHASE_START_WRITE:
                    log += "Start write";
                    break;
                case NERO_PHASE_DONE_WRITE:
                    log += "Done write";
                    break;
                case NERO_PHASE_START_SIMULATE_NOSPD:
                    log += "Start simulate nospd";
                    break;
                case NERO_PHASE_DONE_SIMULATE_NOSPD:
                    log += "Done simulate nospd";
                    break;
                case NERO_PHASE_START_WRITE_NOSPD:
                    log += "Start write nospd";
                    break;
                case NERO_PHASE_DONE_WRITE_NOSPD:
                    log += "Done write nospd";
                    break;
                case NERO_PHASE_ENCODE_VIDEO:
                    log += "Encode video";
                    break;
                case NERO_PHASE_SEAMLESSLINK_ACTIVATED:
                    log += "Seamless link activated";
                    break;
                case NERO_PHASE_BUP_ACTIVATED:
                    log += "BUP activated";
                    break;
                case NERO_PHASE_CONTINUE_FORMATTING:
                    log += "Continue formatting";
                    break;
                case NERO_PHASE_FORMATTING_SUCCESSFUL:
                    log += "Formatting successful";
                    break;
                case NERO_PHASE_DVDVIDEO_DETECTED:
                    log += "DVD video detected";
                    break;
                case NERO_PHASE_DVDVIDEO_REALLOC_STARTED:
                    log += "DVD video reallocation started";
                    break;
                case NERO_PHASE_DVDVIDEO_REALLOC_COMPLETED:
                    log += "DVD video reallocation completed";
                    break;
                case NERO_PHASE_DVDVIDEO_REALLOC_NOTNEEDED:
                    log += "DVD video reallocation not needed";
                    break;
                case NERO_PHASE_DVDVIDEO_REALLOC_FAILED:
                    log += "DVD video reallocation failed";
                    break;
                default:
                    log += "Unknown";
                    break;
            }
			pModel->m_logString = log ;
			pModel->notifyViews(APPEND_STRING_LOG_DISPLAY.c_str());
        }
    }


    NeroUserDlgInOut NERO_CALLBACK_ATTR BurnMediaPModel::neroUserDialog(void *pUserData, NeroUserDlgInOut type, void *data)
    {
        switch (type)
        {
            case DLG_AUTO_INSERT:
            case DLG_DISCONNECT:
            case DLG_SETTINGS_RESTART:

                return DLG_RETURN_CONTINUE;
                break;

            case DLG_DISCONNECT_RESTART:

                return DLG_RETURN_ON_RESTART;
                break;

            case DLG_OVERBURN:

                return DLG_RETURN_TRUE;
                break;

           
            case DLG_FILESEL_IMAGE:
                {                   
                    TA_Base_App::BurnMediaPModel* pModel = static_cast< TA_Base_App::BurnMediaPModel* >( pUserData );
                    if ( pModel == 0 )
                    {
                        LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, 
                            "Failed to cast void* to BurnMediaPModel*. Can't process Nero user dialog callback." );
                    }
                    else
                    {						
						pModel->notifyViews(OPEN_FILEDIALOG_IMAGE_SEL.c_str());									
						if(pModel->m_bFileDialog)
						{
							strcpy((char*)data,pModel->m_selectImageFilePath.c_str());
                            return DLG_RETURN_TRUE;
						}                        
                    }
                    return DLG_BURNIMAGE_CANCEL;
                    break;
                }
            case DLG_WAITCD:
            case DLG_NON_EMPTY_CDRW:
                {
					static bool flag = true;
                    TA_Base_Bus::TransActiveMessage userMsg;     
					if (flag && IDCANCEL == userMsg.showMsgBox( IDS_UE_050081))
                    {
                        TA_Base_App::BurnMediaPModel* pModel = static_cast< TA_Base_App::BurnMediaPModel* >( pUserData );
                        if ( pModel == 0 )
                        {
                            LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, 
                                "Failed to cast void* to BurnMediaPModel*. Can't process Nero user dialog callback." );
                        }
                        else
                        {
                            pModel->m_stateManager.setState( StateManager::STOPPING );
                        }
                    }
					else
					{
						flag ^= true;
					}
                    return DLG_RETURN_EXIT;
                    break;
                }
            case DLG_WAITCD_REMINDER:
            case DLG_AUTO_INSERT_RESTART:
            case DLG_RESTART:
            case DLG_AUDIO_PROBLEMS:
            case DLG_WAITCD_DONE:
            default:

                return DLG_RETURN_EXIT;
                break;
        }
        return DLG_RETURN_EXIT;
    }


    BOOL NERO_CALLBACK_ATTR BurnMediaPModel::neroIdleCallback(void *pUserData)
    {
        TA_Base_App::BurnMediaPModel* pModel = static_cast< TA_Base_App::BurnMediaPModel* >( pUserData );
        if ( pModel == 0 )
        {
            LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, 
                "Failed to cast void* to BurnMediaPModel*. Can't process BurnMediaPModel UI messages." );
            return false;  // Assume we're not aborting.
        }
         
        bool isAborting = false;
        if ( StateManager::STOPPING == pModel->m_stateManager.getState() )
        {
            isAborting = true;
        }
        static MSG msg;
        while ( ::PeekMessage(&msg,NULL,NULL,NULL,PM_NOREMOVE) && !isAborting )
        {
            /*if (!AfxGetThread()->PumpMessage())				
            {
                break;
            }*/
			::TranslateMessage (&msg);
			::DispatchMessage (&msg); //tODO check
        }
        return isAborting;
    }


    BOOL NERO_CALLBACK_ATTR BurnMediaPModel::neroProgressCallback(void *pUserData, DWORD dwProgressInPercent)
    {
        TA_Base_App::BurnMediaPModel* dlg = static_cast< TA_Base_App::BurnMediaPModel* >( pUserData );
        if ( dlg == 0 )
        {
            LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, 
                "Failed to cast void* to BurnMediaPModel*. Can't process Nero progress callback." );
            return false;  // Assume we're not aborting.
        }
         
		GenericEventArgs args;
		args.Param1=&dwProgressInPercent;
		dlg->notifyViews(SET_PROGRESS_PERCENT_BURN_DLG.c_str(),&args);
        if ( StateManager::STOPPING == dlg->m_stateManager.getState() )
        {
            return true;
        }
        return false;
    }


    BOOL NERO_CALLBACK_ATTR BurnMediaPModel::neroAbortedCallback(void *pUserData)
    {
        TA_Base_App::BurnMediaPModel* dlg = static_cast< TA_Base_App::BurnMediaPModel* >( pUserData );
        if ( dlg == 0 )
        {
            LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, 
                "Failed to cast void* to BurnMediaPModel*. Can't process Nero aborted callback." );
            return false;  // Assume we're not aborting.
        }
        if ( StateManager::STOPPING == dlg->m_stateManager.getState() )
        {
            return true;
        }
        return false;
    }

	void BurnMediaPModel::cleanUpNero()
	{
		neroApiFree();

		while (!m_neroIsoItemsList.empty())
        {
			NERO_ISO_ITEM* item = (NERO_ISO_ITEM*)m_neroIsoItemsList.back();
			delete item;
			m_neroIsoItemsList.pop_back();
        }   
	}


    void BurnMediaPModel::addFile( const std::string& fileNameWithPath, const std::string& fileName, bool isDirectory )
    {
        //  Call addFile() for every file that will be burnt to DVD
        //  Limitations:
        //  -   only writes a single directory layer (NERO_ISO_ITEM.subDirFirstItem not used)
        //  -   only writes to DVD's root directory
        //  -   writes in the order you call this routine
        //
        //  If fixing these later, the ISO item tree needs to be set up as:
        //
        //  Ptr     Sourcename          Destname            nextItem    subDirFirstItem   
        //
        //  <1>     c:\dirOne\FileA.1   \dirOne\FileA.1     <2>         NULL
        //  <2>     c:\dirOne\FileB.2   \dirOne\FileB.2        
        //  <3>     c:\dirOne\FileC.3   \dirOne\FileC.3        
        //  <4>     c:\dirTwo\FileD.4   \dirTwo\FileD.4        

        // First check that the file/directory exists.

        if ( isDirectory )
        {
            if ( !TA_ArchiveManager::isValidDirectory( fileNameWithPath ) )
            {
				std::string error = IGuiObserver::getInstance()->getStringResourceFormated(UW050004_ADD_DIR_FAILED,fileNameWithPath,UW050004_REASON_INVALID_DIR);
				TA_THROW( TA_Base_Core::ArchiveException( error ) );
            }
        }
        else // We're expecting a file.
        {
            if ( !TA_ArchiveManager::isValidFile( fileNameWithPath ) )
            {
				std::string error = IGuiObserver::getInstance()->getStringResourceFormated(UW050004_ADD_DIR_FAILED,fileNameWithPath,UW050016_REASON_INVALID_FILE);
				TA_THROW( TA_Base_Core::ArchiveException( error ) );
            }
        }

        // We're not using these because they don't work:
        // pNii = NeroCreateIsoItem();
        // pNii = NeroCreateIsoItemOfSize( sizeof( NERO_ISO_ITEM) );

        NERO_ISO_ITEM* pNii = new NERO_ISO_ITEM;
        if ( pNii == 0 )
        {
            std::string error;
            if ( isDirectory )
            {
                error = IGuiObserver::getInstance()->getStringResourceFormated( UW050004_ADD_DIR_FAILED, fileNameWithPath.c_str(), UW050004_UW050016_REASON_MEMORY );
            }
            else
            {
                error = IGuiObserver::getInstance()->getStringResourceFormated( UW050016_ADD_FILE_FAILED, fileNameWithPath.c_str(), UW050004_UW050016_REASON_MEMORY );
            }
			TA_THROW( TA_Base_Core::ArchiveException( error ) );
        }
        ZeroMemory( pNii, sizeof( NERO_ISO_ITEM ) );
        pNii->itemSize = sizeof( NERO_ISO_ITEM );
        pNii->isReference = false;
        pNii->isDirectory = isDirectory;
        strcpy( pNii->sourceFilePath, fileNameWithPath.c_str() );
        strcpy( pNii->fileName, fileName.c_str() );

        std::string s( "File: " + std::string( pNii->fileName ));
		m_logString =  s.c_str() ;
		notifyViews(APPEND_STRING_LOG_DISPLAY.c_str());

        NERO_ISO_ITEM* pPrevNii = 0;
		if ( !m_neroIsoItemsList.empty() )
        {
			pPrevNii = (NERO_ISO_ITEM*) m_neroIsoItemsList.back();
        }
		m_neroIsoItemsList.push_back( pNii);

        if ( 0 != pPrevNii )
        {
            pPrevNii->nextItem = pNii;
        }
    }
    

    //*************************** END NERO


   
	std::string BurnMediaPModel::formatStringHelper( const std::string& strSrc)
	{
		//changed ?YYYY_MM_DD*.dmp to ?DDMMYY*.dmp
		
		std::string strDate = strSrc.substr( strlen("?YYYY_MM_"), strlen("DD") );
		strDate += strSrc.substr( strlen("?YYYY_"), strlen("MM") );
		strDate += strSrc.substr( strlen("?YY"), strlen("YY") );
		
		std::string strDes = strSrc;
		
		strDes.replace( strlen("?"), strlen("YYYY_MM_DD_"), strDate );
		
		return strDes;
	}

} // TA_Base_App
