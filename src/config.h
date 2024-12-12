#ifndef CONFIG_H
#define CONFIG_H

/**
 * trimewhitespace - Removes leading and trailing whitespace from a string
 * 
 * @param str: Input string thats need to be trimmed.
 * @return: A pointer to the trimmed string.
 */
char *trimwhitespace(char *str);

/**
 * read_config - Reads a configuration value from a file based on a key.
 * 
 * @param key: The configuration key to search for.
 * @return: The value corresponding to the key, or NULL if key is not found. 
 */
const char* read_config(const char *key);


/**
 * list_config - Lists the configurations in a specified file.
 * 
 * @param filename: The name of the configuration file.
 * @return: 0 on success, -1 if error occurs.
 */
int list_config(const char *filename);


/**
 * update_config - Updates a configuration value based on a key
 * 
 * @param filename: Name of configuration file
 * @param key: Configuration key to update.
 * @param new_value: The new value for the key.
 * @return: 0 on success, -1 for errors.
 */
int update_config(const char *filename,const char *key, const char *new_value);


#endif 