/**
 * @file main.c
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
#include "scanner.h"
#include "parser.h"
#include "log_util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


int main(int argc, char *argv[])
{
	int status = 0;
	
	// // Open Files
	if (status = open_files(argc, argv))
		log_info("Successfully opened files.");
	else
		log_error("Unable to successfully open files.");

	// Init the Scanner
	if (status)
		status = init_scanner();

	// Run the Parser
	if (status)
	{
		status = run_parser();
	}

	// Clean up.
	if (status = close_files())
		log_info("Successfully cleaned up files.");
	else
		log_error("Unable to successfully clean up files.");
	
	// End
	log_info("Finishing up.");
	return 0;
}