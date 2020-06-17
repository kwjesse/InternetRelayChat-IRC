/*
 * Katherine Wilsdon
 * CS 3337
 * 3-9-2020
 */

 #include "linked_list.h"
 #include "client.h"
 #include "channel.h"
 #include <assert.h>
 #include <stdlib.h>

 // Eject a client from the channel
 void channel_operator_kick(struct Channel *channel, struct Node *channel_operator, struct Node *head, struct Node *node){
   struct Node *operator = linked_list_getNode(head, channel_operator->data);
   assert(operator == channel_operator);
   channel_deleteClient(channel, head, node);
 }

// Change the channel topic in a channel
void channel_operator_changeTopic(struct Channel *channel, struct Node *channel_operator, struct Node *head, char *channel_topic){
  struct Node *operator = linked_list_getNode(head, channel_operator->data);
  assert(operator == channel_operator);
  channel->channel_topic = channel_topic;
}

// Create channel and set the channel operator to be the client
void channel_createChannel(char* channel_name, struct Client* client_data){
	struct Channel* channel = (struct Channel*)malloc(sizeof(struct Channel));
	channel->channel_name = channel_name;
	// Add to operator list
	linked_list_addNode(&channel->channel_operator_list_head, (void *)client_data);
   	// Ad to client list
	linked_list_addNode(&channel->channel_client_list_head, (void *)client_data);
	channel->channel_client_num += 1;
	// Add to channel list
	linked_list_addNode(&channel_list_head, channel);
	channel_num += 1;
}

// Adds a client to the the client linked list of the channel
void channel_joinChannel(struct Channel *channel, struct Client*client_data){
 	linked_list_addNode(&channel->channel_client_list_head, client_data);
  	channel->channel_client_num += 1;
}

// Deletes the client node from the linked list
void channel_deleteClient(struct Channel *channel, struct Node *head, struct Node *node){
  if (channel->channel_client_num > 1){
    linked_list_deleteNode(head, node);
    channel->channel_client_num -= 1;
  } else {
    channel_deleteChannel(channel, head, node);
  }
}

// Deletes the channel when last client leaves
void channel_deleteChannel(struct Channel *channel, struct Node *head, struct Node *node){
  assert(channel_clients_quantity(channel, head) == 1);
  linked_list_deleteNode(head, node);
  channel_num -= 1;
}

// Returns the number of channels in the IRC
int channel_channels_quantity(struct Node *head){
  int quantity = linked_list_quantity(head);
  channel_num = quantity;
  return quantity;
}

// Returns the number of clients in the channel
int channel_clients_quantity(struct Channel *channel, struct Node *head){
  int quantity = linked_list_quantity(head);
  channel->channel_client_num = quantity;
  return quantity;
}
