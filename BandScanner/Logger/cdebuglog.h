#ifndef CDEBUGLOG_H
#define CDEBUGLOG_H

#include "CStoreLog.h"
#include<QDebug>

class CDebugLog : public IStoreLog
{
public:
    CDebugLog();
    ~CDebugLog();

    virtual int FlushData();
    virtual int WriteString( const QString Message );
    virtual int SetBufferLimit( long lSize );

};

#endif // CDEBUGLOG_H
