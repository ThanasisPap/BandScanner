/*
 * Only file to be included in order to use logging
 */

#ifndef LOGDEFS_H
#define LOGDEFS_H

#include "CLog.h"
#include "CFuncLog.h"
#include "CWinLog.h"
#include "cdebuglog.h"

//Define debug levels
#ifndef LOG_INFO
# define LOG_DEBUG	 	4
# define LOG_INFO 		3
# define LOG_WARNING    2
# define LOG_ERROR		1
# define LOG_CRITICAL   0
# define LOG_MAX_LEVEL  4
#endif

//Logging should be used only through the following macros
#define LOGFUNC(x) CFuncLog log(logit, x); Q_UNUSED(log);
#define LOG_D(...) logit->LogFormat(LOG_DEBUG,    ##__VA_ARGS__)
#define LOG_I(...) logit->LogFormat(LOG_INFO,     ##__VA_ARGS__)
#define LOG_W(...) logit->LogFormat(LOG_WARNING,  ##__VA_ARGS__)
#define LOG_E(...) logit->LogFormat(LOG_ERROR,    ##__VA_ARGS__)
#define LOG_C(...) logit->LogFormat(LOG_CRITICAL, ##__VA_ARGS__)

#endif // LOGDEFS_H
