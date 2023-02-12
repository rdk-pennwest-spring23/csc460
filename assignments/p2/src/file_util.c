/**
 * @file file_util.c
 * 
 * CSC 460 Language Translations
 * Group 2
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long (lon1150@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
 * 
 * @brief Program 2: Scanner
 */

#include "file_util.h"
#include "log_util.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>


/**
 * @brief Opens all relevant files for this project.
 * 
 * @param argc Number of command line arguments
 * @param argv List of command line arguments
 * @return int Success/Failure
 */
int open_files(int argc, char *argv[])
{
	int status = SUCCESS;

	// Check for input file name from command line args
	if (argc > 1)
	{
		strcpy(inputFileName, argv[1]);
		log_info("Detected input file name '%s' from command line.", inputFileName);
	} else {
		// Get an input file name from the user
		printf("Please enter an input file name:\n>> ");
		scanf("%s", inputFileName);
	}
	
	// Check for Output File Name from command line args
	if (argc > 2)
	{
		strcpy(outputFileName, argv[2]);
		log_info("Detected output file name '%s' from command line.", inputFileName);
	} else {
		// Get an input file name from the user
		printf("Please enter an output file name:\n>> ");
		scanf("%s", outputFileName);
	}

	// Check for a valid input file
	int validInput = 0;
	while (status == SUCCESS && !validInput)
	{
		// Check if the input file name has an extension
		char *extIndex = strrchr(inputFileName, '.');
		if (!extIndex || extIndex == inputFileName)
			strcat(inputFileName, ".in");

		// Check if the input file name exists as a file
		if (file_exists(inputFileName))
		{
			log_info("Input file '%s' exists, using for input file.", inputFileName);
			validInput = 1;
		} else {
			log_info("Input file '%s' does not exist, requesting new file name from user.", inputFileName);
			
			// Get new name from user
			printf("Please enter an input file name (q to quit):\n>> ");
			scanf("%s", inputFileName);

			// Check if the user wants to quit
			if (inputFileName[0] == 'q')
				status = FAILURE;
		}
	}

	// Check for a valid output file
	int validOutput = 0;
	while(status == SUCCESS && !validOutput)
	{
		// Check if the ouput file has an extension
		char *extIndex = strrchr(outputFileName, '.');
		if (!extIndex || extIndex == outputFileName)
			strcat(outputFileName, ".out");

		// Check if the output file already exists
		if (file_exists(outputFileName))
		{
			log_info("Output file '%s' already exists, requesting choice from user for how to proceed.", outputFileName);

			// Get user option for how to proceed
			printf("\nOutput file '%s' already exists. Please choose from one of the below options.\n", outputFileName);
			printf("\t1) Overwrite the output file.\n"
				   "\t2) Backup the output file.\n"
				   "\t3) Enter a new output file name.\n"
				   "\t4) Quit.\n>> ");
			char choice[50];
			scanf("%s", choice);

			switch(choice[0])
			{
				case '1': // Overwrite
					log_info("User chose overwrite the output file.");
					delete_file(outputFileName);
					break;

				case '2': // Backup
					log_info("User chose to backup the output file.");
					backup_file(outputFileName);
					delete_file(outputFileName);
					break;

				case '3': // Enter new name
					log_info("User chose to enter a new output file name.");
					printf("Enter a new file name:\n>> ");
					scanf("%s", outputFileName);
					break;

				case '4': // Quit
					log_info("User chose to terminate program.");
					status = FAILURE;
					break;

				default:
					log_info("Invalid input from user.");
			}

		} else {
			// If the file does not exist, use it as the output file.
			log_info("Output file '%s' is designated as the output file.", outputFileName);
			validOutput = 1;
		}
	}

	if (status == SUCCESS)
	{
		// Extract the generic file name from output file name
		char genericFileName[MAX_FILE_NAME_SIZE];
		strncpy(genericFileName, outputFileName, strlen(outputFileName)-4);

		// Generate the listing file
		strcpy(listingsFileName, genericFileName);
		strcat(listingsFileName, ".lis");
		generate_listings_file(listingsFileName);

		// Generate the temporary file
		time_t now;
		time(&now);
		char *date = ctime(&now);
		date[strlen(date)-1] = '\0';
		int i;
		for ( i = 0; date[i]; i++)
			if (date[i] == ':')
				date[i] = '-';
		strcpy(tempFileName, genericFileName);
		strcat(tempFileName, "-");
		strcat(tempFileName, date);
		strcat(tempFileName, ".tmp");
		generate_temporary_file(tempFileName);

		// Open the files
		inputFilePtr = fopen(inputFileName, "r");
		outputFilePtr = fopen(outputFileName, "w");
		listingFilePtr = fopen(listingsFileName, "w");
		tempFilePtr = fopen(tempFileName, "w");
	}

	// Return the status of the file open operation
	return status;
} // open_files


