/**
 * @file file_util.h
 * @Group 2
 * @Program Program 1: File Opener
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long (lon1150@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
 * @class CSC 460 Language Translations
 * @brief 
 * 
 */
// Global constants, #includes //

#ifndef FILE_UTIL
#define FILE_UTIL

#define MAX_FILE_NAME_SIZE 255

#include <stdio.h>
#include <stdlib.h>


enum fileStatus {
    VALID_FILE_NAME = 0,
    INVALID_FILE_NAME = 1,
    FILE_DOES_NOT_EXIST = 2,
    FILE_EXISTS = 4,
    FILE_OPEN = 8,
    FILE_CLOSED = 16,
};


/**
 * @brief Checks if there is an extension at the end of the file.
 * 
 * @param fileName 
 * @return char* 
 */
int has_extension(char* fileName);

/**
 * @brief Checks if a file exists
 * 
 * @param fileName filename
 * @return int 0 - False, 1 - True
 */
enum fileStatus file_exists(char* fileName);

/**
 * @brief Create a copy of the specified file, appending the '.bak' extension.
 * 
 * @param fileName The file to backup.
 */
void backup_file(char* fileName);

/**
 * @brief Delete the specified file.
 * 
 * @param fileName The file name.
 */
void delete_file(char* fileName);

/**
 * @brief Copies all data from the source file to the dest file, byte by byte.
 * 
 * @param dest The destination file
 * @param source The source file
 * 
 */
void copy_file(FILE* dest, FILE* source);

/**
 * @brief Opens a file 
 * 
 */
FILE* open_file(char* fileName, char* c);

/**
 * @brief Get the valid filename as user input
 * 
 * @param fileName the output file name
 */
enum fileStatus validate_filename(char* fileName, char* ext);

/**
 * @brief create temporary file name
 * 
 * @param temp file variable
 */
void create_temp_file(char* tempFileName);


#endif
