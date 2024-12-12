#include <stdio.h>
#include "logging.h"
#include "calculations.h"
#include "config.h"
#include <string.h>
#include <math.h>

// Function declarations

/**
 * display_main_menu - Displays main menu for diabetes management system.
 */
void display_main_menu();

/**
 * display_log_types - Displays the types of log entries available.
 */
void display_log_types();

/**
 * log_filtering - Allows user to view logs based on a time range.
 * 
 * @param filename: File containing log entries.
 *  */ 
void log_filtering(const char *filename);

/**
 * collect_user_input - Collects user input for log entry.
 * 
 * @param entry: Pointer to log_entry struct to populate.
 */
void collect_user_input(log_entry *entry);
/**
 * collect_insulin_input - Updates insulin settings based on user input.
 * 
 * @param entry: Pointer to the log_entry struct to update.
 */
void collect_insulin_input(log_entry *entry);

/**
 * calculate_dosages - Calculates insulin dosage based on the log entry type.
 * 
 * @param entry: Pointer to log_entry struct containing data for calculations.
 */
void calculate_dosages(log_entry *entry);

/**
 * log_insulin_data - Logs insulin data and the date into a file.
 * 
 * @param filename: File where data will be logged.
 * @param entry: The log_entry struct containing the insulin management data to log
 */
void log_insulin_data(const char *filename, log_entry entry);

/**
 * accesss_menu - Controls program flow.
 * 
 * @param filename: The name of the log file.
 */
void access_menu(const char *filename);



void display_main_menu() {
    printf("\nDiabetes Management System\n");
    printf("1. Log Entry\n");
    printf("2. View Logs\n");
    printf("3. View Insulin Settings\n");
    printf("4. Update Insulin Settings\n");
    printf("5. Exit program\n");
    printf("Enter an option\n");
}

void display_log_types(){
    printf("\nEntry Types\n");
    printf("1. MEAL\n");
    printf("2. SNACK\n");
    printf("3. CORRECTION\n");
    printf("4. OTHER\n");
}

void log_filtering(const char *filename){
    printf("\nSelect logs to view:\n");
    printf("1. View logs from today\n");
    printf("2. View logs from the past week\n");
    printf("3. View logs from the past 2 weeks\n");
    printf("4. View logs from the past month\n");

    int choice;
    while (scanf("%d", &choice) != 1 || choice < 1 || choice > 4){
        printf("Invalid input. Please Enter a number between 1 and 4: ");
        while (getchar() != '\n'); 
    }

    const char *time_filter = NULL;
    // Filter options
    switch (choice) {
        case 1: time_filter = "day"; break;
        case 2: time_filter = "week"; break;
        case 3: time_filter = "2 weeks"; break;
        case 4: time_filter = "month"; break;
        default: 
            printf("Invalid choice.\n");
            return;
    }

    if(read_logs(filename, time_filter) != 0){
        printf("Failed to filter logs.\n");
    }
}

void collect_user_input(log_entry *entry){
    // Resets flags
    entry->blood_glucose_level_flag = 0;
    entry->target_blood_glucose_flag = 0;
    entry->meal_time_carbs_flag = 0;
    entry->correction_dosage_flag = 0;
    entry->insulin_dosage_flag = 0;

    // Retrieve blood glucose level from user
    printf("\nEnter blood glucose level in %s: ", entry->unit);
    while(scanf("%f", &entry->blood_glucose_level) != 1 || entry ->blood_glucose_level < 0){
        printf("Invalid input. Please enter valid blood glucose level: ");
        while(getchar() != '\n');
    }
    //Convert blood glucose to standard unit for storing
    entry->blood_glucose_level = convert_to_mmol_L(entry->blood_glucose_level,entry->unit);
    entry->blood_glucose_level_flag = 1;
    
    
    if (strcmp(entry->entry_type, "correction") == 0 || 
        strcmp(entry->entry_type, "meal") == 0 || 
        strcmp(entry->entry_type, "snack") == 0) {
        printf("Using target blood glucose level: %.2f %s\n", entry->target_blood_glucose,entry->unit);
        entry->target_blood_glucose = convert_to_mmol_L(entry->target_blood_glucose, entry->unit);
        entry->target_blood_glucose_flag = 1;
    
    }

    // Collect carbohydrates in grams for meals or snacks
    if (strcmp(entry->entry_type, "meal") == 0 || strcmp(entry->entry_type, "snack") == 0) {
        printf("Enter total carbohydrates in the meal (in grams): ");
        while(scanf("%f", &entry->meal_time_carbs) != 1 || entry->meal_time_carbs < 0){
            printf("Invalid input. Please enter a valid number for carbohydrates: ");
            while(getchar() != '\n');
        }
        entry->meal_time_carbs_flag = 1;
    }

    
}

