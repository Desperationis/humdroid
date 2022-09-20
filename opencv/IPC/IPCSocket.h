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
#include <cerrno>
#include <cstring>

    
class IPCSocket {
private:
    int sockfd, clientfd; 
    char* buffer; 
    struct sockaddr_in servaddr; 
	
	int port;
	std::string address;

	const int MAXBYTES = 1024;
	
public:
	IPCSocket(int port = 5005, std::string address = "127.0.0.1") {
		this->port = port;
		this->address = address;

		buffer = new char[MAXBYTES];

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

		listen(sockfd, 3);
	}

	int Close() {
		return close(clientfd);
	}

	bool Connected(bool burn = false) {
		int oobcode = recv(clientfd, buffer, 1, MSG_DONTWAIT | MSG_PEEK | MSG_OOB);
		if(oobcode > 0)
			return false;

		int code;
		if(burn)
			code = recv(clientfd, buffer, 1024, MSG_DONTWAIT);
		else
			code = recv(clientfd, buffer, 1, MSG_DONTWAIT | MSG_PEEK);

		if(code == -1 && errno == EAGAIN) {
			return true;
		}


		return code > 0;
	}


	/**
	 * Stalls until a new client is connected to this socket. MUST be called
	 * before sending or receiving.
	 *
	 * @returns Socket file descriptor of client, -1 if failure.
	*/ 
	int ListenForClient() {
		socklen_t len = sizeof(servaddr);

		clientfd = accept(sockfd, (struct sockaddr *)&servaddr, &len);

		return clientfd;
	}

	~IPCSocket() {
		shutdown(sockfd, SHUT_RDWR);
	}


	/**
	 * Receives a string up to MAXBYTES long from the socket.
	*/ 
	std::string Receive() {
		int n; 
		
		n = recv(clientfd, (char *)buffer, MAXBYTES, 0); 
		buffer[n] = '\0'; 

		return std::string(buffer);
	}


	/**
	 * Sends `buflen` amount of bytes from `buf` to socket.
	 *
	 * @returns The amount of bytes sent. -1 is returned if unable to send for
	 * whatever reason.
	*/ 
	int Send(const char* buf, size_t buflen) {
		if(buflen > MAXBYTES) 
			return -1;

		int n =send(clientfd, buf, buflen, 0); 
		std::cout<<"SEND ERROR: " << std::strerror(errno) << std::endl;

		return n;
	}


	/**
	 * If a call fails (i.e. Send) return the error that occured.
	*/ 
	std::string GetError() {
		return std::strerror(errno);
	}
};

#endif
