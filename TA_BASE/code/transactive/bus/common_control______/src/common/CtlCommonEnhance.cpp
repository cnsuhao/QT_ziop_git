 
#include "ctlCommonEnhance.h"

QT_BEGIN_NAMESPACE

ctlCommonEnhance::ctlCommonEnhance(QWidget* control):m_control(NULL),m_controlID("")
{
	m_control=control;
}
ctlCommonEnhance::~ctlCommonEnhance()
{
	 m_control=NULL;
}
 
QString ctlCommonEnhance::ControlID()
{
	return m_controlID;
}
void ctlCommonEnhance::setControlID(QString controlID)
{
	m_controlID=controlID;
	//test code
	m_control->setWindowTitle(m_controlID);	 
   //retrieve control configuration based on controlID
   //1. Visibility
   //2. Security Action ID
   //3. Caption,Tooltips
   //4. maximum permitted length of a textedit
   //5. Help
   //6. Style sheet
}
	 


QT_END_NAMESPACE