#if defined ( STEPTYPE_GRC )

/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_agent/src/GRC3DataPointSetComplexStep.h $
  *  @author:  Gregg Kirkpatrick
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  <description>
  */

#if !defined(AFX_GRC3DATAPOINTSETCOMPLEXSTEP_H__6DFF67B5_501A_4878_AECB_A71D2976ADCE__INCLUDED_)
#define AFX_GRC3DATAPOINTSETCOMPLEXSTEP_H__6DFF67B5_501A_4878_AECB_A71D2976ADCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

#include "app/response_plans/plan_agent/src/ActiveStep.h"

namespace TA_Base_App
{

class ActiveDataPointSetStep;
class GRCType3SubTask;
class IActivePlanFlow;

class GRC3DataPointSetComplexStep  : public ActiveStep 
{
    // Public methods
public:
    GRC3DataPointSetComplexStep(const TA_Base_Core::StepDetail& stepDetail, IActivePlanFlow& flowControl);
    virtual ~GRC3DataPointSetComplexStep();

    void addChildStep(ActiveDataPointSetStep *pDataPointSetStep);

protected:
    virtual TA_Base_Core::EActiveStepState executeImpl( void );

    bool doSetDataPoint( );

    void startAllSubTaskThread();
    void stopAllSubTaskThread();

private:
    GRC3DataPointSetComplexStep& operator=( const GRC3DataPointSetComplexStep& );
    GRC3DataPointSetComplexStep( const GRC3DataPointSetComplexStep& );

private:
    std::vector<GRCType3SubTask*> m_vtDataPointSetSubTask;
    std::vector<ActiveDataPointSetStep*> m_vtActiveDataPointSetStep;
};

}

#endif // !defined(AFX_GRC3DATAPOINTSETCOMPLEXSTEP_H__6DFF67B5_501A_4878_AECB_A71D2976ADCE__INCLUDED_)

#endif
