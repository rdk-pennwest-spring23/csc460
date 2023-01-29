// Contains main routine //

#include "file_util.h"
#include "log_util.h"

#include <regex.h>

int main(int argc, char *argv[])
{
	// Initialize Logging
	set_log_to_file(); // Comment out this line to turn off logging to file

	// Check if there are command line arguments input
	if (argc > 1) 
	{
		
	}



	// Prompt for user input
		// User must enter two parameters, input file and output file
	
	char *inputFileName = argv[1], *outputFileName = argv[2];

	log_message("MAIN", inputFileName);
	log_message("MAIN", outputFileName);
		
	// Call file utility functions
	
	// If files opened successfully
		// Read character from input file
		// Write character to output file, listing file, and temporary file
		
	// Include code to delete temporary file, but comment out for the assignment

	// Clean up.
	end_log_to_file();
	return 0;
}
