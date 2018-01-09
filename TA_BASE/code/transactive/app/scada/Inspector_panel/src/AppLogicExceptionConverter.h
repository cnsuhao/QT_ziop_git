#pragma  once
#include "StdAfx.h"
#include "../pmodel/src/OutputLogicException.h"


namespace TA_Base_App
{
	class AppLogicExceptionConverter
	{
	public:
		static TAUnicodeString	ConvertException(OutputLogicException&ex)
		{
			TAUnicodeString rtnString;
			int ExCode = ex.getExceptionCode();
			switch(ExCode)
			{
			case Ex_Code_ScadaProxyException:
				rtnString = TALoadString(STRING_ID_INSPECTOR_PANEL_0300);
				break;
			case Ex_Code_NotAvailableException:
				rtnString = TALoadString(STRING_ID_INSPECTOR_PANEL_0301);
				break;
			case Ex_Code_BadParameterException:
				rtnString = TALoadString(STRING_ID_INSPECTOR_PANEL_0302);
				break;
			case Ex_Code_AccessDeniedException:
				rtnString = TALoadString(STRING_ID_INSPECTOR_PANEL_0303);
				break;
			case Ex_Code_ObjectResolutionException:
				rtnString = TALoadString(STRING_ID_INSPECTOR_PANEL_0304);
				break;
			case Ex_Code_OperationModeException:
				rtnString = TALoadString(STRING_ID_INSPECTOR_PANEL_0305);
				break;
			case Ex_Code_CORBA_Exception:
				rtnString = TALoadString(STRING_ID_INSPECTOR_PANEL_0306);
				break;
			case Ex_Code_UnknownException:
				rtnString = TALoadString(STRING_ID_INSPECTOR_PANEL_0307);
				break;
			case Ex_Code_InvalidValue1_1:
				{
					std::vector<std::string> parags = ex.getParameters();
					TA_ASSERT(parags.size() == 1,"invalid size");

					rtnString = TAAfxFormatString1(STRING_ID_INSPECTOR_PANEL_0309, CONVERT_FROM_STD(parags[0]));
				}
				break;
			case Ex_Code_InvalidValue2_3:
				{
					std::vector<std::string> parags = ex.getParameters();
					TA_ASSERT(parags.size() == 3,"invalid size");

					rtnString = TAAfxFormatString3(STRING_ID_INSPECTOR_PANEL_0310, CONVERT_FROM_STD(parags[0]), CONVERT_FROM_STD(parags[1]), CONVERT_FROM_STD(parags[2]));
				}
				break;
			case Ex_Code_InvalidValue3_1:
				{
					std::vector<std::string> parags = ex.getParameters();
					TA_ASSERT(parags.size() == 1,"invalid size");

					rtnString = TAAfxFormatString1(STRING_ID_INSPECTOR_PANEL_0311, CONVERT_FROM_STD(parags[0]));
				}
				break;
			default:
				rtnString = TALoadString(STRING_ID_INSPECTOR_PANEL_0308);
				break;
			}

			return rtnString;
		}
	};



	//low level

}