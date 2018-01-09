#include "EditCommentDlgPModel.h"


namespace TA_Base_Bus
{

	const long EditCommentDlgPModel::MAXIMUM_COMMENT_LENGTH = 1024;

	EditCommentDlgPModel::EditCommentDlgPModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName)
		: TA_Base_Bus::AbstractPModel(appController, pmodelName)
	{
	}

	EditCommentDlgPModel::~EditCommentDlgPModel()
	{
	}

	void EditCommentDlgPModel::setEditComment(std::string comment)
	{
		m_comment = comment;
	}
}
