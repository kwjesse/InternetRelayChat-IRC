/*
 * Katherine Wilsdon
 * CS 3337
 * 2-21-2020
 * gcc main.c log.c client.c channel.c server.c linked_list.c config.c -lpthread
 * USER and NICK commands implemented
 * JOIN #<channel> is implemented but does not support broadcasting a message to only that channel
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#include "linked_list.h"
#include "client.h"
#include "channel.h"
#include "server.h"
#include "log.h"
#include "config.h"

pthread_mutex_t lock;

void* createClient(void* data);
void* broadcastMessage(void* data);
void* writeMessageToClient(void* data);
void* joinChannel(void* data);

struct args {
	char* buffer;
	void* data;
	int fd;
	struct sockaddr_in address;
	int length;
};

int main(int argc, char **argv){
	struct sockaddr_in server_address, client_address;
	config_load();
	log_debug("Configuration file loaded.\n\n");


  // Provides sequenced, reliable, two-way, connection-based byte streams using the IPv4 Internet protocols communication domain
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd >= 0)
		log_debug("Sucessfully opened socket.\n\n");
	if (socket_fd < 0){
		if (errno == EACCES)
			log_error("ERROR opening socket: EACCES.\n\n");
		else if (errno == EAFNOSUPPORT)
			log_error("ERROR opening socket: EAFNOSUPPORT.\n\n");
		else if (errno == EINVAL)
			log_error("ERROR opening socket: EINVAL.\n\n");
		else if (errno == EINVAL)
			log_error("ERROR opening socket: EINVAL.\n\n");
		else if (errno == EMFILE)
			log_error("ERROR opening socket: EMFILE.\n\n");
		else if (errno == ENFILE)
			log_error("ERROR opening socket: ENFILE.\n\n");
		else if (errno == ENOBUFS)
			log_error("ERROR opening socket: ENOBUFS.\n\n");
		else if (errno == ENOMEM)
			log_error("ERROR opening socket: ENOMEM.\n\n");
		else if (errno == EPROTONOSUPPORT)
			log_error("ERROR opening socket: EPROTONOSUPPORT.\n\n");
		else
			log_error("ERROR opening socket: Uncaught exception.\n\n");
	}

	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
		log_error("ERROR setsockopt(SO_REUSEADDR) failed");

    // Zero out server_address
	bzero((char *) &server_address, sizeof(server_address));

  // Set the address family as AF_INET
	server_address.sin_family = AF_INET;
	log_debug("Set the address family to AF_INET.\n\n");

  // Set server_address -> address in network byte order to (0.0.0.0) for any address for binding
  server_address.sin_addr.s_addr = INADDR_ANY;
	log_debug("Set address for binding to INADDR_ANY.\n\n");

  // Set port in network byte order
  server_address.sin_port = htons(port_num); // client->client_address.sin_port
	log_debug("Set port in network.\n\n");

  // Assigns the address specified by addr to the socket referred to by the file descriptor socket_fd
	bind(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address));
	log_debug("Assigned the address to the socket.\n\n");

  // Marks the socket as a socket that will be used to accept incoming connection requests
	listen(socket_fd, 5);
	int client_len = sizeof(client_address);

  while(1){
		pthread_t pthread;
    /** It extracts the first connection request on the queue of pending connections for the listening socket
        and creates a new connected socket, and returns a new file descriptor referring to that socket. */
		int accept_newsockfd = accept(socket_fd, (struct sockaddr *) &client_address, &client_len);
		if (accept_newsockfd >= 0)
			log_debug("The new socket was accepted and connected.\n\n");
		if (accept_newsockfd < 0){
			if (errno == EAGAIN)
				log_error("ERROR on accept: EAGAIN.\n\n");
			else if (errno == EWOULDBLOCK)
				log_error("ERROR on accept: EWOULDBLOCK.\n\n");
			else if (errno == EBADF)
				log_error("ERROR on accept: EBADF.\n\n");
			else if (errno == ECONNABORTED)
				log_error("ERROR on accept: ECONNABORTED.\n\n");
			else if (errno == EINTR)
				log_error("ERROR on accept: EINTR.\n\n");
			else if (errno == EINVAL)
				log_error("ERROR on accept: EINVAL.\n\n");
			else if (errno == EMFILE)
				log_error("ERROR on accept: EMFILE.\n\n");
			else if (errno == ENFILE)
				log_error("ERROR on accept: ENFILE.\n\n");
			else if (errno == ENOTSOCK)
				log_error("ERROR on accept: ENOTSOCK.\n\n");
			else if (errno == EOPNOTSUPP)
				log_error("ERROR on accept: EOPNOTSUPP.\n\n");
			else if (errno == ENOBUFS)
				log_error("ERROR on accept: ENOBUFS.\n\n");
			else if (errno == ENOMEM)
				log_error("ERROR on accept: ENOMEM.\n\n");
			else if (errno == EPROTO)
				log_error("ERROR on accept: EPROTO.\n\n");
			else
				log_error("ERROR on accept: Uncaught exception.\n\n");
		}
		// Create a thread that will ask for USER and NICK command
		struct args* arg = malloc(sizeof(struct args));
		arg->fd = accept_newsockfd;
		arg->length = client_len;
		arg->address = client_address;
		pthread_create(&pthread, NULL, createClient, (void *)arg);
		int detach_val = pthread_detach(pthread);

	}
	log_close();
	
	return 0;
}

