// Global constants, #includes //

#ifndef FILE_UTIL
#define FILE_UTIL

#define FILE_EXISTS 1
#define FILE_OPEN 2

#include <stdio.h>
#include <stdlib.h>


struct fileinfo
{
    char* name;
    char* ext;
    int status;
    FILE* file;
};

// CONSTANTS //


// FUNCTION DECLARATION //

/**
 * @brief Checks if there is an extension at the end of the file.
 * 
 * @param fileName 
 * @return char* 
 */
int check_ext(char* fileName);

/**
 * @brief Checks if a file exists
 * 
 * @param fileName filename
 * @return int 0 - False, 1 - True
 */
int check_file_exists(char* fileName);

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


#endif
