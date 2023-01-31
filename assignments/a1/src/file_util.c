/**
 * @file file_util.c
 * @Group 2
 * @Program Program 1: File Opener
 * @author Robert Krency (kre1188@pennwest.edu)
 * @author Cody Long (lon1150@pennwest.edu)
 * @author Noelle Nieves (nie9236@pennwest.edu)
 * @brief 
 * 
 */
// Contains file open routine, support routines //

#include "file_util.h"
#include "log_util.h"
#include <stdio.h>
#include <string.h>

// FUNCTION DEFINITIONS //



int check_ext(char* fileName)
{
	int hasExt = 0;
	for (int i = 0; i < strlen(fileName) && !hasExt; i++)
		if (fileName[i] == '.')
			hasExt = 1;
	return hasExt;
}

int check_file_exists(char* fileName)
{
    FILE *fp = fopen(fileName, "r");
    int is_exist = 0;
    if (fp != NULL)
    {
        is_exist = 1;
        fclose(fp); // close the file
    }
    return is_exist;
}


void backup_file(char* fileName)
{
	log_message("Creating a backup of the file.");
	char backup_name[strlen(fileName) + 4];
	strcpy(backup_name, fileName);
	strcat(backup_name, ".bak");

	FILE* file = fopen(fileName, "r");
	FILE* backup = fopen(backup_name, "w");

	copy_file(backup, file);

	fclose(file);
	fclose(backup);

	log_message("Backup created.");
}

void delete_file(char* fileName)
{
	if (remove(fileName) == 0)
		log_message("Deleted file successfully.");
	else
		log_message("Unable to delete file.");
}

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

FILE* open_file(char* fileName, char* c){
	FILE *fp = fopen(fileName, c);

	if(NULL != fp){
		log_message("Opened %s.", fileName);
	}
	else{
		log_message("Unable to open %s.", fileName);
	}

	return fp;
}