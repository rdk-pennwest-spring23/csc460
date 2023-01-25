// Contains file open routine, support routines //

#include "file_util.h"


// FUNCTION DEFINITIONS //


// When called by main, get user input
// Follow the format "fileopen input_file output_file"
// All file names may include drive and path

	// If only "fileopen" entered
		// Prompt for input and output file names
	
	// If only input file entered
		// If no extension given, default to .in
		// Prompt for output file name
	
	// If input and output file entered
		// If no extensions given, default to .in and .out
		
	// If there is a missing parameter, prompt user again
	
// Validate input file to already exist
	// If it does not, reprompt for file name until a valid one is entered
	// If no name is entered, terminate program

// Validate output file to NOT already exist
	// If it does exist, the user can either:
		// Re-enter a file name
		// Overwrite the file entered
			// In this case, back up existing output file with a .bak extension and use name for new output file
			// Account for existing backup files
		// Use a default file name
			// When no name is entered, the input file name is used for the output file, but with a .out extension
			// If this file already exists, use same rules for overwriting
		// Terminate the program
	// If it does not exist, create the file
		
// Create temporary file
	// Can use any name and extension, but cannot overwrite existing file

// Create listing file
	// Uses output file name but with .lis extension
		// Apply same rules for overwriting as with output file
		
// Indicate if files are open or not