

#include "app/alarm/alarm_manager/src/ExportAlarms.h"

#include "bus\generic_gui_view\src\TransActiveMessage.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/DateFormat.h" // TD12474 
#include <QtGlobal>

using namespace TA_Base_Bus;

namespace TA_Base_App
{
	void CExportAlarms::saveNow(t_ParamsToReceive* params)
	{
		if (params == NULL)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError, "Could not export. Data passed to export thread was null.");
			TA_Base_Bus::TransActiveMessage userMsg;
			UINT selectedButton = userMsg.showMsgBox(IDS_UI_040030);
			return;
		}

		m_alarmList = params->alarmList;
		m_fileDlg = params->fileDlg;
		// ++ AZ: TD13826 - wrong default path
		if (params->defaultExportPath == "")
		{

			QByteArray defaultPath = qgetenv("%USERPROFILE%\\Documents");
			// default to desktop instead of my documents folder
			m_defaultExportPath = defaultPath.constData();
		}
		else
		{
			m_defaultExportPath = params->defaultExportPath;
		}
		// AZ: TD13826 ++
		m_pImageStringVector = params->pImageStringVector;
		delete params;

		start();
	}

	void CExportAlarms::run()
	{
		// This is a one way thread and will not loop
		LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "Export was selected");

		// Allocate storage for szSelections

		TAUtfString fileName = tr("Untitled");

		// Open the file read-only. Create and set size to 0. Will overwrite existing files.
		TAUtfString pathName = m_defaultExportPath + fileName;
		if (pathName.isEmpty()) pathName = EXPORTALARMS_LOCALPATH.c_str();
		QFile outFile(pathName);
		// Check the path and file name given for availability
		if (false == outFile.open(QIODevice::ReadWrite))
		{
			TCHAR buff[MAX_PATH];
			//The path and file name is not valid or available
			//Display a message that it is invalid and set the path to local drive
			TAUtfString msg = pathName + TAUtfString(".\n");
			msg += "Set and save to Local Directory";
			TA_Base_Bus::TransActiveMessage userMsg;
			userMsg << msg.std_str();
			UINT selectedButton = userMsg.showMsgBox(IDS_UI_040034);

			QByteArray defaultPath = qgetenv("%USERPROFILE%\\Documents");
			if (!defaultPath.isNull() && !defaultPath.isEmpty())
			{
				m_fileDlg->setDirectory(defaultPath.constData());
			}
		}
		else
		{
			m_fileDlg->setDirectory(m_defaultExportPath);
			// remove the file if it is created
			if (outFile.exists())
			{
				outFile.close();
				outFile.remove();
			}
		}

		m_fileDlg->moveToThread(QApplication::instance()->thread());

		if (m_fileDlg->doModal() != QDialog::Accepted)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "Export was canceled");
			return;
		}

		try
		{
			QString fileName = *m_fileDlg->selectedFiles().begin();
			outFile.setFileName(fileName);
			if (outFile.open(QIODevice::ReadWrite))
			{

				// Variables used repeatedly to write each piece of information to the file.
				std::string textToWrite;

				// Output the date as a heading
				QDateTime currTime = QDateTime::currentDateTime();
				// TD12474 ++
				//textToWrite = currTime.Format("%d %B %Y %H:%M");
				//TA_Base_Core::DateFormat dateFormat;
				//std::string dateFormatStr = dateFormat.getDateFormat(TA_Base_Core::DateFormat::DateTimeFormatNoSec1a);
				textToWrite = currTime.toString(Qt::SystemLocaleShortDate).toStdString();
				// ++ TD12474 
			
				textToWrite += "\n\n";
				outFile.write(textToWrite.c_str(), textToWrite.size());
				std::string headerToWrite;
				// Output the column headings to the file
				QHeaderView* pHeaderCtrl = m_alarmList->horizontalHeader();
				unsigned int colCount = pHeaderCtrl->count();
				
				//memorise the comment column index
				unsigned int commentCol = 0;
				bool foundComment = false;

				for (unsigned int i = 0; i<colCount; i++)
				{
					std::string headerItem = m_alarmList->model()->headerData(i, Qt::Horizontal, Qt::DisplayRole).value<QString>().toStdString();
					if (headerItem.compare("C") == 0)
					{
						commentCol = i;
						foundComment = true;
					}

					headerToWrite += headerItem;

					if (i != colCount - 1)
					{
						headerToWrite += ",";
					}
					else
					{
						headerToWrite += "\n";
					}
				}

				//textToWrite = "Ack,State,Severity,Entity,System,Region,Description,Time Logged,Time Created,Alarm Type,Comment\n";
				outFile.write(headerToWrite.c_str(), headerToWrite.size());

				std::string text;
				int rowCount = m_alarmList->model()->rowCount();
				for (int index = 0; index < rowCount; ++index)
				{
					// Now build up line of information
					std::string alarmItem;
					for (unsigned int i = 0; i<colCount; i++)
					{
						QModelIndex mIndex = m_alarmList->model()->index(index, i, QModelIndex());
						text = m_alarmList->model()->data(mIndex, Qt::DisplayRole).value<QString>().toStdString();
						if (text.empty() == true)
						{
							//check whether the column is image
							QPixmap image = m_alarmList->model()->data(mIndex, Qt::DecorationRole).value<QPixmap>();
							if (!image.isNull())
							{	// Todo: what image to write
								/*if (item.iImage >= 0 && m_pImageStringVector != NULL && m_pImageStringVector->size()>0)
									text = (*m_pImageStringVector)[item.iImage];
								else*/
									text = " "; // unknown string
							}
							else
							{
								text = " "; // unknown
							}
						}

						// if this is the comment col, get the alarm comment if any and append the text
						if (foundComment && i == commentCol && text.compare("C") == 0)
						{
							text = dynamic_cast<TA_Base_Bus::AlarmListCtrl*>(m_alarmList)->getAlarmComment(index);
						}

						if (!text.empty())
						{
							convertText(text);
						}
						alarmItem += text;

						if (i != colCount - 1)
						{
							alarmItem += ",";
						}
						else
						{
							alarmItem += "\n";
						}
					}
					outFile.write(alarmItem.c_str(), alarmItem.size());
				}

				outFile.flush();
				outFile.close();
			}
			else // the path and file are not valid
			{
				TA_Base_Bus::TransActiveMessage userMsg;
				userMsg << std::string(m_fileDlg->getOpenFileName().toStdString());
				UINT selectedButton = userMsg.showMsgBox(IDS_UI_040034);

				return;
			}
		}
		catch (...)
		{
			TA_Base_Bus::TransActiveMessage userMsg;
			userMsg << m_fileDlg->getOpenFileName().toStdString();
			UINT selectedButton = userMsg.showMsgBox(IDS_UI_040034);

			return;
		}

		// Display success message box
		TA_Base_Bus::TransActiveMessage userMsg;
		UINT selectedButton = userMsg.showMsgBox(IDS_UI_040031);
	}

	void CExportAlarms::terminate()
	{
		
	}
	
	void CExportAlarms::convertText(std::string& text)
	{
		// First check for comma. If there is none then the string can remain the same
		if (std::string::npos == text.find(','))
		{
			return;
		}

		// Ok now we have established there is a comma we must change any existing double quotes to "".
		std::string::size_type pos = text.find('\"');
		while (pos != std::string::npos)
		{
			text.insert(pos, "\"");
			pos = text.find('\"', pos + 2);
		}

		// Now add quotes at either end
		text.insert(0, "\"");
		text.insert(text.size(), "\"");
	}
}