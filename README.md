# Diabetes Management System

## Overview 

This project is a type 1 diabetes management program designed to help users log and monitor blood glucose, carbohydrate intake, insulin dosages,and more. It allows the user to configure insulin-related data such as the insulin sensitivity factor and provides logs for tracking and analysis. 

The program is written in C with separate files handling calculations, logging, and configuration management. 

## Features
- Log blood glucose levels, carbohydrate intake, and insulin dosages.
- Automatically calculate insulin dosages based on user-configured carb ratios, correction factors, and target blood glucose levels when applicable.
- View logs filtered by time periods (e.g.,today,past week).
- Update insulin settings through a command line interface
- Handles invalid inputs and file errors.

## How to Run
1. **Compile the Program:**
` gcc -o diabetes_manager main.c calculations.c logging.c config.c -lm`
Ensure all source files are in the same directory as the compiler command.
2. **Run the Executable:**
`./diabetes_manager`

## Dependencies
This program requires:
- A C compiler (e.g., GCC).
- Standard C libraries. 
Ensure the following files are present in the same directory.
- `config.txt` (for configuration values)
- `data/logs.txt` (for storing logs; will be created if it doesn't exist)

## Configuration
The `config.txt` file is crucial for setting user preferences and values. Here are the expected keys: 
- carb ratio: Carbohydrate-to-insulin ratio (grams/unit).

- insulin sensitivity factor: Blood glucose sensitivity factor (mmol/L/unit).

- blood glucose unit: Preferred unit for blood glucose measurement (mmol/L or mg/dL).

- target blood glucose: Target blood glucose level (mmol/L).

Example `config.txt`:
`carb ratio = 10
insulin sensitivity factor = 2
blood glucose unit = mmol/L
target blood glucose = 6.0 `

## Example Usage
### Logging a Meal Entry
1. Select `Log Entry` from the main menu. 
2. Choose `Meal` as the entry type.
3. Input the following details when prompted: 
- Blood glucose level: `5.5`
- Total carbohydrates: `45`
The program calculates and logs the insulin dosage required. 
### Viewing Logs
1. Select `View Logs` from the main menu.
2. Choose the time period (e.g., Past week).
3. The program displays log entries within selected time period.

## Note
- Logs are stored in data/logs.txt. Ensure the data directory exists before running the program.

- If the program encounters missing configuration keys, it will display an error and terminate gracefully.

- This project is for educational and personal use only. 

