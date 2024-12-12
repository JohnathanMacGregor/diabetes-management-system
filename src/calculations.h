#ifndef CALCULATIONS_H
#define CALCULATIONS_H
#include "logging.h"

/** 
 * convert_to_mmol_L - converts given blood glucose value from the user's unit to mmol/L.
 * 
 * @param blood_glucose: Blood glucose value.
 * @param unit: The blood glucose unit in "mmol/L" ir "mg/dL".
 * @return: The blood glucose value in mmol/L.
 */
float convert_to_mmol_L(float blood_glucose, const char *unit);

/**
 * convert_to_preferred_unit - Converts given blood glucose value from the program's 
 * standard unit (mmol/L) to the user's preferred unit for display.
 *
 * @param blood_glucose The blood glucose value in mmol/L.
 * @param unit The user's preferred unit ("mmol/L" or "mg/dL").
 * @return The blood glucose value in the user's preferred unit.
 */
float convert_to_preferred_unit(float blood_glucose, const char *unit);


/**
 * meal_dosage_calculation - Calculates insulin dosage for a meal using carbohydrate
 * intake and user's carb ratio.
 * 
 * @param carb_amount: Total amount of carbs in grams.
 * @param carb_ratio: The carb to insulin ratio in grams/unit.
 * @return: The insulin dosage for the meal.
 */
float meal_dosage_calculation(float carb_amount, float carb_ratio);



/**
 * correction_dosage_calculation - Calculates the correction dosage using user's
 * insulin sensitivity factor (ISF), current blood glucose level, and target blood glucose 
 * level.
 * 
 * @param entry: The log entry storing user data and untis. 
 * @param insulin_sensitivity_factor: The ISF in mmol/L per unit of insulin.
 * @param blood_glucose_level: User's current blood glucose level.
 * @param target_blood_glucose: The user's target blood glucose level.
 * @return: The corection dosage in units.
 * 
 */
float correction_dosage_calculation(log_entry *entry,int insulin_sensitivity_factor, float blood_glucose_level, float target_blood_glucose);


/**
 * total_dosage - Calculates the total insulin dosage by combining meal and correction dosages.
 *
 * @param entry: The log entry containing user data and units.
 * @param carb_amount: The amount of carbohydrates in grams.
 * @param carb_ratio: The carb to insulin ratio (grams/unit).
 * @param insulin_sensitivity_factor The ISF in mmol/L per unit of insulin.
 * @param blood_glucose_level The user's current blood glucose level.
 * @param target_blood_glucose The user's target blood glucose level.
 * @return The total insulin dosage rounded to the nearest 0.5 units.
 */
float total_dosage(log_entry *entry,float carb_amount, float carb_ratio, 
                   int insulin_sensitivity_factor, float blood_glucose_level, 
                   float target_blood_glucose);

#endif
