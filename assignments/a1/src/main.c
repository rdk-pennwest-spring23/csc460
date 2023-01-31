/**
 * @file main.c
 * @Group 2
 * @Program Program 1: File Opener
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long (lon1150@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
 * @brief 
 * 
 */

#include "file_util.h"
#include "log_util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	// Initialize Logging
	set_log_to_file(); // Comment out this line to turn off logging to file

	int quit = 0;
	char inputFileName[128], outputFileName[128];
	inputFileName[0] = '\0';
	outputFileName[0] = '\0';

	// Check if there are command line arguments input
	if (argc > 1) 
	{
		strcpy(inputFileName,argv[1]);
		log_message("Input file specified on command line.");
		if (argc > 2)
		{
			strcpy(outputFileName, argv[2]);
			log_message("Output file specified on command line.");
		}
	}

	// Check for a valid input file
	int validInput = 0;
	while (!quit && !validInput)
	{	
		// Check if the user has input a file name.
		if (inputFileName[0] == '\0')
		{
			log_message("Getting user input for input file name.");
			printf("Please enter a name for the input file: ");
			scanf("%s", inputFileName);
			log_message("Accepted user input for input file name.");
		}

		// Check if the file name is missing an extension
		if (!check_ext(inputFileName))
		{
			log_message("File extension missing. Appending '.in' extension to input file name.");
			char newFileName[strlen(inputFileName) + 3];
			strcpy(newFileName, inputFileName);
			strcat(newFileName, ".in");
			strcpy(inputFileName, newFileName);
		}

		// Check if the file exists
		if (check_file_exists(inputFileName))
		{
			log_message("Input file exists.");
			validInput = 1;
		}
		else
		{
			log_message("Error: Input file does not exist.");
			inputFileName[0] = '\0';
		}
	}

	// Check for a valid ouput file
	int validOutput = 0;
	while (!quit && !validOutput)
	{
		char* userInput;

		// Check if the user has input a file name.
		if (outputFileName[0] == '\0')
		{
			// Prompt for an output file name
			log_message("Getting user input for output file name.");
			printf("Please enter a name for the output file: ");
			scanf("%s", outputFileName);
			log_message("Accepted user input for output file name.");
		}

		// Check if the file name is missing an extension
		if (!check_ext(outputFileName))
		{
			log_message("File extension missing. Appending 'out' extension to input file name.");
			char newFileName[strlen(outputFileName) + 4];
			strcpy(newFileName, outputFileName);
			strcat(newFileName, ".out");
			strcpy(outputFileName, newFileName);
		}
		
		// Check if the output file already exists
		if (check_file_exists(outputFileName))
		{
			log_message("Output file already exists.");
			printf("Please choose an option:\n"
				"\t1) Backup output file.\n"
				"\t2) Overwrite output file.\n"
				"\t3) Enter new output file name.\n"
				"\t4) Quit the program.\n"
			);
			char c[128];
			scanf("%s", c);
			
			switch (c[0])
			{
			case '1': // Backup output file.
				log_message("User opted to backup output file.");
				backup_file(outputFileName);
				delete_file(outputFileName);
				validOutput = 1;
				break;

			case '2': // Overwrite output file.
				log_message("User opted to overwrite output file.");
				delete_file(outputFileName);
				validOutput = 1;
				break;
			
			case '3': // Enter new output file name.
				log_message("User opted to enter new file name.");
				outputFileName[0] = '\0';
				break;

			case '4': // Quit the program.
				log_message("User opted to quit.");
				quit = 1;
				break;
			
			default:
				log_message("Invalid user selection.");
				break;
			}
		}
		else // Output file does not exist.
		{
			validOutput = 1;
		}
	}
	
	//if user did not quit
	if(!quit){
		// If input file opened successfully
		FILE *in_fp = open_file(inputFileName, "r");
		if(NULL != in_fp){
			//log_message("MAIN", "Opening output file.");
			FILE *out_fp = open_file(outputFileName, "w");

			//create listing file
			//log_message("MAIN", "Opening listings file.");
			char *listingFileName = outputFileName;
			strcat(listingFileName, ".lis");
			FILE *list_fp = open_file(listingFileName, "w");

			//create temporary file
			//log_message("MAIN", "Opening temporary file.");
			char *tempFileName = "temporary.txt";
			FILE *temp_fp = open_file(tempFileName, "w");

			//if output, listing, and temporary files open successfully
			if ((NULL != out_fp) && (NULL != list_fp) && (NULL != temp_fp))
			{
				log_message("Copying data.");
				// Read character from input file and write them to output files
				copy_file(out_fp, in_fp);
				copy_file(list_fp, in_fp);
				copy_file(temp_fp, in_fp);

				//close files
				log_message("Closing files.");
				fclose(out_fp);
				fclose(list_fp);
				fclose(temp_fp);
			}

			//close input file
			fclose(in_fp);
		
			// Delete the temporary file
			// delete_file(tempFileName);
		}
	}

	// Clean up.
	end_log_to_file();
	return 0;
}
