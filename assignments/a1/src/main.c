// Contains main routine //

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
		log_message("MAIN", "Input file specified on command line.");
		if (argc > 2)
		{
			strcpy(outputFileName, argv[2]);
			log_message("MAIN", "Output file specified on command line.");
		}
	}

	// Check for a valid input file
	int validInput = 0;
	while (!quit && !validInput)
	{	
		// Check if the user has input a file name.
		if (inputFileName[0] == '\0')
		{
			log_message("MAIN", "Getting user input for input file name.");
			printf("Please enter a name for the input file: ");
			scanf("%s", inputFileName);
			log_message("MAIN", "Accepted user input for input file name.");
		}

		log_message("TEST", inputFileName);

		// Check if the file name is missing an extension
		if (!check_ext(inputFileName))
		{
			log_message("MAIN", "File extension missing. Appending '.in' extension to input file name.");
			char newFileName[strlen(inputFileName) + 3];
			strcpy(newFileName, inputFileName);
			strcat(newFileName, ".in");
			strcpy(inputFileName, newFileName);
		}

		// Check if the file exists
		if (check_file_exists(inputFileName))
		{
			log_message("MAIN", "Input file exists.");
			validInput = 1;
		}
		else
		{
			log_message("MAIN", "Error: Input file does not exist.");
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
			log_message("MAIN", "Getting user input for output file name.");
			printf("Please enter a name for the output file: ");
			scanf("%s", outputFileName);
			log_message("MAIN", "Accepted user input for output file name.");
		}

		// Check if the file name is missing an extension
		if (!check_ext(outputFileName))
		{
			log_message("MAIN", "File extension missing. Appending 'out' extension to input file name.");
			char newFileName[strlen(outputFileName) + 4];
			strcpy(newFileName, outputFileName);
			strcat(newFileName, ".out");
			strcpy(outputFileName, newFileName);
		}
		
		// Check if the output file already exists
		if (check_file_exists(outputFileName))
		{
			log_message("MAIN", "Output file already exists.");
			printf("Please choose an option:\n"
				"\t1) Backup output file.\n"
				"\t2) Overwrite output file.\n"
				"\t3) Enter new output file name.\n"
				"\t4) Quit the program.\n"
			);
			char c;
			scanf("%c", &c);
			
			switch (c)
			{
			case '1': // Backup output file.
				backup_file(outputFileName);
				delete_file(outputFileName);
				validOutput = 1;
				break;

			case '2': // Overwrite output file.
				delete_file(outputFileName);
				validOutput = 1;
				break;
			
			case '3': // Enter new output file name.
				outputFileName[0] = '\0';
				break;

			case '4': // Quit the program.
				quit = 1;
				break;
			
			default:
				log_message("MAIN", "Invalid user selection.");
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
		FILE *in_fp = fopen(inputFileName, "r");
		if(NULL != in_fp){
			log_message("MAIN", "Opening output file.");
			FILE *out_fp = fopen(outputFileName, "w");

			//create listing file
			log_message("MAIN", "Opening listings file.");
			char *listingFileName = outputFileName;
			strcat(listingFileName, ".lis");
			FILE *list_fp = fopen(listingFileName, "w");

			//create temporary file
			log_message("MAIN", "Opening temporary file.");
			char *tempFileName = "temporary.txt";
			FILE *temp_fp = fopen(tempFileName, "w");

			//if output, listing, and temporary files open successfully
			if ((NULL != out_fp) && (NULL != list_fp) && (NULL != temp_fp))
			{
				log_message("MAIN", "Copying data.");
				// Read character from input file and write them to output files
				char ch= fgetc(in_fp);
				while (ch != EOF)
				{
					fputc(ch, out_fp);
					fputc(ch, list_fp);
					fputc(ch, temp_fp);
					ch = fgetc(in_fp);
					// Write character to output file, listing file, and temporary file
					
				} 

				//close files
				log_message("MAIN", "Closing files.");
				fclose(out_fp);
				fclose(list_fp);
				fclose(temp_fp);
			}
			else{
				log_message("MAIN", "Error: output, listing, or temporary file open unsuccessful.");
			}

			//close input file
			fclose(in_fp);
		
			// Delete the temporary file
			// delete_file("temporary.txt");
		}
		else {
			log_message("MAIN", "Error: input file open unsuccessful.");
		}
	}

	// Clean up.
	end_log_to_file();
	return 0;
}
