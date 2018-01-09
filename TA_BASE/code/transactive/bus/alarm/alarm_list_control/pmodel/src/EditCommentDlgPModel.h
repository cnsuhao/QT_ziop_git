#pragma once


#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"


namespace TA_Base_Bus
{
	class EditCommentDlgPModel
		:	public AbstractPModel
	{
	public:
		EditCommentDlgPModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
		~EditCommentDlgPModel();

		/*
		* This is to set the comment edit box to the param comment
		*
		* @param comment string to be shown
		* @return void
		*
		*/
		void setEditComment(std::string comment);

		/*
		* This is to get the comment edit box to the param comment
		*
		* @param none
		* @return string of the edited comment
		*
		*/
		std::string getEditComment()
		{
			return std::string(m_comment);
		}

		static const long MAXIMUM_COMMENT_LENGTH;
	private:
		std::string m_comment;
	};

}