/********************************************************************\
  created:    2001/03/22
  created:    22:3:2001   20:20
  filename:   c:\my projects\dualmagic\controlappl\include\cfunclog.h
  file path:  c:\my projects\dualmagic\controlappl\include
  file base:  cfunclog
  file ext:   h
  author:     Alex Kucherenko
  
  purpose:  
\********************************************************************/

#ifndef _LOG_SYSTEM_FUNCTION_LOG_H_
#define _LOG_SYSTEM_FUNCTION_LOG_H_

#include "LogDefs.h"
#include <stdio.h>
#include <QString>
#include <cstdio>

class CFuncLog
{
  public:
//    CFuncLog( CLog &pLog, const QString FunctionName );
//    CFuncLog( CLog *pLog, const QString FunctionName );
    CFuncLog( CLog *pLog, const char *FunctionName );
    CFuncLog( CLog *pLog, char *szFunctionName );
    ~CFuncLog(  );

    int LogString( long Level, const QString Message );
    QString FormatString( const QString Format, ... );

    CFuncLog &operator<< ( int iValue );
    CFuncLog &operator<< ( long lValue );
    CFuncLog &operator<< ( double dValue );
    CFuncLog &operator<< ( const QString &strValue );
    CFuncLog &operator<< ( char *cszValue );
    
    char *endl( void )
    {
      return (char*)"\n";
    }
    
  private:
    QString       m_strFuncName;
    CLog          *m_pLog;
    char          m_tmpBuf[ MAX_STRING_LENGTH ];
};

#endif /* _LOG_SYSTEM_FUNCTION_LOG_H_ */
