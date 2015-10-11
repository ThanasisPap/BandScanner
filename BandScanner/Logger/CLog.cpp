#include "CLog.h"

//String array to convert log levels to strings
char errMsg[5][10] = { "CRITICAL", "ERROR",  "WARN ", "Info ", "Debug" };

//Global definition of the logger
CLog *logit;

//////////////////////////////////////////////////////////////////////////
// Format string using sprintf function 

QString CLog::LogFormatString( QString Format, ... )
{
    va_list args;
    va_start( args, Format );
    m_tmpBuffer[0] = 0;
    vsnprintf( m_tmpBuffer, sizeof( m_tmpBuffer ), Format.toLocal8Bit().data(), args );
    va_end( args );

    return QString( m_tmpBuffer );
}

//////////////////////////////////////////////////////////////////////////
//Log formated strings

int CLog::LogFormat( const int Level, const QString Message, ... )
{
    va_list args;
    va_start( args, Message );
    m_tmpBuffer[0] = 0;
    vsnprintf( m_tmpBuffer, sizeof( m_tmpBuffer ), Message.toLocal8Bit().data(), args );
    va_end( args );

    QString qstr;
    qstr.sprintf("%s %s %s",
            ( m_bLogTime ) ? CurrentTime().toLocal8Bit().data(): "",
            errMsg[Level], m_tmpBuffer );

    m_pStore->WriteString( qstr );

    if( m_bAutoFlush == true )
        m_pStore->FlushData();

    return 0;
}

//////////////////////////////////////////////////////////////////////////
// redirector of simple strings

int CLog::LogString( long Level, const char *szMessage )
{
    return LogString( Level, QString( szMessage ) );
}

//////////////////////////////////////////////////////////////////////////
// Log String according to level  

int CLog::LogString( long Level, const QString &Message )
{
    if( Level > m_lLevel ) return 0;

    QString retString;

    retString = LogFormatString(
                m_strMessageFormat,
                ( m_bLogTime ) ? CurrentTime().toLocal8Bit().data(): "",
                errMsg[Level], Message.toLocal8Bit().data() );

    m_pStore->WriteString( retString );

    if( m_bAutoFlush == true )
        m_pStore->FlushData();

    return 0;
}

//////////////////////////////////////////////////////////////////////////
// log string without any formating and testing

int CLog::LogRawString( const QString &Message )
{
    m_pStore->WriteString( Message );

    if( m_bAutoFlush == true )
        m_pStore->FlushData();

    return 0;
}

//:> End of file
