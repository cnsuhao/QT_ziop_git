/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/EquipmentStatusViewer/src/FileUtility.h $
 * @author:  Rod Rolirad
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/09/03 14:11:12 $
 * Last modified by:  $Author: CM $
 * 
 * Class to allow to export a report to a CSV file
 *
 */
#if !defined(AFX_FILEUTILITY_H__3E5F996E_0D91_403E_89BF_862605AA3F07__INCLUDED_)
#define AFX_FILEUTILITY_H__3E5F996E_0D91_403E_89BF_862605AA3F07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


namespace TA_Base_App
{
	class CFileUtility
	{

	public:
		void SetReportComment(const TAUnicodeString& comment);

		/**
		 *	SetLocationName()
		 *
		 *	Sets the location name to include in the CSV file export
		 *
		 *	@param	locationName	Name of the location
		 *
		 */
		void SetLocationName(const TAUnicodeString& locationName);

		/**
		 *	SetReportName()
		 *
		 *	Sets the report name to include in the CSV file export
		 *
		 *	@param	reportName		Name of the report
		 *
		 */
		void SetReportName(const TAUnicodeString& reportName);

		/**
		 *	SetFileName()
		 *
		 *	Sets the name of the CSV file to export report to
		 *
		 *	@param	fileName		Name of the file to export to
		 *
		 */
		void SetFileName(const TAUnicodeString& fileName);
		
		/**
		 *	ExportToCSV()
		 *
		 *	Exports the list to the CSV file
		 *
		 *	@param	equipmentListCtrl	Pointer to the equipment list to export
		 *	@return                     Bool to signify success (true) or failure (false)
		 *
		 */
		bool ExportToCSV(const CListCtrl* equipmentListCtrl);		

		/**
		 *	CFileUtility()
		 *
		 *	Default constructor
		 *
		 */
		CFileUtility();

		/**
		 *	~CFileUtility()
		 *
		 *	Default destructor
		 *
		 */
		virtual ~CFileUtility();

	private:
        
        typedef std::map<int, TAUnicodeString> ColumnMap; // column index -> column heading

		/**
		 *	getColumns()
		 *
		 *	Determines from the specified equipment list header control the output columns to be included
         *  in the exported file. Included are all non-zero sized columns + the Quality column.
		 *
		 *	@param	equipmentHeaderCtrl		Equipment list header control
         *
         *  @return column heading map keyed by column index
		 *
		 */
		ColumnMap getColumns(const CHeaderCtrl& equipmentHeaderCtrl) const;

		/**
		 *	generateHeader()
		 *
		 *	Builds a header for the exported file. This includes the current time, 
         *  location, report name, additional operator comment and column headings.
		 *
		 *	@param	columnMap		output columns headings
         *
         *  @return header text
		 *
		 */
		TAUnicodeString generateHeader(const ColumnMap& columnMap) const;

        /**
         * Appropriately formats the input text so that it may be able
         *  to be safely added to a CSV file (if it contains comma's or " quotes, it
         *  can interfere with the core formatting mechanism of CSV)
         * @return correctly formatted string, based on input text
         */
        TAUnicodeString adjustForCSVFormat(const TAUnicodeString& text) const;

		/**
		 *	write()
		 *
		 *	Writes the created buffer to the file (includes header)
		 *
		 *	@param	bufferToWrite	Buffer to write to file
		 *
		 */
		bool write(const TAUnicodeString& bufferToWrite);
		
		/**
		 *	Name of the file
		 *
		 */
		TAUnicodeString m_szFileName;		

		/**
		 *	Comment
		 *
		 */
		TAUnicodeString m_szComment;		

		/**
		 *	Name of the location
		 *
		 */
		TAUnicodeString m_szLocationName;	

		/**
		 *	Name of the report
		 *
		 */
		TAUnicodeString m_szReportName;		
		
	};

}

#endif // !defined(AFX_FILEUTILITY_H__3E5F996E_0D91_403E_89BF_862605AA3F07__INCLUDED_)
