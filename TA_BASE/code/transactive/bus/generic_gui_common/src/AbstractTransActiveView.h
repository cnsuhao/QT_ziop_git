#ifndef _ABSTRACT_TRANSACTIVE_VIEW_H__
#define _ABSTRACT_TRANSACTIVE_VIEW_H__

#ifdef WIN32
#include <windows.h>
#endif


#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "core/utilities/src/TAAssert.h"

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
		AbstractTransActiveView(ITransActiveWinApp * pApp)    
			: m_pModel(NULL)
			, m_pWinApp(pApp)
			, m_hasPrivatePModel(false)
		{

		}

		AbstractTransActiveView()    
			: m_pModel(NULL)
			, m_pWinApp(NULL)
			, m_hasPrivatePModel(false)
		{
			m_pWinApp = dynamic_cast<ITransActiveWinApp*> (AfxGetApp());
			TA_ASSERT(m_pWinApp != NULL, "TransActiveApp is valid");
		}

		~AbstractTransActiveView() 
		{
            if (m_hasPrivatePModel && m_pModel != NULL)
            {
                m_pModel->cleanup();
                delete m_pModel;
                m_pModel = NULL;
            }
		};

	public:
		/** get pointer to ITransActiveApp
		* 
		*
		*  @return ITransActiveWinApp *
		*/
		ITransActiveWinApp * getTransActiveWinApp()
		{
			return m_pWinApp;
		}

		virtual RECT getViewPosition()
		{
			RECT rect;
			SetRectEmpty(&rect);
			HWND hwnd = getViewHandle();

			if (hwnd != NULL)
			{
				GetWindowRect(hwnd, &rect);
			}
			return rect;
		}


	public:
		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args=NULL) {};

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
            if ( !m_hasPrivatePModel )
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
		 *	__NOTE__: do initial registration for pmodel data update in this function
		 */
		virtual void setupPModel() = 0;
	private:
		bool hasPrivatePModel()
		{
			const std::string pmodelName = getMyPModelName();

			//if the PModel is owned by the view, it should be created beforehand
			return (pmodelName.empty());
		}

	protected:
		ITransActivePModel * m_pModel;	///< pointer to corresponding model object	
		ITransActiveWinApp * m_pWinApp; ///< pointer to ITransActiveApp
		bool m_hasPrivatePModel;        ///< has own private PModel or shared PModel
	};

	 
}

#endif
