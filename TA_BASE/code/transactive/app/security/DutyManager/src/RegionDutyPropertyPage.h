#if !defined(AFX_REGIONDUTYPROPERTYPAGE_H__D4338974_3D84_469D_9E50_6B56AE4C3822__INCLUDED_)
#define AFX_REGIONDUTYPROPERTYPAGE_H__D4338974_3D84_469D_9E50_6B56AE4C3822__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/RegionDutyPropertyPage.h $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * This class represents the "Regions" (Locations) tab. It displays the active 
  * region duty state for logged in operators.
  *
  */

#include "bus/generic_gui_view/src/TransActivePropertySheet.h"



namespace TA_Base_App
{
	class RegionDutyPropertyPageModel;

    class RegionDutyPropertyPage : public AbstractPropertyPage
    {
	    //DECLARE_DYNCREATE(RegionDutyPropertyPage)

    // Operations
    public:
	    RegionDutyPropertyPage(ITransActiveWinApp * pWinApp, UINT nIDTemplate, UINT nIDCaption = 0 );
	    virtual ~RegionDutyPropertyPage();

		//+++++++++++++++++++++
		virtual std::string getMyPModelName();
		virtual void setupPModel();
		virtual void initUIControl();
		//+++++++++++++++++++++++
		TA_Base_Bus::DataCache&				getDataCache();	
		TA_Base_Bus::SessionCache&			getSessionCashe();
		TA_Base_Bus::RegionDutyMatrix&		getRegionDutyMatrix();
		std::string							getRegionDisplayName(RegionKey key);
		std::string							getRegionName(RegionKey key);
		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args=NULL);
		virtual void requestUserReply(TA_Base_Bus::UIReplyEventArgsPtr uiEvent);

        //// ISessionObserver interface
        //virtual void sessionChanged(const TA_Base_Bus::SessionUpdate& sessionUpdate);

        //// IRegionDutyObserver interface
        //virtual void dutyChanged(const TA_Base_Bus::RegionDutyUpdate& dutyUpdate);

         //TD17082++ made public to allow the client handle the initialization call
		void initialiseRegionDutyGrid();

		void initialiseRegionDutyGridStatic();

		void updateRegionDutyGrid();

		void redrawScrollBar();

		void OnRegionDutyUpdate(const RegionDutyUpdate& dutyUpdate);
		void OnSessionUpdate(const SessionUpdate& sessionUpdate);

    protected:
        void enableDutyControlButtons();
        void addOperator(const TA_Base_Bus::SessionInfoEx& sessionInfoEx);
        void addOperator(const TA_Base_Bus::SessionId& sessionId);
        void updateOperator(int row, const TA_Base_Bus::SessionInfoEx& sessionInfoEx);
        void updateOperator(int row, const TA_Base_Bus::SessionId& sessionId);
        void removeOperator(int row);

    // Overrides
	    // ClassWizard generate virtual function overrides
	    //{{AFX_VIRTUAL(RegionDutyPropertyPage)
	protected:
	    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	    //}}AFX_VIRTUAL


    // Implementation
    protected:
	    // Generated message map functions
	    //{{AFX_MSG(RegionDutyPropertyPage)
	    virtual BOOL OnInitDialog();
	    afx_msg void OnSize(UINT nType, int cx, int cy);
	    afx_msg void OnTransferRegionDutyBtn();
	    afx_msg void OnDelegateSubDutyBtn();
        //afx_msg LRESULT OnReceiveSessionUpdate(WPARAM wParam, LPARAM lParam);
        //afx_msg LRESULT OnReceiveRegionDutyUpdate(WPARAM wParam, LPARAM lParam);
	    //}}AFX_MSG
	    DECLARE_MESSAGE_MAP()


    // Dialog Data
    private:
	    //{{AFX_DATA(RegionDutyPropertyPage)
	    enum { IDD = IDD_REGIONS_DLG };
	    CButton	m_transferRegionDutyBtn;
	    CColorButton	m_delegateSubsystemDutyBtn;
	    //}}AFX_DATA

        CGridCtrl						m_regionDutyGrid;
		RegionDutyPropertyPageModel*	m_realPModel;
    };

} // namespace TA_Base_App

#endif // !defined(AFX_REGIONDUTYPROPERTYPAGE_H__D4338974_3D84_469D_9E50_6B56AE4C3822__INCLUDED_)
