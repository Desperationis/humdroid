#include "IPCSocket.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main() { 
	IPCSocket socket;

	std::string msg = socket.receive();

	std::cout<<"Message: " << msg << std::endl;
	auto j3 = json::parse(msg);
	std::cout<<"Parameter: " << j3["loadFolder"]["folder"]<<std::endl;

	std::string dump = j3.dump();
	int code = socket.send(dump.c_str(), dump.length());

	std::cout<<"Send code: " << code << std::endl;
        
    return 0; 
}
