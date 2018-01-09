#include "TransactiveAboutBoxDlg.h"
#include <QTimer>


TransactiveAboutBoxDlg::TransactiveAboutBoxDlg( const char buildVersion[],
                                                const char buildDate[],
                                                const char componentName[],
                                                const char componentVersion[],
                                                const int headerArraySize,
                                                char* headerArray[] )
    : m_buildVersion( buildVersion ),
      m_buildDate( buildDate ),
      m_componentName( componentName ),
      m_componentVersion( componentVersion ),
      m_headerArraySize( headerArraySize ),
      m_headerArray( headerArray )
{
    ui.setupUi( this );
    ui.videoPlayer->load( Phonon::MediaSource( ":/logo.avi" ) );
    ui.videoPlayer->play();
    setFixedSize ( this->size() );
    setAttribute( Qt::WA_DeleteOnClose, true );
    setWindowFlags( windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint | Qt::WindowContextHelpButtonHint );
    languageChange();
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
    ui.videoPlayer->seek( 0 );
    ui.videoPlayer->play();
}


void TransactiveAboutBoxDlg::languageChange()
{
    ui.retranslateUi( this );
    ui.releaseNumber->setText( tr(m_buildVersion) );
    ui.buildDateTime->setText( tr(m_buildDate) );
    ui.applicationName->setText( tr(m_componentName) );
    ui.projectName->setText( 0 < m_headerArraySize ? tr(m_headerArray[0]) : tr("TransActive") );
    this->setWindowTitle( tr("About ") + tr(m_componentName) );
}
