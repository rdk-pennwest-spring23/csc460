/**
 * @file main.c
 * @Group 2
 * @Program Program 1: File Opener
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long (lon1150@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
 * @class CSC 460 Language Translations
 * @brief 
 * 
 */

#include "file_util.h"
#include "log_util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int get_new_file_name(char* fileName, char* extension, int fileCanExist, enum fileStatus fileStatus);

int main(int argc, char *argv[])
{
	int quit = 0;
	enum fileStatus fileStatus = VALID_FILE_NAME;

	char inputFileName[MAX_FILE_NAME_SIZE], outputFileName[MAX_FILE_NAME_SIZE], listingFileName[MAX_FILE_NAME_SIZE];
	inputFileName[0] = '\0';
	outputFileName[0] = '\0';
	listingFileName[0] = '\0';


	// Check if there are command line arguments input
	if (argc > 1) 
	{
		strcpy(inputFileName,argv[1]);
		log_info("Input file %s specified on command line.", inputFileName);
		if (argc > 2)
		{
			strcpy(outputFileName, argv[2]);
			log_info("Output file %s specified on command line.", outputFileName);
		}
	}

	// Get a valid input file name
	int validInput = 0;
	while (!quit && !validInput)
	{
		log_info("Validating input file name.");
		quit = get_new_file_name(inputFileName, ".in", 1, fileStatus);
		validInput = (fileStatus != INVALID_FILE_NAME);
		if (!quit && (fileStatus == INVALID_FILE_NAME))
		{
			log_error("Invalid input file name given: %s", inputFileName);
			inputFileName[0] = '\0';
		}
	}

	// Check for a valid ouput file
	int validOutput = 0;
	while (!quit && !validOutput)
	{
		log_info("Validating output file name.");
		int status;
		status = get_new_file_name(outputFileName, ".out", 0, fileStatus);
		validOutput = (fileStatus != INVALID_FILE_NAME);
		quit = (status == 1);
		if ((fileStatus == INVALID_FILE_NAME) || (fileStatus == FILE_EXISTS))
		{
			outputFileName[0] = '\0';
		}
		if (status == 2)
		{
			strcpy(outputFileName, inputFileName);
			strcat(outputFileName, ".out");
		}
	}

	// Get a valid listings file name
	int validListings = 0;
	while (!quit && !validListings)
	{
		log_info("Validating listings file.");
		strcpy(listingFileName, outputFileName);
		strcat(listingFileName, ".lis");
		quit = get_new_file_name(listingFileName, ".lis", 0, fileStatus);
		validListings = (fileStatus != INVALID_FILE_NAME);
	}
	
	//if user did not quit
	if(!quit && validInput && validOutput && validListings){

		FILE* inputFile = open_file(inputFileName,"r");
		FILE* outputFile = open_file(outputFileName,"w");
		FILE* listingFile = open_file(listingFileName,"w");

		// Open the temp file
		char tempFileName[MAX_FILE_NAME_SIZE];
		create_temp_file(tempFileName);
		FILE *tempFile = open_file(tempFileName, "w");

		// Check files were opened.
		if ((inputFile != NULL) && (outputFile != NULL) && (listingFile != NULL) && (tempFile != NULL))
		{
			log_info("Copying data.");
				// Read character from input file and write them to output files
			copy_file(outputFile, inputFile);
			copy_file(listingFile, inputFile);
			copy_file(tempFile, inputFile);

			//close files
			log_info("Closing files.");
			fclose(inputFile);
			fclose(outputFile);
			fclose(listingFile);
			fclose(tempFile);

			// Delete temp file
			// delete_file(tempFileName);
		}
		
	}

	// Clean up.
	return 0;
}

/**
 * @brief Get a new valid file name from the user
 * 
 * @param fileName The name of the file
 * @return int 1 on error, 0 on success
 */
int get_new_file_name(char* fileName, char* extension, int fileCanExist, enum fileStatus fileStatus)
{
	int status = 0, validFile = 0;

	while ((status != 1) && !validFile)
	{
		// Get a new name from the user if there is no name
		if (strlen(fileName) == 0)
		{
			printf("Please enter file name ('q' to quit): \n>> ");
			scanf("%s", fileName);

			// Check if the user wants to quit
			if (strcmp(fileName, "q") == 0)
			{
				status = 1;
				log_info("User opted to quit.");
			}
		}

		// Validate the input file name
		fileStatus = validate_filename(fileName, extension);

		// If the file cannot previously exist, ask user to choose an option to proceed
		if ((status != 1) && !fileCanExist && (fileStatus == FILE_EXISTS))
		{
			log_info("File already exists.");
			printf("File exists. Please choose an option:\n"
				"\t1) Backup file.\n"
				"\t2) Overwrite file.\n"
				"\t3) Enter new file name.\n"
				"\t4) Quit the program.\n>> "
			);
			char c[128];
			fgets(c, 8, stdin);
			
			switch (c[0])
			{
			case '1': // Backup output file.
				log_info("User opted to backup output file.");
				backup_file(fileName);
				delete_file(fileName);
				validFile = 1;
				break;

			case '2': // Overwrite output file.
				log_info("User opted to overwrite output file.");
				delete_file(fileName);
				validFile = 1;
				break;
			
			case '3': // Enter new output file name.
				log_info("User opted to enter new file name.");
				fileName[0] = '\0';
				break;

			case '\n':
				log_info("No input from user.");
				status = 2;
				break;

			case '4': // Quit the program.
				log_info("User opted to quit.");
				status = 1;
				break;
			
			default:
				log_info("Invalid user selection.");
				break;
			}
		}
		else
		{
			validFile = 1;
		}
	}

	return status;

}