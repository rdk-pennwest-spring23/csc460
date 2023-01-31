/**
 * @file log_util.c
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long (lon1150@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
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

        snprintf(logFileName, sizeof(logFileName), "log-%02d-%02d-%02d.txt",
            curTime->tm_hour, 
            curTime->tm_min, 
            curTime->tm_sec);

        logFile = fopen(logFileName, "a");
        logStatus = LOG_TO_FILE_TRUE;
        log_message("Logging to file enabled.");
        fprintf(logFile, "%s", "");
    }
}


void end_log_to_file()
{
    if (logStatus == LOG_TO_FILE_TRUE)
    {
        fclose(logFile);
        logStatus = LOG_TO_FILE_FALSE;
        log_message("Logging to file disabled.");
    }
}

void log_format(const char* tag, const char* message, va_list args) 
{   
    time_t now;     
    time(&now);     
    char * date =ctime(&now);   
    date[strlen(date) - 1] = '\0';  
    printf("[%s] [%s]", date, tag);  
    vprintf(message, args);
    printf("\n"); 
}

void log_error(const char* message, ...) 
{  
    va_list args;   
    va_start(args, message);    
    log_format("ERROR", message, args);     
    va_end(args); 
}

void log_message(const char* message, ...) 
{   
    va_list args;   
    va_start(args, message);    
    log_format("INFO", message, args);  
    va_end(args); 
}

void log_debug(const char* message, ...) 
{
    va_list args;   
    va_start(args, message);    
    log_format("DEBUG", message, args);     
    va_end(args);
}