// Prompts the client for USER and NICK commands and creates a client 
void* createClient(void* data){
	struct args* arg = (struct args*)data;
	int accept_newsockfd = arg->fd;
	pthread_t pthread;

	struct Client* client_data = malloc(sizeof(struct Client));
	client_data->client_address = arg->address;
	client_data->client_length = arg->length;
	client_data->client_fd = accept_newsockfd;
	client_data->is_invisible = '0';
	client_data->is_IRC_operator = '0';
		
	char* nickname = calloc(buffer_size, sizeof(char));
	char* full_name = calloc(buffer_size,sizeof(char));
	char* user_name = calloc(buffer_size, sizeof(char));

	char isCorrectUserCommand = '0';
		
	while(isCorrectUserCommand == '0'){
		// Write USER command to client
		char* buffer_user_w = "Enter command: USER <username> <hostname> <servername> :<realname>";
		int write_user_val = write(client_data->client_fd, buffer_user_w, strlen(buffer_user_w));
		if (write_user_val > 0){
			log_debug("Wrote message for USER command.\n\n");
		} else if (write_user_val == 0){
			log_error("ERROR Nothing was written.\n\n");
		} else if (write_user_val < 0){
			if (errno == EAGAIN)
				log_error("ERROR writing to socket for USER: EAGAIN.\n\n");
			else if (errno == EWOULDBLOCK)
				log_error("ERROR writing to socket for USER: EWOULDBLOCK.\n\n");
			else if (errno == EBADF)
				log_error("ERROR writing to socket for USER: EBADF.\n\n");
			else if (errno == EDESTADDRREQ)
				log_error("ERROR writing to socket for USER: EDESTADDRREQ.\n\n");
			else if (errno == EDQUOT)
				log_error("ERROR writing to socket for USER: EDQUOT.\n\n");
			else if (errno == EFBIG)
				log_error("ERROR writing to socket for USER: EFBIG.\n\n");
			else if (errno == EFAULT)
				log_error("ERROR writing to socket for USER: EFAULT.\n\n");
			else if (errno == EINTR)
				log_error("ERROR writing to socket for USER: EINTR.\n\n");
			else if (errno == EINVAL)
				log_error("ERROR writing to socket for USER: EINVAL.\n\n");
			else if (errno == EIO)
				log_error("ERROR writing to socket for USER: EIO.\n\n");
			else if (errno == ENOSPC)
				log_error("ERROR writing to socket for USER: ENOSPC.\n\n");
			else if (errno == EPIPE)
				log_error("ERROR writing to socket for USER: EPIPE.\n\n");
			else
				log_error("ERROR writing to socket for USER: Uncaught exception.\n\n");
		}	

		//Read USER command from client
		char buffer_user_r[buffer_size];
		int read_user_val = read(client_data->client_fd, buffer_user_r, buffer_size-1);
		if (read_user_val > 0){
			log_debug("Read message from client for USER command.\n\n");
		} else if (read_user_val == 0){
			log_debug("Thread is closing down, EOF recieved.\n\n");
			break;
		} else if (read_user_val < 0){
			if (errno == EAGAIN)
				log_error("ERROR reading from socket for USER: EAGAIN.\n\n");
			else if (errno == EWOULDBLOCK)
				log_error("ERROR reading from socket for USER: EWOULDBLOCK.\n\n");
			else if (errno == EBADF)
				log_error("ERROR reading from socket for USER: EBADF.\n\n");
			else if (errno == ECONNREFUSED)
				log_error("ERROR reading from socket for USER: ECONNREFUSED.\n\n");
			else if (errno == EFAULT)
				log_error("ERROR reading from socket for USER: EFAULT.\n\n");
			else if (errno == EINTR)
				log_error("ERROR reading from socket for USER: EINTR.\n\n");
			else if (errno == EINVAL)
				log_error("ERROR reading from socket for USER: EINVAL.\n\n");
			else if (errno == ENOMEM)
				log_error("ERROR reading from socket for USER: ENOMEM.\n\n");
			else if (errno == ENOTCONN)
				log_error("ERROR reading from socket for USER: ENOTCONN.\n\n");
			else if (errno == ENOTSOCK)
				log_error("ERROR reading from socket for USER: ENOTSOCK.\n\n");
			else
				log_error("ERROR reading from socket for USER: Uncaught exception.\n\n");
		}
		char* server_name;
		char* host_name;
 		buffer_user_r[strcspn(buffer_user_r, "\n")] = '\0'; 
		int read_user_sscanf = sscanf(buffer_user_r, "USER %s %s %s :%s", user_name, &host_name, &server_name, full_name);
		if (read_user_sscanf == 4){
			isCorrectUserCommand = '1';
			client_data->client_full_name = full_name;
			client_data->client_user_name = user_name;
		}
	}	

	char isCorrectNickCommand = '0';
		
	while(isCorrectNickCommand == '0'){
		// Write NICK command to client
		char* buffer_nick_w = "Enter command: NICK <nickname>";
		int write_nick_val = write(client_data->client_fd, buffer_nick_w, strlen(buffer_nick_w));
		if (write_nick_val > 0){
			log_debug("Wrote message for NICK command.\n\n");
		} else if (write_nick_val == 0){
			log_error("ERROR Nothing was written.\n\n");
		} else if (write_nick_val < 0){
			if (errno == EAGAIN)
				log_error("ERROR writing to socket for NICK: EAGAIN.\n\n");
			else if (errno == EWOULDBLOCK)
				log_error("ERROR writing to socket for NICK: EWOULDBLOCK.\n\n");
			else if (errno == EBADF)
				log_error("ERROR writing to socket for NICK: EBADF.\n\n");
			else if (errno == EDESTADDRREQ)
				log_error("ERROR writing to socket for NICK: EDESTADDRREQ.\n\n");
			else if (errno == EDQUOT)
				log_error("ERROR writing to socket for NICK: EDQUOT.\n\n");
			else if (errno == EFBIG)
				log_error("ERROR writing to socket for NICK: EFBIG.\n\n");
			else if (errno == EFAULT)
				log_error("ERROR writing to socket for NICK: EFAULT.\n\n");
			else if (errno == EINTR)
				log_error("ERROR writing to socket for NICK: EINTR.\n\n");
			else if (errno == EINVAL)
				log_error("ERROR writing to socket for NICK: EINVAL.\n\n");
			else if (errno == EIO)
				log_error("ERROR writing to socket for NICK: EIO.\n\n");
			else if (errno == ENOSPC)
				log_error("ERROR writing to socket for NICK: ENOSPC.\n\n");
			else if (errno == EPIPE)
				log_error("ERROR writing to socket for NICK: EPIPE.\n\n");
			else
				log_error("ERROR writing to socket for NICK: Uncaught exception.\n\n");
		}	

		//Read NICK command from client
		char buffer_nick_r[buffer_size];
		int read_nick_val = read(client_data->client_fd, buffer_nick_r, buffer_size-1);
		if (read_nick_val > 0){
			log_debug("Read message from client for NICK command.\n\n");
		} else if (read_nick_val == 0){
			log_debug("Thread is closing down, EOF recieved.\n\n");
			break;
		} else if (read_nick_val < 0){
			if (errno == EAGAIN)
				log_error("ERROR reading from socket for NICK: EAGAIN.\n\n");
			else if (errno == EWOULDBLOCK)
				log_error("ERROR reading from socket for NICK: EWOULDBLOCK.\n\n");
			else if (errno == EBADF)
				log_error("ERROR reading from socket for NICK: EBADF.\n\n");
			else if (errno == ECONNREFUSED)
				log_error("ERROR reading from socket for NICK: ECONNREFUSED.\n\n");
			else if (errno == EFAULT)
				log_error("ERROR reading from socket for NICK: EFAULT.\n\n");
			else if (errno == EINTR)
				log_error("ERROR reading from socket for NICK: EINTR.\n\n");
			else if (errno == EINVAL)
				log_error("ERROR reading from socket for NICK: EINVAL.\n\n");
			else if (errno == ENOMEM)
				log_error("ERROR reading from socket for NICK: ENOMEM.\n\n");
			else if (errno == ENOTCONN)
				log_error("ERROR reading from socket for NICK: ENOTCONN.\n\n");
			else if (errno == ENOTSOCK)
				log_error("ERROR reading from socket for NICK: ENOTSOCK.\n\n");
			else
				log_error("ERROR reading from socket for NICK: Uncaught exception.\n\n");
		}
		// Remove trailing newline character
		buffer_nick_r[strcspn(buffer_nick_r, "\n")] = '\0'; 
		int read_nick_sscanf = sscanf(buffer_nick_r, "NICK %s", nickname);
		if (read_nick_sscanf == 1){
			isCorrectNickCommand = '1';
			client_data->client_nickname = nickname;
		}
	}
	pthread_mutex_lock(&lock);
	client_addClient(&client_list_head, client_data);
	pthread_mutex_unlock(&lock);

	struct args* arg_data = malloc(sizeof(struct args));
	arg_data->data = client_data;
	// Puts each client on a thread
	pthread_create(&pthread, NULL, broadcastMessage, (void *)arg_data);
	int detach_val = pthread_detach(pthread);
	return 0;
}

