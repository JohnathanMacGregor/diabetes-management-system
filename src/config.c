#include <stdio.h>
#include "config.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

//list config just shows the units, ratio, isf
//read config for getting the units, ratio, isf 
//make them be able to change the units...
//convert between units 


char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}


const char* read_config(const char *key) {

    static char value[50];
    
    FILE *file = fopen("config.txt", "r");

    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    char buffer[50]; 
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';  // remove newline character

        //parse line into key and value
        char *parsed_key = strtok(buffer, "=");
        char *parsed_value = strtok(NULL, "=");

        if (parsed_key != NULL && parsed_value != NULL) {
            parsed_key = trimwhitespace(parsed_key);
            parsed_value = trimwhitespace(parsed_value);
            
            // Compare the keys
            if (strcmp(key, parsed_key) == 0) {
                fclose(file);
                strncpy(value, parsed_value, sizeof(value) - 1); 
                value[sizeof(value) - 1] = '\0';
                return value;  // Return the parsed value
            }
        }
    }

    fclose(file);
    return NULL; // If key not found, return NULL
}

int list_config(const char *filename){

    FILE *file = fopen(filename,"r");
    char buffer[100];

    if (file == NULL){
        perror("Error opening file");
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL){
        printf("%s",buffer);
    }

    fclose(file);
    return 0;

}

int update_config(const char *filename, const char *key, const char *new_value){
    
    FILE *file = fopen(filename,"r");
    if (file == NULL){
        perror("Error opening original file");
        return -1;
    }

    //Open a temporary file
    FILE *temp_file = fopen("temp_config.txt","w");
    if (file == NULL){
        perror("Error opening temporary file");
        fclose(file);
        return -1;
    }
    char buffer[50];
    bool key_found = false;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';  

        char *parsed_key = strtok(buffer, "=");
        char *parsed_value = strtok(NULL, "=");

        if (parsed_key != NULL && parsed_value != NULL){
            
            parsed_key = trimwhitespace(parsed_key);
            parsed_value = trimwhitespace(parsed_value);
            
            //Compare keys
            if (strcmp(key, parsed_key)==0){
                // Write the key and new value to temp file if keys matched
                fprintf(temp_file, "%s = %s\n", key, new_value); 
                key_found = true; // Update flag 
            } else {
                // Write the original values into the temp file
                fprintf(temp_file, "%s = %s\n", parsed_key, parsed_value);
            }
        }else {
            fprintf(temp_file, "%s\n", buffer);
        }

    }

    // If key was not found, keep file as is
    if (!key_found){
        printf("%s not found in file.\n",key);  // Debug line
        fclose(file);
        fclose(temp_file);
        remove("temp_config.txt");
        return -1;
    }

    fclose(file);
    fclose(temp_file);

    // Delete old file and rename temp file to the orginal file name 
    if (remove(filename) != 0) {
        perror("Error removing original file");
        return -1;
    }
    if (rename("temp_config.txt", filename) != 0) {
        perror("Error renaming temporary file");
        return -1;
    }


    return 0;
}

