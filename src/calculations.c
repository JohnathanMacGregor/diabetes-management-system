#include <stdio.h>
#include "calculations.h"
#include <math.h>
#include <string.h>
#include "logging.h"

// Target Blood Glucose Range in mmol/L
#define lower_target 4.0
#define upper_target 8.0


float convert_to_mmol_L(float blood_glucose, const char *unit){

    if (unit != NULL && strcmp(unit, "mg/dL") == 0){
        return blood_glucose / 18.018; //Conversion factor from mg/dL to mmol/L
    }

    return blood_glucose; // No conversion if unit is mmol/L
}


float convert_to_preferred_unit(float blood_glucose, const char *unit){
    if (unit != NULL && strcmp(unit, "mg/dL") == 0){
        return blood_glucose * 18.018; // Converting mmol/L to mg/dL
    }
    return blood_glucose;
}


float meal_dosage_calculation(float carb_amount, float carb_ratio) {
    return (carb_amount / carb_ratio);
}


float correction_dosage_calculation(log_entry *entry, int insulin_sensitivity_factor, float blood_glucose_level, float target_blood_glucose) {
    blood_glucose_level = convert_to_mmol_L(blood_glucose_level,entry->unit);
    // No correction for blood glucose levels between target range
    if (blood_glucose_level >= lower_target && blood_glucose_level <= upper_target){
        return 0.0;
    }
    if (blood_glucose_level < lower_target){
        return 0.0;
    }
    
    // Correction dosage formula for high blood glucose
    return ((blood_glucose_level - target_blood_glucose) / insulin_sensitivity_factor);
}



float total_dosage(log_entry *entry,float carb_amount, float carb_ratio, 
                   int insulin_sensitivity_factor, float blood_glucose_level, 
                   float target_blood_glucose) {
    float meal_dosage = meal_dosage_calculation(carb_amount, carb_ratio);
    float correction_dosage = correction_dosage_calculation(entry,insulin_sensitivity_factor, blood_glucose_level, target_blood_glucose);
    //Rounds total dosage to nearest 0.5 units
    return round((meal_dosage + correction_dosage) * 2) / 2;
}