// Handles reading and writing and the JOIN command for each client
void* broadcastMessage(void* data){
	struct args* arg = (struct args*) data;
	struct Client* client_data = (struct Client*) arg->data;
	while(1){
		char* buffer = (char*)calloc(buffer_size, sizeof(char));
		// Read message from the client
		int read_val = read(client_data->client_fd, buffer, buffer_size-1);
		if (read_val > 0){
			log_debug("Read message from @%s.\n\n", client_data->client_nickname);
		} else if (read_val == 0){
			log_debug("Thread is closing down, EOF recieved.\n\n");
			struct Node* node = linked_list_getNode(client_list_head, (void *)client_data);
			pthread_mutex_lock(&lock);
			linked_list_deleteNode(client_list_head, node);
			pthread_mutex_unlock(&lock);
			free(buffer);
			break;
		} else if (read_val < 0){
			if (errno == EAGAIN)
				log_error("ERROR reading from socket: EAGAIN.\n\n");
			else if (errno == EWOULDBLOCK)
				log_error("ERROR reading from socket: EWOULDBLOCK.\n\n");
			else if (errno == EBADF)
				log_error("ERROR reading from socket: EBADF.\n\n");
			else if (errno == ECONNREFUSED)
				log_error("ERROR reading from socket: ECONNREFUSED.\n\n");
			else if (errno == EFAULT)
				log_error("ERROR reading from socket: EFAULT.\n\n");
			else if (errno == EINTR)
				log_error("ERROR reading from socket: EINTR.\n\n");
			else if (errno == EINVAL)
				log_error("ERROR reading from socket: EINVAL.\n\n");
			else if (errno == ENOMEM)
				log_error("ERROR reading from socket: ENOMEM.\n\n");
			else if (errno == ENOTCONN)
				log_error("ERROR reading from socket: ENOTCONN.\n\n");
			else if (errno == ENOTSOCK)
				log_error("ERROR reading from socket: ENOTSOCK.\n\n");
			else
				log_error("ERROR reading from socket: Uncaught exception.\n\n");
		}
		pthread_t pthread_write;
		
		arg->buffer = buffer;
		// If a client wants to join a channel
		buffer[strcspn(buffer, "\n")] = '\0';	
		int join_ret_val = strncmp(buffer,"JOIN", 4);
		if (join_ret_val == 0){
			pthread_create(&pthread_write, NULL, joinChannel, (void *)arg);
			int detach_val = pthread_detach(pthread_write);
			if (detach_val != 0){
				if (errno == EINVAL){
					log_error("Thread is not joinable.\n\n");
				} else if (errno == ESRCH){
					log_error("No thread with the ID thread could be found.\n\n");
				} else{
					log_error("Error unhandled exception on pthread_detached.\n\n");
				}
			}	

		} else {	
			// Else write message to every client
			pthread_create(&pthread_write, NULL, writeMessageToClient, (void *)arg);
			int detach_val = pthread_detach(pthread_write);
			if (detach_val != 0){
				if (errno == EINVAL){
					log_error("Thread is not joinable.\n\n");
				} else if (errno == ESRCH){
					log_error("No thread with the ID thread could be found.\n\n");
				} else{
					log_error("Error unhandled exception on pthread_detached.\n\n");
				}
			}	
		}
	}
	
	return 0;
}

