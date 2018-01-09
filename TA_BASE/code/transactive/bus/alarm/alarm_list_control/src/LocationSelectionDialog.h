#ifndef LOCATIONSELECTIONDIALOG_H
#define LOCATIONSELECTIONDIALOG_H

#include <QWidget>
#include "ui_locationselectiondialog.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"

namespace TA_Base_Bus
{
	// Forward declaration
	class LocationSelectionDlgPModel;

	class LocationSelectionDialog : public AbstractDialog
	{
		Q_OBJECT
	public:
		/**
		* Constructs a new dialog with the given initial location selection
		*
		* @param initialLocation    The location to initially select in the drop down
		* @param pParent
		*/
		LocationSelectionDialog(TA_Base_Bus::ITransActiveApp* app);


		/**
		* Destructor
		*/
		virtual ~LocationSelectionDialog();

		/**
		* Sets the initial dialog selection
		*
		* @param initialLocation    The location to initially select in the drop down
		*/
		void setInitialSelection(unsigned long initialLocation);

		/**
		* This will get the location selected by the user.
		*
		* @return the selected location key
		*/
		unsigned long getSelectedLocation();
		
		//ITransActiveView interface
		virtual std::string  getMyPModelName(void);
		virtual void setupPModel(void);
		// For dynamic language switching
		virtual void translateUI();
	public slots:
		void onSelectionChangeLocationList(int index);
		void onOK();
		void onCancel();
	protected:
		virtual void initUIControl();
	private:
		Ui::LocationSelectionDialog ui;
		LocationSelectionDlgPModel* m_pTheRealPModel;
	};

}

#endif // LOCATIONSELECTIONDIALOG_H
