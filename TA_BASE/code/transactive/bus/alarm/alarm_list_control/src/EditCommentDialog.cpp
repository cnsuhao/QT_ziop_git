#include "EditCommentDialog.h"
#include "ConstDefine.h"
#include <QString>

#include "bus/alarm/alarm_list_control/pmodel/src/EditCommentDlgPModel.h"
#include "bus/alarm/alarm_list_control/pmodel/src/AlarmListModelConsts.h"

namespace TA_Base_Bus
{
	EditCommentDialog::EditCommentDialog(TA_Base_Bus::ITransActiveApp* app)
		: AbstractDialog(app, ViewType_EditComment)
	{
		ui.setupUi(this);

		this->m_pModel = new EditCommentDlgPModel(app->getApplicationController(), PMODEL_EDIT_COMMENT_DLG);
		Qt::WindowFlags flags = windowFlags();
		flags = flags | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint;
		this->setWindowFlags(flags);

		// Setup Connect Slots
		connect(ui.btnOk, SIGNAL(clicked()), this, SLOT(onOK()));
		connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
		connect(ui.txtComment, SIGNAL(textChanged()), this, SLOT(onCommentChange()));
	}

	EditCommentDialog::~EditCommentDialog()
	{

	}

	void EditCommentDialog::setupPModel(void)
	{	
		m_pMyModel = dynamic_cast<EditCommentDlgPModel*>(m_pModel);
	}

	// For dynamic language switching
	void EditCommentDialog::translateUI()
	{
		ui.retranslateUi(this);
	}

	void EditCommentDialog::onOK()
	{
		m_pMyModel->setEditComment(ui.txtComment->toPlainText());
		m_resultData->fromMultiByteString(ui.txtComment->toPlainText());
		QDialog::accept();
	}

	void EditCommentDialog::onCancel()
	{
		QDialog::reject();
	}

	void EditCommentDialog::onCommentChange()
	{
		//m_pMyModel->setEditComment(ui.txtComment->toPlainText().toStdString());
	}

	void EditCommentDialog::setEditComment(TAUtfString comment)
	{
		m_pMyModel->setEditComment(comment.toMultiByteStdString());
	}

	TAUtfString EditCommentDialog::getEditComment()
	{
		TAUtfString objTemp;
		objTemp.fromMultiByteString(m_pMyModel->getEditComment().c_str());
		return objTemp;
	}

	void EditCommentDialog::initUIControl()
	{
		// Setup Controls
		//ui.txtComment->setText(EditCommentDlgPModel::MAXIMUM_COMMENT_LENGTH);
	}

	void EditCommentDialog::OnInitDialog()
	{
		TAUtfString objTemp = TAUtfString::createFromMultitByteString(m_pMyModel->getEditComment().c_str());
		ui.txtComment->setText(objTemp.q_str());
	}
}
