/*
 * Katherine Wilsdon
 * CS 3337
 * 2-21-2020
 */

# include <stdlib.h>
#include "linked_list.h"

/*
 * Adds a node to the front of the linked list
 */
void linked_list_addNode(struct Node **head, void *data){
  struct Node* node = malloc(sizeof(struct Node));
  node->next = *head;
  node->data = data;
  *head = node;
}

/*
 * Gets the next node from the provided node
 */
struct Node* linked_list_getNextNode(struct Node *node){
  return node->next;
}

/*
 * Returns the node that contains the provided data
 */
struct Node* linked_list_getNode(struct Node *head, void *data){
  struct Node *node = head;
  while (node->data != data && node != NULL){
    node = node->next;
  }
  return node;
}

/*
 * Deletes the node from the linked list
 */
void linked_list_deleteNode(struct Node *head, struct Node *node){
  struct Node *ptr = head;
  if (ptr != node){
	while (ptr->next != node && ptr->next != NULL && ptr != NULL){
      ptr = ptr->next;
    }
    if (ptr != NULL && ptr->next == node){
      ptr->next = node->next;
      free(node);
    }
  } else {
    ptr = node->next;
    free(node);
  }
}

/*
 * Returns the size of the linked list
 */
int linked_list_quantity(struct Node *head){
  int size = 0;
  struct Node *ptr = head;
  while (ptr != NULL){
    ptr = ptr->next;
    size++;
  }
  return size;
}
