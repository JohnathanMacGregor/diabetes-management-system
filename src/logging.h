#ifndef LOGGING_H
#define LOGGING_H

// Struct representing a log entry for insulin management data.
typedef struct {
    float blood_glucose_level;    // Glucose level in mmol/l
    float insulin_dosage;         // Insulin dosage in units
    float meal_time_carbs;        // Total carbohydrates in the meal
    char entry_type[20];          // Type of log entry (e.g., Meal, Correction, etc...)
    float target_blood_glucose;   // Target blood glucose level in mmol/L 
    int correction_factor;        // Correction factor in mmol/L/unit
    float correction_dosage;      // Calculated correction dosage in units
    float carb_ratio;             // Carbohydrate to insulin ratio in g/unit
    char unit[10];                // Unit measurement for blood glucose in mmol/L or mg/dL
    
    // Set of flags for checking if member is set
    int blood_glucose_level_flag;
    int target_blood_glucose_flag;
    int meal_time_carbs_flag;
    int correction_dosage_flag;
    int insulin_dosage_flag;
} log_entry;


/**
 * log_config - Reads configuration values from file and populates the log_entry struct
 * @param entry: A pointer to the log_entry struct to populate.
 * @return 0 on success, -1 for errors.
 */
int log_config(log_entry *entry);

/**
 * log_date_time - Logs the current date and time into the given file
 * 
 * @param filename: Name of file to log the date and time into.
 * @return: 0 for success, -1 for errors
 */
int log_date_time(const char *filename);

/**
 * log_data - Logs insulin management data to the file given.
 * 
 * @param entry: log_entry struct containing data to log
 * @return 0 for success, -1 for failure.
 */
int log_data(log_entry entry, const char *filename);

/**
 * read_logs: Reads and filters log entries from the specified file based on a time filter
 * 
 * @param filename: File that contains log entries.
 * @param time_filter: The time filter; "day", "week", "2 weeks" or "month".
 * @return 0 for success, -1 for errors.
 */
int read_logs(const char *filename, const char *time_filter);

#endif