// Writes a message to every client
void* writeMessageToClient(void* data){
	struct args* arg = (struct args*) data;
	struct Client* client_data = (struct Client*) arg->data;
	char* buffer = (char *)arg->buffer;
	int quantity = linked_list_quantity(client_list_head);
	struct Node* node = client_list_head;
	struct Client* node_data;
	if (node != NULL){
		node_data = (struct Client*) node->data;
	}
	struct Node* temp = client_list_head;
	for (int i = 0; i < quantity; ++i){
		temp = linked_list_getNextNode(node);
		// Skip client that sent the message
		if (node_data == client_data){
			if (temp != NULL){
				node = temp;
				node_data = (struct Client*) node->data;
			}
		} else {
			// Writes message
				int write_val = write(node_data->client_fd, buffer, buffer_size);
			if (write_val > 0){
				log_debug("Wrote message to @%s.\n\n", node_data->client_nickname);
			} else if (write_val == 0){
				log_error("ERROR Nothing was written.\n\n");
			} else if (write_val < 0){
				if (errno == EAGAIN)
					log_error("ERROR writing to socket: EAGAIN.\n\n");
				else if (errno == EWOULDBLOCK)
					log_error("ERROR writing to socket: EWOULDBLOCK.\n\n");
				else if (errno == EBADF)
					log_error("ERROR writing to socket: EBADF.\n\n");
				else if (errno == EDESTADDRREQ)
					log_error("ERROR writing to socket: EDESTADDRREQ.\n\n");
				else if (errno == EDQUOT)
					log_error("ERROR writing to socket: EDQUOT.\n\n");
				else if (errno == EFBIG)
					log_error("ERROR writing to socket: EFBIG.\n\n");
				else if (errno == EFAULT)
					log_error("ERROR writing to socket: EFAULT.\n\n");
				else if (errno == EINTR)
					log_error("ERROR writing to socket: EINTR.\n\n");
				else if (errno == EINVAL)
					log_error("ERROR writing to socket: EINVAL.\n\n");
				else if (errno == EIO)
					log_error("ERROR writing to socket: EIO.\n\n");
				else if (errno == ENOSPC)
					log_error("ERROR writing to socket: ENOSPC.\n\n");
				else if (errno == EPIPE)
					log_error("ERROR writing to socket: EPIPE.\n\n");
				else
					log_error("ERROR writing to socket: Uncaught exception.\n\n");
			}

			if (temp != NULL){
				node = temp;
				node_data = (struct Client*) node->data;
			}
		}
	}
	return 0;
}

