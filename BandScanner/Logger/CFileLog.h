/********************************************************************\
	created:		2001/03/22
	created:		22:3:2001   20:20
	filename: 	c:\my projects\dualmagic\controlappl\include\cfilelog.h
	file path:	c:\my projects\dualmagic\controlappl\include
	file base:	cfilelog
	file ext:		h
	author:			Alex Kucherenko
	
	purpose:	
\********************************************************************/

#ifndef _LOG_SYSTEM_FILE_LOG_H_
#define _LOG_SYSTEM_FILE_LOG_H_

#include "CStoreLog.h"
#include "qtextstream.h"
#include "qfile.h"

#ifndef MAX_STRING_LENGTH
# define MAX_STRING_LENGTH 8192
#endif

class CFileLog : public IStoreLog
{
  public:
    CFileLog( const QString &FileName, long lSize = MAX_STRING_LENGTH );// throw ( QString );
    virtual ~CFileLog(  );

    virtual int FlushData();
    virtual int WriteString( const QString Message );
    virtual int SetBufferLimit( long lSize );

  private:
    QString        m_strFileName;
    QFile          m_file;
    //QTextStream    m_outStream;
    long           m_lBufSize;
    unsigned char  *m_pBuffer;
    long           m_lBufPos;
};

#endif /* _LOG_SYSTEM_FILE_LOG_H_ */
