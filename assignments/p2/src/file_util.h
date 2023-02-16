/**
 * @file file_util.h
 * 
 * CSC 460 Language Translations
 * Group 2
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long (lon1150@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
 * 
 * @brief Program 2: Scanner
 */

#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#define MAX_FILE_NAME_SIZE 255

#define SUCCESS 1
#define FAILURE 0

#include <stdio.h>
#include <stdlib.h>

/* File Pointers */
FILE *inputFilePtr, *outputFilePtr, *listingFilePtr, *tempFilePtr;

char inputFileName[MAX_FILE_NAME_SIZE];
char outputFileName[MAX_FILE_NAME_SIZE];
char listingsFileName[MAX_FILE_NAME_SIZE];
char tempFileName[MAX_FILE_NAME_SIZE];

int backup;

/**
 * @brief Opens all relevant files for 
 * 
 * @param argc Number of command line arguments
 * @param argv List of command line arguments
 * @return int Success/Failure
 */
int open_files(int argc, char *argv[]);

/**
 * @brief Closes any remaining open files.
 * 
 * @return int Success/Failure
 */
int close_files();

/**
 * @brief 
 * 
 * @param listFilePtr 
 * @return int Success/Failure
 */
int generate_listings_file(char* listFilePtr);

/**
 * @brief 
 * 
 * @param tempFileName Name of the temporary file.
 * @return int Success/Failure
 */
int generate_temporary_file(char* tempFileName);

/**
 * @brief Creates a backup of a file. 
 * 
 * @param fileName Name of the file to backup.
 * @return int Success/Failure
 */
int backup_file(char* fileName);

/**
 * @brief Attempts to delete the file.
 * 
 * @param fileName Name of the file to delete.
 * @return int Success/Failure
 */
int delete_file(char* fileName);

/**
 * @brief Writes to the specified file using the format and optional arguments.
 * 
 * @param file File to write to.
 * @param fmt Format for output.
 * @param ... Optional Arguments
 * @return int Success/Failure
 */
int write_to_file(FILE* file, const char* fmt, ...);

/**
 * @brief Checks if a file exists
 * 
 * @param fileName The name of the file to check.
 * @return int 1 if exists, 0 if not.
 */
int file_exists(char* fileName);


#endif
// EOF