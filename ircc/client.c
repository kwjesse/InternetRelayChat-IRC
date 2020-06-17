/*
 * Katherine Wilsdon
 * CS 3337
 * 2-21-2020
 * gcc client.c -lpthread
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void error(char *msg)
{
	perror(msg);
	exit(0);
}
void* writeToServer(void* data);
void* readFromServer(void* data);

int main(int argc, char **argv) {
	int sockfd, port_num, n;

	struct sockaddr_in server_address;
	struct hostent *server;
	char buffer[256];

  // Throws error when the command line arguments of hostname and port are not provided
	if (argc < 3) {
		fprintf(stderr,"Usage %s hostname port\n", argv[0]);
		exit(0);
	}

  // Sets port number
	port_num = atoi(argv[2]);

  // Provides sequenced, reliable, two-way, connection-based byte streams using the IPv4 Internet protocols communication domain
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

  // Gets the provided host name
  server = gethostbyname(argv[1]);
  if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
  // Zero out server_address
	bzero((char *) &server_address, sizeof(server_address));

  // Set the address family as AF_INET
  server_address.sin_family = AF_INET;

  // Copy addresss from server to server_address -> address family -> address in network byte order
  bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);

  // Set port in network byte order
  server_address.sin_port = htons(port_num);

  // Connects the socket referred to by the file descriptor sockfd to the address specified by addr
	if (connect(sockfd,(struct sockaddr *)&server_address, sizeof(server_address)) < 0)
		error("ERROR connecting");

  while (1) {
    pthread_t pthread_write;
		pthread_t pthread_read;

		pthread_create(&pthread_write, NULL, writeToServer, (void *)sockfd);
		pthread_create(&pthread_read, NULL, readFromServer, (void *)sockfd);
		pthread_detach(pthread_self());
	}	
	close(sockfd);
	return 0;
}

void* writeToServer(void* data){
	int sockfd = (int) data;
	char buffer[256];
	while(1){
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);

		// Write a message
		int write_val = write(sockfd, buffer, strlen(buffer));
		if (write_val < 0){
				error("ERROR writing to socket");
				break;
		}
	}
	return 0;
}

void* readFromServer(void* data){
	int sockfd = (int) data;
	char buffer[256];
	while (1){
		bzero(buffer, 256);

		int read_val = read(sockfd,buffer,255);
		if (read_val < 0){
			error("ERROR reading from socket");
			break;
		}
		printf("%s\n",buffer);
	}
	return 0;
}
