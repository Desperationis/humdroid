// Server side implementation of UDP client-server model 
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
    int sockfd; 
    char* buffer; 
    struct sockaddr_in servaddr, cliaddr; 
    // const char *hello = "Hello from server"; 
	
	const int PORT = 5005;
	const int MAXLINE = 1024;
	
public:
	IPCSocket() {
		buffer = new char[MAXLINE];

		// Create UDP socket
		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		if(sockfd < 0) {
			perror("Socket creation failed!");
			exit(EXIT_FAILURE);
		}

		memset(&servaddr, 0, sizeof(servaddr)); 
		memset(&cliaddr, 0, sizeof(cliaddr)); 

		servaddr.sin_family = AF_INET; 
		servaddr.sin_port = htons(PORT); 
		servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

		// Bind to localhost
		int bindCode = bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
		if(bindCode < 0) { 
			perror("Bind failed!"); 
			exit(EXIT_FAILURE); 
		} 
	}

	std::string receive() {
		int n; 
		socklen_t len;
		
		len = sizeof(cliaddr);
		
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
					MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
					&len); 
		buffer[n] = '\0'; 

		return std::string(buffer);
	}

	/**
	 * Sends `buflen` amount of bytes from `buf` to socket.
	*/ 
	int send(const char* buf, size_t buflen) {
		if(buflen > MAXLINE) 
			return -1;

		socklen_t len;
		len = sizeof(cliaddr);
		
		return sendto(sockfd, buf, buflen,  
			MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
				len); 
	}
};


int main() { 
	IPCSocket socket;

	std::string msg = socket.receive();

	std::cout<<"Message: " << msg << std::endl;

	const char* buf = "Hello";
	int code = socket.send(buf, strlen(buf));

	std::cout<<"Code: " << code << std::endl;
        
        
    return 0; 
}
