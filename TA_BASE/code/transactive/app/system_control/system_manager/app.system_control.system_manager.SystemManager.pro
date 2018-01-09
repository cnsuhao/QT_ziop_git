TRANSLATIONS += \
	SystemManager_en_US.ts \
	SystemManager_zh_CN.ts \

DEPENDPATH += \
	$$PWD/src/ \
	$$PWD/pmodel/src/ \

HEADERS	+= \
	src/AbstructQDialog.h \
	src/AbstructQDialogTemplate.h \
	src/ChangeOperationModeDlg.h \
	src/Factory.h \
	src/MyAbstractDialog.h \
	src/OptionsDlg.h \
	src/ProcessDataPuller.h \
	src/ProcessParamsDlg.h \
	src/ProcessTableWidget.h \
	src/QtUtilities.h \
	src/ShutdownSystemControllerDlg.h \
	src/StartProcessDlg.h \
	src/StopProcessDlg.h \
	src/StringResource.h \
	src/StyleDlg.h \
	src/SystemControllerDlg.h \
	src/SystemManager.h \
	src/SystemManagerApp.h \
	src/SystemManagerDlg.h \
	src/TransActiveLoginDlg.h \
	src/Translation.h \

SOURCES += \
	src/AbstructQDialog.cpp \
	src/ChangeOperationModeDlg.cpp \
	src/Factory.cpp \
	src/MyAbstractDialog.cpp \
	src/OptionsDlg.cpp \
	src/ProcessDataPuller.cpp \
	src/ProcessParamsDlg.cpp \
	src/ProcessTableWidget.cpp \
	src/QtUtilities.cpp \
	src/ShutdownSystemControllerDlg.cpp \
	src/StartProcessDlg.cpp \
	src/StopProcessDlg.cpp \
	src/StyleDlg.cpp \
	src/SystemControllerDlg.cpp \
	src/SystemManager.cpp \
	src/SystemManagerApp.cpp \
	src/SystemManagerDlg.cpp \
	src/TransActiveLoginDlg.cpp \
	src/VersionGen.cpp \
	src/ViewFactory.cpp \
	src/main.cpp \
	src/processlist.cpp \
	src/Translation.cpp \

FORMS += \
	src/ChangeOperationModeDlg.ui \
	src/OptionsDlg.ui \
	src/ProcessParamsDlg.ui \
	src/ShutdownSystemControllerDlg.ui \
	src/StartProcessDlg.ui \
	src/StopProcessDlg.ui \
	src/StyleDlg.ui \
	src/SystemControllerDlg.ui \
	src/SystemManagerDlg.ui \
	src/TransActiveLoginDlg.ui \
