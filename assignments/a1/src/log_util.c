/**
 * @file log_util.c
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long ()
 * @author Noelle Nieves ()
 * @brief Logging utilities.
 * 
 */

#include "log_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static FILE* logFile;
static int logStatus = LOG_TO_FILE_FALSE;


void set_log_to_file()
{
    if (logStatus == LOG_TO_FILE_FALSE)
    {
        char logFileName[32];
        time_t now;
        time(&now);
        struct tm *curTime = localtime(&now);

        snprintf(logFileName, sizeof(logFileName), "log-%02d:%02d:%02d.txt",
            curTime->tm_hour, 
            curTime->tm_min, 
            curTime->tm_sec);

        logFile = fopen(logFileName, "a");
        logStatus = LOG_TO_FILE_TRUE;
        log_message("LOG", "Logging to file enabled.");
    }
}


void end_log_to_file()
{
    if (logStatus == LOG_TO_FILE_TRUE)
    {
        fclose(logFile);
        logStatus = LOG_TO_FILE_FALSE;
        log_message("LOG", "Logging to file disabled.");
    }
}


void log_message(char *sender, char *message)
{
    // Print the time
    time_t now;
    time(&now);
    struct tm *curTime = localtime(&now);

    // Setup the message
    char printMesssage[strlen(message) + strlen(sender) + 32];
    snprintf(printMesssage, sizeof(printMesssage), "[%02d:%02d:%02d] [%s] %s\n", 
        curTime->tm_hour, 
        curTime->tm_min, 
        curTime->tm_sec,
        sender,
        message
    );

    // Print message to console
    printf("%s", printMesssage);

    // Print message to log file
    if (logStatus == LOG_TO_FILE_TRUE)
        fprintf(logFile, printMesssage);
}
