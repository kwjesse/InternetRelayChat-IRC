#ifndef IRCD_LOG_H
#define IRCD_LOG_H

enum Log_Level{
    LOG_ERR,
    LOG_ALL,
    NONE
};

//===========================================================================//
// Global Variables
//===========================================================================//

enum Log_Level log_level;
char* log_path;
char* log_filename;
FILE* log_fp;

//===========================================================================//
// Methods
//===========================================================================//

// Return the time in a format to be put into the file name
char* log_getTime();

// Create the path to file including the file name
char* log_createFullPath();

// Create a file at the path with the file name provided
void log_init();

// Provide a concise version of logging levels
void log_error(char* format, ...);

// Provide a detailed version of logging levels used for debugging
void log_debug(char* format, ...);

// Flushes the file
void log_flush();

// Properly closes the file
void log_close();

#endif