/**
 * @brief Closes any remaining open files, deletes the temporary file.
 * 
 * @return int Success/Failure
 */
int close_files()
{
	if (inputFilePtr != NULL)
		fclose(inputFilePtr);
	if (outputFilePtr != NULL)
		fclose(outputFilePtr);
	if (listingFilePtr != NULL)
		fclose(listingFilePtr);
	if (tempFilePtr != NULL)
		fclose(tempFilePtr);

	// Delete the temporary file.
	// delete_file(tempFileName);

	return SUCCESS;
}


/**
 * @brief Handles any extras related to generating the listings file.
 * 
 * @param listFilePtr 
 * @return int Success/Failure
 */
int generate_listings_file(char* lisFileName)
{
	if ( file_exists(lisFileName) )
	{
		backup_file(lisFileName);
		delete_file(lisFileName);
	}
	return SUCCESS;
}


/**
 * @brief Handles any extras when generating the temporary file.
 * 
 * @param tempFileName Name of the temporary file.
 * @return int Success/Failure
 */
int generate_temporary_file(char* genericFileName)
{
	return SUCCESS;
}


/**
 * @brief Creates a backup of a file. 
 * 
 * @param fileName Name of the file to backup.
 * @return int Success/Failure
 */
int backup_file(char* fileName)
{
	// Create the name of the backup file
	char backupFileName[MAX_FILE_NAME_SIZE];
	time_t now;
	time(&now);
	char *date = ctime(&now);
	date[strlen(date)-1] = '\0';
	int i;
	for ( i = 0; date[i]; i++)
		if (date[i] == ':')
			date[i] = '-';

	// Extract the name from the given fileName
	strcpy(backupFileName, fileName);
	backupFileName[strlen(backupFileName) - 4] = '-';

	// Build the name of the backup file
	strcat(backupFileName, "-");
	strcat(backupFileName, date);
	strcat(backupFileName, ".bak");

	// Open the files
	FILE *source = fopen(fileName, "r");
	FILE *dest = fopen(backupFileName, "w");
	
	// Copy the data
	char c = fgetc(source);
	while ( c != EOF)
	{
		fputc(c, dest);
		c = getc(source);
	}

	// Close the files
	fclose(source);
	fclose(dest);

	return SUCCESS;
}


/**
 * @brief Attempts to delete the file.
 * 
 * @param fileName Name of the file to delete.
 * @return int Success/Failure
 */
int delete_file(char* fileName)
{
	int status;
	
	if (remove(fileName) == 0)
	{
		log_info("Deleted file '%s' successfully.", fileName);
		status = SUCCESS;
	}
	else
	{
		log_error("Unable to delete file '%s' successfully.", fileName);
		status = FAILURE;
	}

	return status;
} // delete_file


/**
 * @brief Writes to the specified file using the format and optional arguments.
 * 
 * @param file File to write to.
 * @param fmt Format for output.
 * @param ... Optional Arguments
 * @return int Success/Failure
 */
int write_to_file(FILE* file, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(file, fmt, args);
	va_end(args);

	return SUCCESS;
} // write_to_file


/**
 * @brief Checks if a file exists
 * 
 * @param fileName The name of the file to check.
 * @return int 1 if exists, 0 if not.
 */
int file_exists(char* fileName)
{
	FILE* filePtr = fopen(fileName, "r");
	int status;
	if ( status = (filePtr != NULL) )
		fclose(filePtr);
	return status;
}

// EOF