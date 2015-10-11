/********************************************************************\
	created:		2001/03/22
	created:		22:3:2001   20:21
	filename: 	c:\my projects\dualmagic\controlappl\include\cstorelog.h
	file path:	c:\my projects\dualmagic\controlappl\include
	file base:	cstorelog
	file ext:		h
	author:			Alex Kucherenko
	
	purpose:	
\********************************************************************/

#ifndef _LOG_SYS_STORE_LOG_H_
#define _LOG_SYS_STORE_LOG_H_

#include <QString>

//////////////////////////////////////////////////////////////////////////
// Abstract class set three default function which must support any child.
// Any child must support buffered and non-buffered store 

class IStoreLog 
{
	public:
    virtual ~IStoreLog( void ){}
		virtual int FlushData() = 0;
        virtual int WriteString( const QString Message ) = 0;
		virtual int SetBufferLimit( long lSize ) = 0;
};

#endif /* _LOG_SYS_STORE_LOG_H_ */
