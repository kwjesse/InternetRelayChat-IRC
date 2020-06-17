/*
 * Katherine Wilsdon
 * CS 3337
 * 3-12-2020
 */

 #include "linked_list.h"
 #include "server.h"

 // Adds a client to the the IRC operator linked list
 void server_addIRCOperator(struct Node **head, void *data){
   linked_list_addNode(head, data);
   server_IRC_operators_num += 1;
 }

 // Deletes the IRC operator node from the linked list known as deopping
 void server_deleteIRCOperator(struct Node *head, struct Node *node){
   linked_list_deleteNode(head, node);
   server_IRC_operators_num -= 1;
 }

 // Returns the size of the list of IRC operators in the server
 int server_IRC_Operator_quantity(struct Node *head){
   int quantity = linked_list_quantity(head);
   server_IRC_operators_num = quantity;
   return quantity;
 }

 // Adds a client's nickname to the the blacklist nickname linked list
 void server_addBlacklistNickname(struct Node **head, void *data){
   linked_list_addNode(head, data);
   server_blacklist_nicknames_num += 1;
 }

 // Deletes the blacklist nickname node from the linked list
 void server_deleteBlacklistNickname(struct Node *head, struct Node *node){
   linked_list_deleteNode(head, node);
   server_blacklist_nicknames_num -= 1;
 }

 // Returns the size of the list of blacklisted nicknames in the server
 int server_blacklist_nickname_quantity(struct Node *head){
   int quantity = linked_list_quantity(head);
   server_blacklist_nicknames_num = quantity;
   return quantity;
 }

 // Adds a client's nickname to the the whitelist nickname linked list
 void server_addWhitelistNickname(struct Node **head, void *data){
   linked_list_addNode(head, data);
   server_whitelist_nicknames_num += 1;
 }

 // Deletes the whitelist nickname node from the linked list
 void server_deleteWhitelistNickname(struct Node *head, struct Node *node){
   linked_list_deleteNode(head, node);
   server_whitelist_nicknames_num -= 1;
 }

 // Returns the size of the list of whitelisted nicknames in the server
 int server_whitelist_nickname_quantity(struct Node *head){
   int quantity = linked_list_quantity(head);
   server_whitelist_nicknames_num = quantity;
   return quantity;
 }
