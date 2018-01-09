/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/EquipmentStatusViewer/src/FileUtility.cpp $
 * @author:  Rod Rolirad
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/09/03 14:11:12 $
 * Last modified by:  $Author: CM $
 * 
 * Class to allow to export a report to a CSV file
 *
 */

#include "stdafx.h"
#include "core\utilities\src\TAAssert.h"
#include "core\utilities\src\DebugUtil.h"
#include "app/scada/EquipmentStatusViewer/src/FileUtility.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/CustomDefines.h"
#include "core/utilities/src/DateFormat.h" // TD12474 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace
{
	static const TAUnicodeString LF(_T("\r\n"));
	static const TAUnicodeString SEPARATOR(_T(","));
}


namespace TA_Base_App
{
	CFileUtility::CFileUtility()
	{
		m_szFileName = _T("");
		m_szLocationName = _T("");
		m_szReportName = _T("");
	}

	
	CFileUtility::~CFileUtility() 
	{
	}


	void CFileUtility::SetFileName(const TAUnicodeString& fileName)
	{
		if (!fileName.IsEmpty())
        {
			m_szFileName = fileName;
        }
	}


	void CFileUtility::SetReportName(const TAUnicodeString& reportName)
	{
		if (!reportName.IsEmpty())
        {
			m_szReportName = reportName;
        }
	}


	void CFileUtility::SetLocationName(const TAUnicodeString& locationName)
	{
		if (!locationName.IsEmpty())
        {
			m_szLocationName = locationName;
        }
	}


	void CFileUtility::SetReportComment(const TAUnicodeString& comment)
	{
		m_szComment = comment;
	}


	bool CFileUtility::ExportToCSV(const CListCtrl* equipmentListCtrl)
	{
        TA_ASSERT(equipmentListCtrl != NULL, "Invalid equipment list control");

		if ((equipmentListCtrl == NULL) || (m_szFileName.IsEmpty()) ||
			(m_szLocationName.IsEmpty()) || (m_szReportName.IsEmpty()))
		{
			// cannot export without these being valid or set
			return false;
		}

        // Determine which equipment list columns should be included in the output.
        // Gotta cast away the constness of equipmentListCtrl as GetHeaderCtrl() is,
        // for some reason, not a const function.
        const CHeaderCtrl& equipmentHeaderCtrl = *(const_cast<CListCtrl*>(equipmentListCtrl)->GetHeaderCtrl());
        ColumnMap columnMap(getColumns(equipmentHeaderCtrl));

        // Generate export file header, including column headings.
		TAUnicodeString reportContent(generateHeader(columnMap));

        // Generate the remainder of the report as comma separated values.
        int rowCount(equipmentListCtrl->GetItemCount());
        ColumnMap::const_iterator colIt(columnMap.begin());

		for (int row(0); row < rowCount; row++)
		{
            TAUnicodeString reportItem(_T(""));

			for (colIt = columnMap.begin(); colIt != columnMap.end(); colIt++)
			{
                if (!reportItem.IsEmpty())
                {
                    reportItem += SEPARATOR;
                }

				TAUnicodeString itemText;
				itemText.str() = equipmentListCtrl->GetItemText(row, colIt->first);
                reportItem += adjustForCSVFormat(itemText);
			}

            reportContent += reportItem + LF;
		}

        // Dump the report to file.
   		return write(reportContent);
	}


    CFileUtility::ColumnMap CFileUtility::getColumns(const CHeaderCtrl& equipmentHeaderCtrl) const
    {
        ColumnMap columnMap;
        int columnCount(equipmentHeaderCtrl.GetItemCount());
        const int MAX_COL_HEADING_LEN(256);
     
        // Iterate over the columns in the equipment list control and build
        // up the map of output columns. Included is the Quality column (TES 763)
        // + any visible (non-zero width) column.
        for (int col(0); col < columnCount; col++)
        {
            TAUnicodeString columnHeading;
            HDITEM hdItem;
            hdItem.mask = HDI_WIDTH | HDI_TEXT;
            hdItem.pszText = columnHeading.str().GetBuffer(MAX_COL_HEADING_LEN);
            hdItem.cchTextMax = MAX_COL_HEADING_LEN;
            
            equipmentHeaderCtrl.GetItem(col, &hdItem);

            if (hdItem.cxy > 0 || col == QUALITY_COLUMN)
            {
                columnMap.insert(std::make_pair(col, columnHeading));
			}
        }

        return columnMap;
    }


	TAUnicodeString CFileUtility::generateHeader(const ColumnMap& columnMap) const
	{		
        // Header format:
        //
        // <current date and time>
        // <location name>
        // <report name>
        // [<comment>]
        //
        // <column1>,<column2>, ... ,<columnN>

		// TD12474 ++
		//TAUnicodeString headerText = CTime::GetCurrentTime().Format("%d %b %Y %I:%M%p") + LF;		

		TA_Base_Core::DateFormat dateFormat; 
		std::string dateFormatStr = dateFormat.getDateFormat(TA_Base_Core::DateFormat::DateTimeFormatNoSec3);

		TAUnicodeString headerText;
		headerText.str() = CTime::GetCurrentTime().Format(dateFormatStr.c_str());
		headerText += LF;
		// ++ TD12474

		headerText += m_szLocationName + LF;
		headerText += m_szReportName + LF;
		if (m_szComment.IsEmpty() == FALSE)
		{
            headerText += adjustForCSVFormat(m_szComment) + LF;
		}
		headerText += LF;

        TAUnicodeString columnHeadings(_T(""));
        for (ColumnMap::const_iterator colIt = columnMap.begin(); colIt != columnMap.end(); colIt++)
        {
            if (!columnHeadings.IsEmpty())
            {
                columnHeadings += SEPARATOR;
            }
 
			columnHeadings += colIt->second;
        }

		headerText += columnHeadings + LF;

        return headerText;
	}


    TAUnicodeString CFileUtility::adjustForCSVFormat(const TAUnicodeString& text) const
    {
        TAUnicodeString result = text;

        // First of all, replace all quotes with double quotes "" - 
        // CSV format interprets double quotes as special code for single quote..
        result.str().Replace(_T("\""), _T("\"\""));
        
        // Then place quotes around the text, which allows text with comma's to be
        // present in this field (if we don't surround with quotes, a comma will
        // mess up the delimiting)
        result = _T("\"") + result + _T("\"");
        
        return result;
    }


	bool CFileUtility::write(const TAUnicodeString& bufferToWrite)
	{
		if (m_szFileName.IsEmpty() || bufferToWrite.IsEmpty())
        {
			return false;
        }

        try
        {
            CFile exportFile(m_szFileName.c_str(), CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite);
                             
            exportFile.Write(bufferToWrite.c_str(), bufferToWrite.GetLength());
            exportFile.Flush();
            exportFile.Close();

  		    return true;
        }
        catch (const CFileException& ex)
        {
            LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError,
                        "Failed to write to file %s (error %d)", 
                        static_cast<LPCTSTR>(m_szFileName.c_str()), ex.m_cause);
        }

        return false;
	}

} // TA_Base_App
