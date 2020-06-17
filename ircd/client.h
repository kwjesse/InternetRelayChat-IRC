/*
 * Katherine Wilsdon
 * CS 3337
 * 3-6-2020
 */

#ifndef IRCD_CLIENT_H
#define IRCD_CLIENT_H

 #include <netinet/in.h>
 #include <netdb.h>
 #include <sys/socket.h>
 #include "linked_list.h"

 struct Client {
   char *client_nickname;
   char *client_full_name;
   char *client_user_name;
   struct sockaddr_in client_address; // ->sin_family, ->sin_port, ->sin_addr
   int client_fd;
   socklen_t client_length;
   char is_invisible;
   char is_IRC_operator;
 };

//===========================================================================//
// Global Variables
//===========================================================================//
 struct Node *client_list_head;
 int client_connections_num;

//===========================================================================//
// Methods
//===========================================================================//

//-----------------------------------
// TODO: Add deopping
//----------------------------------

 // Adds a client to the the client linked list
 void client_addClient(struct Node **head, void *data);

 // Deletes the client node from the linked list
 void client_deleteClient(struct Node *head, struct Node *node);

 // Returns the size of the list of clients in the channel
 int client_connections_quantity(struct Node *head);

 #endif
