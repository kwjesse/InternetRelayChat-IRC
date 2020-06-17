/*
 * Katherine Wilsdon
 * CS 3337
 * 3-6-2020
 */

 #ifndef IRCD_CHANNEL_H
 #define IRCD_CHANNEL_H

 #include "linked_list.h"
 #include "client.h"

 struct Channel {
   // Channel name beginning with a '&' or '#' character of length up to 200 characters, no spaces or commas
   char *channel_name;
   struct Node *channel_operator_list_head;
   struct Node *channel_client_list_head;
   char is_private_channel;
   char is_secret_channel;
   char is_invite_only_channel;
   char *channel_topic;
   char has_no_messages_from_outside_channel;
   char is_moderated_channel;
   char has_user_limit;
   int user_limit_num;
   struct Node *channel_ban_list_head;
   char can_speak_on_moderated_channel;
   char has_channel_key;
   char *channel_key;
   int channel_client_num;
 };

 //===========================================================================//
 // Global Variables
 //===========================================================================//
  struct Node *channel_list_head;
  int channel_num;

 //===========================================================================//
 // Methods
 //===========================================================================//

  // Eject a client from the channel
   void channel_operator_kick(struct Channel *channel, struct Node *channel_operator, struct Node *head, struct Node *node);

  // Change the channel topic in a channel
  void channel_operator_changeTopic(struct Channel *channel, struct Node *channel_operator, struct Node *head, char *channel_topic);

  // Create channel and set the channel operator to be the client
  void channel_createChannel(char* channel_name, struct Client* client_data);  

  // Adds a client to the the client linked list of channel
  void channel_joinChannel(struct Channel *channel, struct Client*client_data);

  // Deletes the client node from the linked list
  void channel_deleteClient(struct Channel *channel, struct Node *head, struct Node *node);

  // Deletes the channel when last client leaves
  void channel_deleteChannel(struct Channel *channel, struct Node *head, struct Node *node);

  // Returns the number of channels in the IRC
  int channel_channels_quantity(struct Node *head);

  // Returns the number of clients in the channel
  int channel_clients_quantity(struct Channel *channel, struct Node *head);

 #endif
