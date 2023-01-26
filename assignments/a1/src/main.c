// Contains main routine //

#include "file_util.h"
#include <regex.h>

int main(int argc, char *argv[])
{

	// Prompt for user input
		// User must enter two parameters, input file and output file
	
	regex_t filenameRegex;
	char* inputFileName = argv[1], outputFileName = argv[2];
	int flag;

	flag = regcomp(&filenameRegex, "^[^~)('!*<>:;,?\"*|/]+$", 0);
	if (flag) 
	{
		fprintf(stderr, "Error initializing filename regex.");
		exit(1);
	}

	flag = regexec(&filenameRegex, inputFileName, 0, NULL, 0);
	if (!flag)
	{
		puts("Input file name is a valid filepath.");
	}
		
	// Call file utility functions
	
	// If files opened successfully
		// Read character from input file
		// Write character to output file, listing file, and temporary file
		
	// Include code to delete temporary file, but comment out for the assignment

	system("PAUSE");
	return 0;
}