void collect_insulin_input(log_entry *entry) {
    char choice;
    char buffer[50];
    do {
        printf("\nSelect the parameter to update:\n");
        printf("1. Insulin Sensitivity Factor (e.g., 1:2 -> 2)\n");
        printf("2. Carb Ratio (e.g., 1:10 -> 10)\n");
        printf("3. Blood Glucose Units (mmol/L or mg/dL)\n");
        printf("4. Target Blood Glucose Level\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        
        while (scanf(" %c", &choice) != 1 || choice < '1' || choice > '5') {
            printf("Invalid input. Please select a valid option (1-5): ");
            while (getchar() != '\n'); 
        }
        // Update the configuration selected by the user
        switch (choice) {
            case '1': {
                int insulin_sensitivity_factor;
                printf("Enter insulin sensitivity factor (e.g., 1:2 -> 2): ");
                while (scanf("%d", &insulin_sensitivity_factor) != 1 || insulin_sensitivity_factor <= 0) {
                    printf("Invalid input. Please enter a valid insulin sensitivity factor: ");
                    while (getchar() != '\n');
                    
                }
                sprintf(buffer, "%d", insulin_sensitivity_factor);
                update_config("config.txt", "insulin sensitivity factor", buffer);
                entry->correction_factor = insulin_sensitivity_factor;
                break;
            }
            case '2': {
                float carb_ratio;
                printf("Enter carb ratio (e.g., 1:10 -> 10): ");
                while (scanf("%f", &carb_ratio) != 1 || carb_ratio <= 0) {
                    printf("Invalid input. Please enter a valid carb ratio: ");
                    while (getchar() != '\n');  
                }
                sprintf(buffer, "%.2f", carb_ratio);
                update_config("config.txt", "carb ratio", buffer);
                entry->carb_ratio = carb_ratio;
                break;
            }
            case '3': {
                char blood_glucose_unit[10];
                printf("Enter blood glucose units (mmol/L or mg/dL): ");
                while (1) {
                    scanf("%9s", blood_glucose_unit);
                    if (strcmp(blood_glucose_unit, "mmol/L") == 0 || strcmp(blood_glucose_unit, "mg/dL") == 0) {
                        break;
                    }
                    printf("Invalid input. Please enter a valid unit (mmol/L or mg/dL): ");
                    while (getchar() != '\n');
                   
                }
                update_config("config.txt", "blood glucose unit", blood_glucose_unit);
                break;
            }
            case '4':{
                float target_blood_glucose;
                printf("Enter target blood glucose level: ");
                while (scanf("%f", &target_blood_glucose) != 1 || target_blood_glucose <= 0) {
                    printf("Invalid input. Please enter a valid target blood glucose level: ");
                    while (getchar() != '\n');
                    
                }
                sprintf(buffer, "%.2f", target_blood_glucose);
                update_config("config.txt", "target blood glucose", buffer);
                entry->target_blood_glucose = target_blood_glucose;
                break;
            }

            case '5':
                printf("Exiting insulin settings.\n");
                break;
            
        }
    } while (choice != '5');
}

void access_menu(const char *filename){
    
    int choice; 
    int choice2;
    log_entry entry = {0};

    if (log_config(&entry) != 0){
        printf("Failed to load configuration values. Please check config.txt.\n");
        return;
    }


    do {
        display_main_menu();
        while (scanf("%d", &choice) != 1 || choice < 1 || choice > 5) {
            printf("Invalid input. Please enter a number between 1 and 5: ");
            while (getchar() != '\n'); 
        }
        if (choice == 1){
            display_log_types();
            while (scanf("%d", &choice2) != 1 || choice2 < 1 || choice2 > 4) {
                printf("Invalid input. Please select a log type (1-4): ");
                while (getchar() != '\n'); 
            }


            // Assign entry type
            const char *types[] = {"meal", "snack", "correction", "other"};
            strncpy(entry.entry_type, types[choice2 - 1], sizeof(entry.entry_type) - 1);
            entry.entry_type[sizeof(entry.entry_type) - 1] = '\0'; 

            collect_user_input(&entry);

            // Only calculate dosages if relevant
            if (choice2 < 4) { 
                calculate_dosages(&entry);
            }

            log_insulin_data(filename, entry);
            
        } else if (choice == 2){
            log_filtering(filename);
        } else if (choice == 3){
            printf("\nInsulin Settings\n");
            list_config("config.txt");
        } else if (choice == 4){

            collect_insulin_input(&entry);
        }else if (choice == 5) {
            printf("Exiting program...Goodbye\n");
        }else{
            printf("Please enter a valid opetion (1 to 5): ");
        } 
    }while (choice != 5);
}

void calculate_dosages(log_entry *entry) {
    
    // Calculates only correction dose if applicable
    if (entry->blood_glucose_level_flag && entry->target_blood_glucose_flag) {
        entry->correction_dosage = correction_dosage_calculation(
            entry,
            entry->correction_factor,
            entry->blood_glucose_level,
            entry->target_blood_glucose
        );
        entry->correction_dosage_flag = 1; // Set flag
        entry->insulin_dosage = round(entry->correction_dosage * 2)/2;
        entry->insulin_dosage_flag = 1;
    } else {
        entry->correction_dosage_flag = 0; // Unset flag
        entry->insulin_dosage_flag = 0;
    }
    // Calculates total insulin dose for meals and snacks
    if (entry->meal_time_carbs_flag) {
        entry->insulin_dosage = total_dosage(
            entry,
            entry->meal_time_carbs,
            entry->carb_ratio,
            entry->correction_factor,
            entry->blood_glucose_level,
            entry->target_blood_glucose
        );
        entry->insulin_dosage_flag = 1; // Set flag
    } else {
        if (strcmp(entry->entry_type,"correction") != 0)
            entry->insulin_dosage_flag = 0; // Unset flag
    }
}

void log_insulin_data(const char *filename, log_entry entry) {
    log_date_time(filename);
    log_data(entry, filename);
}

int main() {
    const char *filename = "data/logs.txt";
    access_menu(filename);
    return 0;

}