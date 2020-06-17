/*
 * Katherine Wilsdon
 * CS 3337
 * 3-12-2020
 */

#ifndef IRCD_SERVER_H
#define IRCD_SERVER_H

//===========================================================================//
// Global Variables
//===========================================================================//
 struct Node *server_IRC_operators_list_head;
 struct Node *server_blacklist_nicknames_head;
  struct Node *server_whitelist_nicknames_head;
 int server_IRC_operators_num;
 int server_blacklist_nicknames_num;
 int server_whitelist_nicknames_num;

//===========================================================================//
// Methods
//===========================================================================//

// Adds a client to the the IRC operator linked list
void server_addIRCOperator(struct Node **head, void *data);

// Deletes the IRC operator node from the linked list known as deopping
void server_deleteIRCOperator(struct Node *head, struct Node *node);

// Returns the size of the list of IRC operators in the server
int server_IRC_Operator_quantity(struct Node *head);



// Adds a client's nickname to the the blacklist nickname linked list
void server_addBlacklistNickname(struct Node **head, void *data);

// Deletes the blacklist nickname node from the linked list
void server_deleteBlacklistNickname(struct Node *head, struct Node *node);

// Returns the size of the list of blacklisted nicknames in the server
int server_blacklist_nickname_quantity(struct Node *head);


// Adds a client's nickname to the the whitelist nickname linked list
void server_addWhitelistNickname(struct Node **head, void *data);

// Deletes the whitelist nickname node from the linked list
void server_deleteWhitelistNickname(struct Node *head, struct Node *node);

// Returns the size of the list of whitelisted nicknames in the server
int server_whitelist_nickname_quantity(struct Node *head);

 #endif
