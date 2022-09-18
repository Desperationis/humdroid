#include "IPCSocket.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main() { 
	IPCSocket socket;

	std::string msg = socket.receive();

	std::cout<<"Message: " << msg << std::endl;
	auto j3 = json::parse(msg);

	auto templates = j3["loadTemplates"]["templates"];

	std::cout<<"Templates: " <<std::endl;
	for(int i = 0; i < templates.size(); i ++) {
		std::cout << templates[i] << std::endl;
	}

	std::string dump = j3.dump();
	int code = socket.send(dump.c_str(), dump.length());

	std::cout<<"Send code: " << code << std::endl;
        
    return 0; 
}
