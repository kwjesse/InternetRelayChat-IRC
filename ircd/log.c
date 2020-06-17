#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>

#include "log.h"

// Return the time in a format to be put into the file name
char* log_getTime(){
  char * buffer = (char *)malloc(sizeof(char)*50);
  time_t raw_time;
  time(&raw_time);
  strftime(buffer, 50, "-%Y-%m-%d--%Hh-%Mm-%Ss", localtime(&raw_time));
  return buffer;
}

// Create the path to file including the file name
char* log_createFullPath(){
  char *path = (char *)malloc(sizeof(char)*6000);
  char *timestamp = log_getTime();
  sprintf(path, "%s/%s%s.log", &log_path, &log_filename, timestamp);
  return path;
}

// Create a file at the path with the file name provided
void log_init(){
  char* full_path = (char *)malloc(sizeof(char)*6000);

  if (log_path != NULL && log_filename != NULL){
    // Open file
    full_path = log_createFullPath();
    log_fp = fopen(full_path, "a");
	  assert(log_fp != NULL);
    // If fopen fails, revert to stdout
    if (log_fp == NULL){
      log_fp = stdout;
      fprintf(log_fp, "Failed to create log file\n");
    } else {
      printf("Successfully opened log file.\n\n");
    }
  }
  else{
    if (log_fp != NULL && log_fp != stdout)
      fclose(log_fp);
    log_fp = stdout;
  }
}

// Provide a concise version of logging levels
void log_error(char* format, ...){
  char time[50] = { 0 };
  // If log_level is not set to NONE (no logging), write to log file
  // Note: when log_level is LOG_ALL(debug/detailed), LOG_ERR(concise) calls are included
  if(log_level != 2){
    // Initializing arguments to store all values after format
    va_list arguments;
    va_start (arguments, format);
    // Prints time and format with arguments
    char * timestamp = log_getTime();
    fprintf(log_fp, "%s ", timestamp);
    vfprintf (log_fp, format, arguments);
    // Cleans up the list
    va_end (arguments);
    fflush(log_fp);
  }
}

// Provide a detailed version of logging levels used for debugging
void log_debug(char* format, ...){
  char time[50] = { 0 };
  // If log_level is not set to LOG_ERR(concise) or NONE (no logging), write to log file
  if(log_level != 0 && log_level != 2){
    // Initializing arguments to store all values after format
    va_list arguments;
    va_start (arguments, format);
    // Prints time and format with arguments
    char * timestamp = log_getTime();
    fprintf(log_fp, "%s ", timestamp);
    vfprintf (log_fp, format, arguments);
    // Cleans up the list
    va_end (arguments);
	fflush(log_fp);
 }
}

// Flushes the file
void log_flush(){
  fflush(log_fp);
}

// Properly closes the file
void log_close(){
  if(log_fp != stdout)
    fclose(log_fp);
  log_fp = NULL;
}
