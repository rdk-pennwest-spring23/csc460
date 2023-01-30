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
	log_message("MAIN", "Creating a backup of the file.");
	char backup_name[strlen(fileName) + 4];
	strcpy(backup_name, fileName);
	strcat(backup_name, ".bak");

	FILE* file = fopen(fileName, "r");
	FILE* backup = fopen(backup_name, "w");

	char ch = fgetc(file);
	while (ch != EOF)
	{
		fputc(ch, backup);
		ch = fgetc(file);
	}

	fclose(file);
	fclose(backup);

	log_message("MAIN", "Backup created.");
}

void delete_file(char* fileName)
{
	if (remove(fileName) == 0)
		log_message("MAIN", "Deleted file successfully.");
	else
		log_message("MAIN", "Unable to delete file.");
}