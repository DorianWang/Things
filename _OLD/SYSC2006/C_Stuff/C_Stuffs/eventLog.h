#ifndef EVENTLOG_H_INCLUDED
#define EVENTLOG_H_INCLUDED

#include "time.h"
#include <stdlib.h>
#include <stdio.h>
#include "system."

#define EVENT_LOG_NAME "STP_Log.txt"


extern bool LogCreated;      // keeps track whether the log file is created or not
extern FILE logFile;         // Log file is maintained. May not be the best...

void Log (char *message);    // logs a message to LOGFILE
void LogErr (char *message); // logs a message; execution is interrupted


bool LogCreated = false;

void Log (char *message)
{

	if (!LogCreated) {
		logFile = fopen(EVENT_LOG_NAME, "w");
		LogCreated = true;
	}

	if (logFile == NULL) {
		if (LogCreated)
			LogCreated = false;
		return;
	}
	else
	{
	   fputs()
		fputs(message, logFile);
		fputs("/n", logFile);
	}

}

void LogErr (char *message)
{
	Log(message);
	exit (EXIT_FAILURE);
}










