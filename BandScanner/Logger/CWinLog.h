/********************************************************************\
    created:		2001/03/22
    created:		22:3:2001   20:21
    filename: 	c:\my projects\dualmagic\controlappl\include\cwinlog.h
    file path:	c:\my projects\dualmagic\controlappl\include
    file base:	cwinlog
    file ext:		h
    author:			Alex Kucherenko

    purpose:
\********************************************************************/

#ifndef _LOG_SYSTEM_WIN_LOG_H_
#define _LOG_SYSTEM_WIN_LOG_H_

#include "CStoreLog.h"

#include <vector>
#include "logdialog.h"
#include <QWidget>

typedef std::vector<QString> TStrArray;
typedef TStrArray::iterator TStrIter;

#define WINDOW_HEIGHT 200
#define WINDOW_WIDTH  300

//////////////////////////////////////////////////////////////////////////
//  

class CWinLog : public IStoreLog
{
public:
    CWinLog( QWidget *parent = NULL, long MaxLines = 1000 );
    virtual ~CWinLog(  );

    virtual int FlushData();
    virtual int WriteString( const QString Message );
    virtual int SetBufferLimit( long lSize );

protected:
    inline  void    CreateLogWindow( QWidget *parent );
    //static  LRESULT APIENTRY localWndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );

private:

    unsigned long     m_lMaxLines;
    bool              m_bFirstString;
    TStrArray         m_CurrMessage;
    long              m_lBufferLimit;
    LogDialog         *window;
};

#endif /* _LOG_SYSTEM_WIN_LOG_H_ */
