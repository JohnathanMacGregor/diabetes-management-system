// logging.c

#include <stdio.h>
#include "logging.h"
#include "calculations.h"
#include <time.h> 
#include "config.h"
#include <stdlib.h>
#include <string.h> 

int log_config(log_entry *entry) {
    if (!entry) {
        return -1;
    }

    // Read carb ratio configuration value from config file
    const char *carb_ratio_str = read_config("carb ratio");
    if (carb_ratio_str == NULL){
        printf("Error: Missing carb ratio in config.txt");
        return -1;
    }
    
    
    entry->carb_ratio = atof(carb_ratio_str); // Converts string to float

    // Read the ISF from config file
    const char *insulin_sensitivity_factor_str = read_config("insulin sensitivity factor");
    
    if (insulin_sensitivity_factor_str == NULL){
        printf("Error: Missing insulin sensitivity factor in config.txt");
        return -1;
    }
    
    
    entry->correction_factor = atoi(insulin_sensitivity_factor_str); //Convert string to int

    // Read blood glucose unit from config file.
    const char *unit_str = read_config("blood glucose unit");
    if (unit_str == NULL){
        printf("Error: Missing blood glucose unit in config.txt");
        return -1;
    }
    
    // Copy unit into entry struct
    strncpy(entry->unit, unit_str, sizeof(entry->unit) - 1);
    entry->unit[sizeof(entry->unit) - 1] = '\0'; // Null termination

    // Validate configuration values
    if (entry->carb_ratio <= 0 || entry->correction_factor <= 0) {
        printf("Error: Invalid values in config.txt (carb ratio or correction factor).\n");
        return -1;
    }
    
    // Read target blood glucose from config file
    const char *target_blood_glucose_str = read_config("target blood glucose");
    if (target_blood_glucose_str == NULL){
        printf("Error: Missing target blood glucose in config.txt\n");
        return -1;

    }
    entry->target_blood_glucose = atof(target_blood_glucose_str); 
    if (entry->target_blood_glucose <= 0){
        printf("Error: Invalid target blood glucose value in config.txt\n");
        return -1;
    }
   
    return 0;
}



int log_date_time(const char *filename){
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file for date logging\n");
        return -1;
    }
    
    time_t t = time(NULL);  // Get current time

    // Convert to local time
    struct tm date = *localtime(&t);

    char datetime_str[20]; // Buffer for formatted date and time
    snprintf(datetime_str, sizeof(datetime_str), "%d-%02d-%02d %02d:%02d:%02d\n", 
             date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, 
             date.tm_hour, date.tm_min, date.tm_sec);


    // Write the timestamp to the file
    if (fprintf(file, "Log Entry Time: %s\n", datetime_str) < 0){
        perror("Error writing date to log file\n");
        fclose(file);
        return -1;
    }  

    fclose(file);
    
    return 0;
}

int log_data(log_entry entry, const char *filename){
    
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file for data logging\n");
        return -1;  
    }
    
    // Log data into file if flag is set
    if(entry.blood_glucose_level_flag){
        if(fprintf(file,"Blood Glucose: %.2f mmol/L\n",entry.blood_glucose_level) < 0 ){
            perror("Error writing blood glucose to log file\n");
            fclose(file);
            return -1;
        }
    }

    if(entry.target_blood_glucose_flag){
        if (fprintf(file,"Target: %.2f mmol/L\n", entry.target_blood_glucose) < 0 ){
            perror("Error writing target blood glucose to log file\n");
            fclose(file);
            return -1;
        }
    }

    if(entry.meal_time_carbs_flag){
        if(fprintf(file,"Carbs: %.2f g, Carb Ratio: %.2f/unit\n",
        entry.meal_time_carbs, entry.carb_ratio) < 0 ) {
            perror("Error writing carbohydrate data to log file\n");
            fclose(file);
            return -1;
        }
    }

    if(entry.correction_dosage_flag){
        if (fprintf(file, "Correction Factor: %d mmol/L/unit\n"
                       "Correction Dosage: %.2f units\n",
                        entry.correction_factor, entry.correction_dosage) < 0 ){
                            perror("Error writing correction dosage to log file\n");
                            fclose(file);
                            return -1;
                        }
    }

    if (entry.insulin_dosage_flag){
        if (fprintf(file, "Total Insulin Dosage: %.2f units\nType: %s\n", 
            entry.insulin_dosage, entry.entry_type) < 0){
                perror("Error writing insulin dosage to log file\n");
                fclose(file);
                return -1;
            }
    }
    
    if (entry.blood_glucose_level_flag && (!entry.correction_dosage_flag && !entry.meal_time_carbs_flag)){
        if (fprintf(file, "Type: %s\n", entry.entry_type) < 0){
            perror("Error writing entry type to log file\n");
            fclose(file);
            return -1;
        }
    }

    // Suggests insulin dose if needed
    if (entry.insulin_dosage_flag || entry.correction_dosage_flag)
        printf("\nSuggested Insulin Dosage: %.2f units\n", entry.insulin_dosage);
    
    fclose(file);
    return 0;  
}

