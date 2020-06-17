/*
 * Katherine Wilsdon
 * CS 3337
 * 3-6-2020
 */

 #include "linked_list.h"
 #include "client.h"

 // Adds a client to the the client linked list
 void client_addClient(struct Node **head, void *data){
   linked_list_addNode(head, data);
   client_connections_num += 1;
 }

 // Deletes the client node from the linked list
 void client_deleteClient(struct Node *head, struct Node *node){
   linked_list_deleteNode(head, node);
   client_connections_num -= 1;
 }

 // Returns the size of the list of clients in the channel
 int client_connections_quantity(struct Node *head){
   int quantity = linked_list_quantity(head);
   client_connections_num = quantity;
   return quantity;
 }
