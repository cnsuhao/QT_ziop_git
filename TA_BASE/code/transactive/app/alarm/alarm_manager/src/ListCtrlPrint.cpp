
#include "ListCtrlPrint.h"
#include "bus\common_control\src\printdialog\CtlPrintDialog.h"
#include <QMetaType>

namespace TA_Base_App
{
	CListCtrlPrint::CListCtrlPrint()
		: m_printer(0)
		, m_document(0)
	{
		csFtHead = "Arial";
		csFtFoot = "Times New Roman";
		csFtPrint = "Arial";
		iFtHead = 10;
		iFtFoot = 6;
		iFtPrint = 8;
		csPage = "Page %d of %d";
		pImageStringVector = NULL;
	}

	CListCtrlPrint::~CListCtrlPrint()
	{

	}

	void CListCtrlPrint::print(t_ParamsToReceive* params)
	{
		// Copy the parameters.
		csPageHead = params->csPageHead;
		strTitle = params->title;
		csPage = params->csPage;
		pList = params->list;
		pImageStringVector = params->imageStringVector;
		m_printer = params->printer;
		m_document = params->document;
		start();
	}

	int CListCtrlPrint::getItemCountPerPage()
	{
		return 0;
	}

	void CListCtrlPrint::run()
	{
		QString strStream;
		QTextStream out(&strStream);

		const int rowCount = pList->model()->rowCount();
		const int columnCount = pList->model()->columnCount();

		out << "<html>\n"
			"<head>\n"
			"<meta Content=\"Text/html; charset=utf-8\">\n"
			<< QString("<title>%1</title>\n").arg(strTitle)
			<< "</head>\n"
			"<body bgcolor=#ffffff link=#5000A0>\n";

		// Report header
		out << "<p><b>" << csPageHead << "</b></p>";

		out << "<table border=1 cellspacing=0 cellpadding=2>\n";
		// headers
		out << "<thead><tr bgcolor=#f0f0f0>";
		for (int column = 0; column < columnCount; column++)
			if (!pList->isColumnHidden(column))
				out << QString("<th>%1</th>").arg(pList->model()->headerData(column, Qt::Horizontal).toString());
		out << "</tr></thead>\n";

		// data table
		for (int row = 0; row < rowCount; row++) {
			out << "<tr>";
			for (int column = 0; column < columnCount; column++) {
				if (!pList->isColumnHidden(column)) 
				{
					QModelIndex mIndex = pList->model()->index(row, column, QModelIndex());
					QString textData = pList->model()->data(mIndex, Qt::DisplayRole).value<QString>();
					QString foreground = pList->model()->data(mIndex, Qt::ForegroundRole).value<QColor>().name();
					
					QString background = pList->model()->data(mIndex, Qt::BackgroundColorRole).value<QColor>().name();

					QString data = pList->model()->data(pList->model()->index(row, column)).toString().simplified();
					out << QString("<td bgcolor=%1 > <font color=%2> %3 </font></td>").arg(background, foreground, (!textData.isEmpty()) ? textData : QString("&nbsp;"));
				}
			}
			out << "</tr>\n";
		}
		out << "</table>\n"
			"</body>\n"
			"</html>\n";

		QTextDocument *document = new QTextDocument();
		document->setHtml(strStream);
		document->setPageSize(m_printer->pageRect().size());
		document->print(m_printer);

		delete document;
	}

	void CListCtrlPrint::terminate()
	{

	}
}