#ifndef IPCSOCK_H
#define IPCSOCK_H

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <string>
#include <iostream>

    
class IPCSocket {
private:
    int sockfd, clientfd; 
    char* buffer; 
    struct sockaddr_in servaddr; 
	
	int port;
	std::string address;

	const int MAXLINE = 1024;
	
public:
	IPCSocket(int port = 5005, std::string address = "127.0.0.1") {
		this->port = port;
		this->address = address;

		buffer = new char[MAXLINE];

		// Create UDP socket
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if(sockfd < 0) {
			perror("Socket creation failed!");
			exit(EXIT_FAILURE);
		}

		memset(&servaddr, 0, sizeof(servaddr)); 

		servaddr.sin_family = AF_INET; 
		servaddr.sin_port = htons(port); 
		servaddr.sin_addr.s_addr = inet_addr(address.c_str()); 

		// Bind to localhost
		int bindCode = bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
		if(bindCode < 0) { 
			perror("Bind failed!"); 
			exit(EXIT_FAILURE); 
		} 
	}

	void listenForClient() {
		listen(sockfd, 3);
		socklen_t len = sizeof(servaddr);

		std::cout << "Listening for client.." << std::endl;
		clientfd = accept(sockfd, (struct sockaddr *)&servaddr, &len);
		std::cout << "Client " << clientfd << std::endl;
	}

	~IPCSocket() {
		shutdown(sockfd, SHUT_RDWR);
	}

	std::string receive() {
		int n; 
		
		n = recv(clientfd, (char *)buffer, MAXLINE, 0); 
		buffer[n] = '\0'; 

		return std::string(buffer);
	}

	/**
	 * Sends `buflen` amount of bytes from `buf` to socket.
	*/ 
	int Send(const char* buf, size_t buflen) {
		if(buflen > MAXLINE) 
			return -1;

		return send(clientfd, buf, buflen, 0); 
	}
};

#endif
