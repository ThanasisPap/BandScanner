
#include "CStoreLog.h"
#include "CWinLog.h"

//////////////////////////////////////////////////////////////////////////
//  

CWinLog::CWinLog( QWidget *parent, long MaxLines /*= 1000*/ )
    : m_lMaxLines( MaxLines )
    , m_bFirstString( true )
{
    CreateLogWindow(parent);
    //QObject::connect(this, SIGNAL(sendMessage(void)), this, SLOT(UsersEndMessage(void)));
}

//////////////////////////////////////////////////////////////////////////
//  

CWinLog::~CWinLog(  )
{
    //CSyncLock lock( &m_CriticalSection );
    //Close and delete window
    window->close();
    delete window;
}

//////////////////////////////////////////////////////////////////////////
//  

int CWinLog::FlushData()
{
    return 0;
}

//////////////////////////////////////////////////////////////////////////
//  

int CWinLog::WriteString( const QString Message )
{
    if( m_bFirstString )
    {
        window->show();
        m_bFirstString = false;
    }

    emit(window->newMessage(Message));

    return 0;
}

//////////////////////////////////////////////////////////////////////////
//  

int CWinLog::SetBufferLimit( long lSize )
{
    m_lMaxLines = lSize;
    return 0;
}

//////////////////////////////////////////////////////////////////////////
//  

void CWinLog::CreateLogWindow( QWidget *parent )
{
    window = new LogDialog(parent);
    window->resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    window->setWindowTitle("Logger");
    window->move(0,0);
}
