/**
 * @file log_util.h
 * @Group 2
 * @Program Program 1: File Opener
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long (lon1150@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
 * @brief 
 * 
 */

#ifndef LOGGING_H
#define LOGGING_H

#define LOG_TO_FILE_FALSE 0
#define LOG_TO_FILE_TRUE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

/**
 * @brief Sets up the logging module
 * 
 */
void set_log_to_file();

/**
 * @brief End Logging, close the logging file.
 * 
 */
void end_log_to_file();

/**
 * @brief Log the formatted message to the console
 * 
 * @param sender The sender's ID
 * @param message Message to be logged
 */
void log_message(const char* message, ...); 

void log_error(const char* message, ...); 
void log_debug(const char* message, ...);

#endif