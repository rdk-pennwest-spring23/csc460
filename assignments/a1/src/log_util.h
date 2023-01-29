/**
 * @file log_util.h
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long ()
 * @author Noelle Nieves ()
 * @brief Logging utilities.
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
void log_message(char *sender, char *message);

#endif