#include "cdebuglog.h"

CDebugLog::CDebugLog()
{

}

CDebugLog::~CDebugLog()
{

}

int CDebugLog::FlushData()
{
    return 0;
}

int CDebugLog::WriteString( const QString Message )
{
    qDebug() << Message;
    return 0;
}

int CDebugLog::SetBufferLimit( long lSize )
{
    //Avoid unused lSize warning
    return lSize;
}
