/**
 * @file log_util.c
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long (lon1150@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
 * @class CSC 460 Language Translations
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

void log_format(const char* tag, const char* message, va_list args) 
{   
    time_t now;     
    time(&now);     
    char * date =ctime(&now);   
    date[strlen(date) - 1] = '\0';  
    printf("[%s] [%s] ", date, tag);  
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

void log_info(const char* message, ...) 
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