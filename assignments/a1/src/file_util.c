/**
 * @file file_util.c
 * @Group 2
 * @Program Program 1: File Opener
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long (lon1150@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
* @class CSC 460 Language Translations
 * @brief 
 * 
 */
// Contains file open routine, support routines //

#include "file_util.h"
#include "log_util.h"
#include <stdio.h>
#include <string.h>
#include <time.h>


/**
 * @brief Checks if there is an extension at the end of the file.
 * 
 * @param fileName 
 * @return char* 
 */
int has_extension(char* fileName)
{
	int hasExt = 0;
	for (int i = 0; i < strlen(fileName) && !hasExt; i++)
		if (fileName[i] == '.')
			hasExt = 1;
	return hasExt;
}

/**
 * @brief Checks if a file exists
 * 
 * @param fileName filename
 * @return int 0 - False, 1 - True
 */
enum fileStatus file_exists(char* fileName)
{
    FILE *fp = fopen(fileName, "r");
    enum fileStatus status = FILE_DOES_NOT_EXIST;
    if (fp != NULL)
    {
        status = FILE_EXISTS;
        fclose(fp); // close the file
    }
    return status;
}

/**
 * @brief Create a copy of the specified file, appending the '.bak' extension.
 * 
 * @param fileName The file to backup.
 */
void backup_file(char* fileName)
{
	log_info("Creating a backup of the file %s.", fileName);

	char* backup_name = malloc(sizeof(char) * (strlen(fileName) + 4));

	strcpy(backup_name, fileName);
	strcat(backup_name, ".bak");
	
	// If the backup file already exists, create a backup of the backup with timestamp.
	if (file_exists(backup_name) == FILE_EXISTS)
	{
		char backup_backup[MAX_FILE_NAME_SIZE];
		strcpy(backup_backup, backup_name);

		char datetime[100];
		time_t now;
		time(&now);
		struct tm *curTime = localtime(&now);
		
		snprintf(datetime, MAX_FILE_NAME_SIZE, "-%02d-%02d-%02d-%02d-%02d-%02d.bak",
			curTime->tm_hour, 
			curTime->tm_min, 
			curTime->tm_sec,
			curTime->tm_mday,          	// get day of month (1-31)
			curTime->tm_mon + 1,      	// get month of year (0-11)
			curTime->tm_year + 1900   	// get year since 1900
		);

		strcat(backup_backup, datetime);

		FILE* backup = fopen(backup_name, "r");
		FILE* bb_file = fopen(backup_backup, "w");
		copy_file(bb_file, backup);
		fclose(backup);
		fclose(bb_file);
	}

	FILE* file = fopen(fileName, "r");
	FILE* backup = fopen(backup_name, "w");

	copy_file(backup, file);

	fclose(file);
	fclose(backup);

	log_info("Backup created in file %s.", backup_name);

	free(backup_name);
}

/**
 * @brief Delete the specified file.
 * 
 * @param fileName The file name.
 */
void delete_file(char* fileName)
{
	if (remove(fileName) == 0)
		log_info("Deleted file successfully.");
	else
		log_info("Unable to delete file.");
}

/**
 * @brief Copies all data from the source file to the dest file, byte by byte.
 * 
 * @param dest The destination file
 * @param source The source file
 * 
 */
void copy_file(FILE* dest, FILE* source)
{
	char ch = fgetc(source);
	while (ch != EOF)
	{
		fputc(ch, dest);
		ch = fgetc(source);
	}
	rewind(source);
}

/**
 * @brief Opens a file 
 * 
 */
FILE* open_file(char* fileName, char* c){
	FILE *fp = fopen(fileName, c);

	if(NULL != fp){
		log_info("Opened %s.", fileName);
	}
	else{
		log_info("Unable to open %s.", fileName);
	}

	return fp;
}

/**
 * @brief Get the valid filename as user input
 * 
 * @param fileName the output file name
 */
enum fileStatus validate_filename(char* fileName, char* ext)
{
	enum fileStatus status = VALID_FILE_NAME;
	if (strlen(fileName) == 0)
	{
		status = INVALID_FILE_NAME;
	}

	// Check if the input has a specified extension
	if ((status == VALID_FILE_NAME) && !has_extension(fileName))
	{
		strcat(fileName, ext);
	}

	// If it's a valid file name, copy it to fileName check if it exists.
	if (status == VALID_FILE_NAME)
		status = file_exists(fileName);

	return status;

}

/**
 * @brief create temporary file
 * 
 * @param temp file name variable pointer
 */
void create_temp_file(char* tempFileName){
	time_t now;
    time(&now);
    struct tm *curTime = localtime(&now);
	
	snprintf(tempFileName, MAX_FILE_NAME_SIZE, "temp-%02d-%02d-%02d-%02d-%02d-%02d.tmp",
		curTime->tm_hour, 
		curTime->tm_min, 
		curTime->tm_sec,
		curTime->tm_mday,          	// get day of month (1-31)
    	curTime->tm_mon + 1,      	// get month of year (0-11)
    	curTime->tm_year + 1900   	// get year since 1900
	);
}