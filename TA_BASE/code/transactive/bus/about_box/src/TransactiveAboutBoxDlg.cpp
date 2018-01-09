#include "TransactiveAboutBoxDlg.h"
#include <QTimer>


TransactiveAboutBoxDlg::TransactiveAboutBoxDlg( const char buildVersion[],
                                                const char buildDate[],
                                                const char componentName[],
                                                const char componentVersion[],
                                                const int headerArraySize,
                                                char* headerArray[] )
{
    ui.setupUi( this );

	m_videoWidget = new QVideoWidget;
	m_videoWidget->setParent(ui.centralWidget);
	m_videoWidget->setGeometry(QRect(210, 10, 191, 61));
	
	m_mediaPlayer = new QMediaPlayer;
	m_mediaPlayer->setVideoOutput(m_videoWidget);
	m_mediaPlayer->setMedia(QUrl::fromLocalFile(":/logo.avi"));
	
	m_videoWidget->show();
	m_mediaPlayer->play();

    ui.releaseNumber->setText( buildVersion );
    ui.buildDateTime->setText( buildDate );
    ui.applicationName->setText( componentName );
    ui.projectName->setText( 0 < headerArraySize ? headerArray[0] : "TransActive" );

    this->setWindowTitle( QString( "About " ) + componentName );

    this->setFixedSize ( this->size() );
    this->setAttribute( Qt::WA_DeleteOnClose, true );
    this->setWindowFlags( windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint | Qt::WindowContextHelpButtonHint );
}


void TransactiveAboutBoxDlg::on_okButton_clicked()
{
    close();
}


void TransactiveAboutBoxDlg::on_videoPlayer_finished()
{
    QTimer::singleShot( 0, this, SLOT(replay()) );
}


void TransactiveAboutBoxDlg::replay()
{
	m_mediaPlayer->setPosition(0);
	m_mediaPlayer->play();
}