// Creates a channel or joins an existing channel
void* joinChannel(void* data){
	struct args* arg = (struct args*) data;
	struct Client* client_data = (struct Client*) arg->data;
	char* buffer = (char *)arg->buffer;
	char* channel_name = (char*)calloc(buffer_size, sizeof(char));
	int join_val = sscanf(buffer, "JOIN #%s", channel_name);
	if (join_val == 1){
		struct Node* node = channel_list_head;
		struct Channel* channel_data;
		if (node != NULL){
			channel_data = (struct Channel*)node->data;
		}
		struct Node* temp = channel_list_head;
		int quantity = linked_list_quantity(channel_list_head);
		char foundChannel = '0';
		for (int i = 0; i < quantity; ++i){
			temp = linked_list_getNextNode(node);
			// Join existing channel
			if (strcmp(channel_data->channel_name,channel_name) == 0){
				pthread_mutex_lock(&lock);
				channel_joinChannel(channel_data, client_data);
				pthread_mutex_unlock(&lock);
				foundChannel = '1';
				log_debug("@%s joined an existing channel called #%s.\n\n", client_data->client_nickname, channel_name);
				char* message = "You have joined an existing channel\n";
				int write_val = write(client_data->client_fd, message, buffer_size);
				break;
			} else{
				if (temp != NULL){
					node = temp;
					channel_data = (struct Channel*) node->data;
				}
			}	
		}
		// Create a new channel if not found in the channel list
		if (foundChannel == '0'){
			pthread_mutex_lock(&lock);
			channel_createChannel(channel_name, client_data);
			pthread_mutex_unlock(&lock);
			log_debug("@%s created new a new channel called #%s.\n\n", client_data->client_nickname, channel_name);
			char* message = "You have created a new channel\n";
			int write_val = write(client_data->client_fd, message, buffer_size);
		}		

	}
	return 0;

}
