/*
 * Katherine Wilsdon
 * CS 3337
 * 3-6-2020
 */

#ifndef IRCD_LINKED_LIST_H
#define IRCD_LINKED_LIST_H

struct Node {
  struct Node *next;
  void *data;
};

//===========================================================================//
// Methods
//===========================================================================//

// Adds a node to the front of the linked list
void linked_list_addNode(struct Node **head, void *data);

// Gets the next node from the provided node
struct Node* linked_list_getNextNode(struct Node *node);

// Returns the node that contains the provided data
struct Node* linked_list_getNode(struct Node *head, void *data);

// Deletes the node from the linked list
void linked_list_deleteNode(struct Node *head, struct Node *node);

// Returns the size of the linked list
int linked_list_quantity(struct Node *head);

#endif
