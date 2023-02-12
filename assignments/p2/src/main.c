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


int main(int argc, char *argv[])
{
	int status = 0;
	
	// Open Files
	if (status = open_files(argc, argv))
		log_info("Successfully opened files.");
	else
		log_error("Unable to successfully open files.");

	// Run the Scanner

	// Clean up.
	if (status = close_files())
		log_info("Successfully cleaned up files.");
	else
		log_error("Unable to successfully clean up files.");
	
	return 0;
}