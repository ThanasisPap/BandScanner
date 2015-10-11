/********************************************************************\
	created:		2001/03/22
	created:		22:3:2001   20:20
	filename: 	c:\my projects\dualmagic\controlappl\include\clog.h
	file path:	c:\my projects\dualmagic\controlappl\include
	file base:	clog
	file ext:		h
	author:			Alex Kucherenko
	
	purpose:	
\********************************************************************/

#ifndef _LOG_SYSTEM_LOG_MAIN_CLASS_H_
#define _LOG_SYSTEM_LOG_MAIN_CLASS_H_

#include "CStoreLog.h"
//#include "LogDefs.h"
#include <QString>
#include <QDateTime>
#include <stdio.h>

#ifndef MAX_STRING_LENGTH
# define MAX_STRING_LENGTH 256
#endif


//#define DEF_LONG_FORMAT_STR "Process : %#06x : Thread : %#04x : Time : %s : Level : %12s : Message : %s\n"
#define DEF_SHORT_FORMAT_STR "%#06x : %#04x : %s : %12s : %s\n"
#define DEF_TIME_LONG_STR "%02u:%02u:%02u ms:%03u"
#define DEF_TIME_SHORT_STR "%02u:%02u:%02u"
#define DEF_JUST_MESSAGE_STR "%s %s %s"

class CLog
{
  public:
    // constructor 
    CLog( IStoreLog *pStoreLog, long maxLevel, bool bParentToStore = true ) : 
      m_strTimeFormat( DEF_TIME_LONG_STR ),
      m_strMessageFormat( DEF_JUST_MESSAGE_STR ),
      m_bAutoFlush( true ),             // Store Log to disk immediately
      m_bIsParent( bParentToStore ),    // if true - then we destroy StoreClass
      m_lLevel( maxLevel ),
      m_pStore( pStoreLog ),            // our store class
      m_bLogTime( true )                // Add time string into log
    {
      m_tmpBuffer[0] = 0;
    }
    
    // destructor
    virtual ~CLog(  )
    {
      m_pStore->FlushData();

      if( m_bIsParent == true )
        delete m_pStore, m_pStore = NULL;
    }
  
    // Log function ( most used )
    inline QString LogFormatString( QString Format, ... );
    int LogString( long Level, const QString &Message );
    int LogString( long Level, const char *szMessage );
    int LogRawString( const QString &Message );
    int LogFormat( const int Level, const QString Message, ... );

    // AutoFlush
    inline bool GetAutoFlush( void ) const 
    {
      return m_bAutoFlush;
    }

    inline void SetAutoFlush( bool state )
    {
      m_bAutoFlush = state;
    }
    
    // TimeFormat
    inline QString GetTimeFormat( void ) const
    {
      return m_strTimeFormat;
    }

    inline void SetTimeFormat( const QString &format )
    {
      m_strTimeFormat = format;
    }

    // Message format
    inline const QString &GetMessageFormat( void )
    {
      return m_strMessageFormat;
    }

    inline void SetMessageFormat( const QString &format )
    {
      m_strMessageFormat = format;
    }

    // Write Time to Log
    inline bool GetLogTime( void ) const
    {
      return  m_bLogTime;
    }

    inline void SetLogTime( bool state )
    {
      m_bLogTime = state;
    }

  protected:

    virtual const QString &LevelText( long /*Level*/ )
    {
      m_strTempBuf = " LOG NOT IMPLEMENTED CATEGORY : ";
      return m_strTempBuf;
    }
    
    inline const QString CurrentTime(  )
    {
        m_strTempBuf = QTime::currentTime().toString();
        return m_strTempBuf;
    }

private:

    QString     m_strTimeFormat;
    QString     m_strMessageFormat;
    bool        m_bAutoFlush;
    bool        m_bIsParent;
    long        m_lLevel;
    IStoreLog   *m_pStore;
    bool        m_bLogTime;

    char        m_tmpBuffer[ MAX_STRING_LENGTH ];
    QString     m_strTempBuf;
};

extern CLog *logit;

#endif /* _LOG_SYSTEM_LOG_MAIN_CLASS_H_ */
