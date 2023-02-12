/**
 * @file log_util.h
 * @Group 2
 * @Program Program 1: File Opener
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long (lon1150@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
 * @class CSC 460 Language Translations
 * @brief A set of logging functions from https://stackoverflow.com/a/23446001
 * 
 */

#ifndef LOG_UTIL_H
#define LOG_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

/**
 * @brief Log the formatted message to the console
 * 
 * @param sender The sender's ID
 * @param message Message to be logged
 */
void log_info(const char* message, ...); 
void log_error(const char* message, ...); 
void log_debug(const char* message, ...);

#endif