int read_logs(const char *filename, const char *time_filter) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    const char *preffered_unit = read_config("blood glucose unit"); //gets user's preffered unit

    time_t now = time(NULL); 
    struct tm *current_time = localtime(&now);
    time_t start_time = 0; // Inititalize start time
   
    //Determines start time based on the time filter.
    if (strcmp(time_filter, "day") == 0) {
        current_time->tm_hour = 0;
        current_time->tm_min = 0;
        current_time->tm_sec = 0;
        start_time = mktime(current_time); // Start of the day
    } else if (strcmp(time_filter, "week") == 0) {
        current_time->tm_hour = 0;
        current_time->tm_min = 0;
        current_time->tm_sec = 0;
        current_time->tm_mday -= current_time->tm_wday; // Start of week (Sunday)
        
        start_time = mktime(current_time);

    } else if (strcmp(time_filter, "2 weeks") == 0) {
        current_time->tm_hour = 0;
        current_time->tm_min = 0;
        current_time->tm_sec = 0;
        current_time->tm_mday -= (current_time->tm_wday + 7); // Start of 2 weeks ago
    
        start_time = mktime(current_time);
    } else if (strcmp(time_filter, "month") == 0) {
        current_time->tm_hour = 0;
        current_time->tm_min = 0;
        current_time->tm_sec = 0;
        current_time->tm_mday = 1; // Start of the month
        start_time = mktime(current_time);
    } else {
        printf("Invalid time filter specified.\n");
        fclose(file);
        return -1;
    }

    int within_filter = 0; // Flag to check if a log entry is within the time filter

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "Log Entry Time:", 15) == 0) {
            struct tm log_time = {0};
            //Parse timestamp from the log entry.
            if (sscanf(line, "Log Entry Time: %d-%d-%d %d:%d:%d\n",
                    &log_time.tm_year, &log_time.tm_mon, &log_time.tm_mday,
                    &log_time.tm_hour, &log_time.tm_min, &log_time.tm_sec) == 6) {
                log_time.tm_year -= 1900; 
                log_time.tm_mon -= 1;    
                time_t log_timestamp = mktime(&log_time);

                if (log_timestamp >= start_time) {
                    printf("\n%s", line); 
                    within_filter = 1; // Entry is within filter
                } else {
                    within_filter = 0; // Entry is outside filter
                }
            } else {
                printf("Error parsing log entry time: %s\n", line);
            }
        } else if (within_filter){
            // Parse and display applicable log entry details
            if (strncmp(line, "Blood Glucose:", 14) == 0) {
                float blood_glucose;
                sscanf(line, "Blood Glucose: %f mmol/L\n", 
                                    &blood_glucose);

                printf("Blood Glucose Level: %.2f %s\n", 
                        convert_to_preferred_unit(blood_glucose, read_config("blood glucose unit")), preffered_unit);
            } else if (strncmp(line, "Target:",7) == 0) {
                float target_blood_glucose;
                sscanf(line, "Target: %f mmol/L\n", 
                                    &target_blood_glucose);
                printf("Target: %.2f %s\n", 
                        convert_to_preferred_unit(target_blood_glucose, read_config("blood glucose unit")), preffered_unit);       

            } else if (strncmp(line, "Carbs:", 6) == 0) {
                float carbs, carb_ratio;
                sscanf(line, "Carbs: %f g, Carb Ratio: %f/unit", &carbs, &carb_ratio);
                printf("Carbs: %.2f g, Carb Ratio: %.2f/unit\n", carbs, carb_ratio);
            } else if (strncmp(line, "Correction Factor:", 18) == 0) {
                int correction_factor;
                sscanf(line, "Correction Factor: %d mmol/L/unit\n", 
                    &correction_factor);
                printf("Correction Factor: %d mmol/L/unit\n", 
                    correction_factor);
            } else if (strncmp(line, "Correction Dosage:", 18) == 0){
                float correction_dosage;
                sscanf(line,"Correction Dosage: %f units\n", &correction_dosage);
                printf("Correction Dosage: %.2f units\n",correction_dosage);
            } else if (strncmp(line, "Total Insulin Dosage:", 21) == 0) {
                float total_dosage;
                sscanf(line, "Total Insulin Dosage: %f units", &total_dosage);
                printf("Total Insulin Dosage: %.2f units\n", total_dosage);
            } else if (strncmp(line, "Type:", 5) == 0) {
                char entry_type[20];
                sscanf(line, "Type: %[^\n]", entry_type);
                printf("Type: %s\n", entry_type);
            }
        }
    }
        
    fclose(file);
    return 0;
}
