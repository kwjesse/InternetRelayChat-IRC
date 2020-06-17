#ifndef IRCD_CONFIG_H
#define IRCD_CONFIG_H

//===========================================================================//
// Global Variables
//===========================================================================//

char* config_fileType;
int port_num;
int max_client_connections;
int buffer_size;
FILE *fp_config;

//===========================================================================//
// Methods
//===========================================================================//

// Sets an int variable: port_num, max_client_connections, buffer_size
int config_setInt(char* line);

// Sets a char variable: config_fileType, log_path, log_filename
char* config_setChar(char* line);

// Sets the logging level parameter, log_level
enum Log_Level config_setLogLevel(char* line);

// Loads configuration file
void config_load();

#endif
