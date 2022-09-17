#include "IPCSocket.h"
#include "nlohmann/json.hpp"

int main() { 
	IPCSocket socket;

	std::string msg = socket.receive();

	std::cout<<"Message: " << msg << std::endl;

	const char* buf = "Hello";
	int code = socket.send(buf, strlen(buf));

	std::cout<<"Code: " << code << std::endl;
        
        
    return 0; 
}
