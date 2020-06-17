#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "config.h"

// Sets an int variable: port_num, max_client_connections, buffer_size
int config_setInt(char* line){
  char parameter[50] = { 0 };
  int name = 0;
  char ret_val = sscanf(line, "%s = %d\n", &parameter, &name);
  if (ret_val != 2){
    printf("Error in '%s' line. Could not convert parameter value to int in configuration file.\n\n.", line);
  } else{
    printf("Successfully read in %s as %d.\n\n", parameter, name);
  }
  return name;
}

// Sets a char variable: config_fileType, log_path, log_filename
char* config_setChar(char* line){
  char parameter[50] = { 0 };
  char* name = (char *)calloc(4906, sizeof(char));
  char ret_val = sscanf(line, "%s = %s\n", &parameter, &name);
  if (ret_val != 2){
    printf("Error in '%s' line. Could not convert parameter value to char in configuration file.\n\n", &line);
  } else{
    printf("Successfully read in %s as %s.\n\n", parameter, &name);
  }
  return name;
}

// Sets the logging level parameter, log_level
enum Log_Level config_setLogLevel(char* line){
  char temp[10] = { 0 };
  enum Log_Level name = 0;
  char parameter[50] = { 0 };
  char ret_val = sscanf(line, "%s = %s\n", &parameter, &temp);
  if (ret_val != 2){
    printf("Error in '%s' line. Could not convert parameter value to logging level in configuration file. Must be in the format 'logging_levels = LOG_ERR','logging_levels = LOG_ALL', 'logging_levels = NONE'.\n\n", line);
  } else{
    if (strstr(temp, "LOG_ERR")){
      name = 0;
      printf("Successfully read in %s as %s.\n\n", parameter, temp);
    } else if (strstr(temp, "LOG_ALL")){
      name = 1;
      printf("Successfully read in %s as %s.\n\n", parameter, temp);
    } else if (strstr(temp, "NONE")){
      name = 2;
      printf("Successfully read in %s as %s.\n\n", parameter, temp);
    } else{
      printf("Error in '%s' line. Did not provide a valid parameter value to logging level in configuration file. Must be in the format 'logging_levels = LOG_ERR','logging_levels = LOG_ALL', 'logging_levels = NONE'.\n\n", line);
    }
  }
  return name;
}

// Loads configuration file
void config_load(){
  char *config_file_name = "config.txt";
  char line_buffer[100];
  //fp_config = (FILE*)malloc(sizeof(FILE*));

  // If there is an error with reading the file, report it and exit.
  if ((fp_config=fopen(config_file_name, "r")) == NULL) {
    printf("Failed to open config file %s", config_file_name);
    exit(EXIT_FAILURE);
  } else {
    printf("Successfully opened config file.\n\n");
  }

  // While not end of file
  while (!(feof(fp_config))){
    fgets(line_buffer, 100, fp_config);
    if (line_buffer[0] == '/' || strlen(line_buffer) < 4) {
      continue;
    }
    if (strstr(line_buffer, "filetype ")){
      config_fileType = config_setChar(line_buffer);
    } else if (strstr(line_buffer, "port ")){
      port_num = config_setInt(line_buffer);
    } else if (strstr(line_buffer, "logging_levels ")){
      log_level = config_setLogLevel(line_buffer);
    } else if (strstr(line_buffer, "log_path ")){
      log_path =  config_setChar(line_buffer);
    } else if (strstr(line_buffer, "log_filename ")){
      log_filename = config_setChar(line_buffer);
    } else if (strstr(line_buffer, "max_client_connections ")){
      max_client_connections = config_setInt(line_buffer);
    } else if (strstr(line_buffer, "buffer_size ")){
      buffer_size = config_setInt(line_buffer);
	  break;
    } else {
      printf("Error in '%s' line. Unknown line.", line_buffer);
    }
  }

  printf("config_fileType = %s\n\n", &config_fileType);
  printf("port_num = %d\n\n", port_num);
  printf("log_level = %d\n\n", log_level);
  printf("log_path = %s\n\n", &log_path);
  printf("log_filename = %s\n\n", &log_filename);
  printf("max_client_connections = %d\n\n", max_client_connections);
  printf("buffer_size = %d\n\n", buffer_size);

  printf("Call log_init from config file.\n\n");
  log_init();
  fclose(fp_config);
  printf("Successfully closed config file.\n\n");
}
