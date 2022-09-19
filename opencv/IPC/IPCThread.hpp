#ifndef IPCTHREAD_HPP
#define IPCTHREAD_HPP

#include <thread>
#include <iostream>
#include <functional>
#include <atomic>


#include "IPC/IPCSocket.h"
#include "IPC/Messages.hpp"
#include "util/MutexQueue.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class IPCThread {
private:
	IPCSocket* socket;
	MutexQueue<IPCOutputMsg> outputQueue;
	std::thread* thread;
	std::atomic<bool> running;
	std::atomic<bool> interruptFlag;


public:
	IPCThread() {
		thread = nullptr;
	}

	bool IsRunning() {
		if (thread == nullptr)
			return false;

		return running.load();
	}

	void Start() {
		socket = new IPCSocket();
		thread = new std::thread(std::bind(&IPCThread::Loop, this));
	}


	void Loop() {
		while(true) {
			std::cout << "Checking output queue..." << std::endl;
			for(int i = 0; i < outputQueue.Size(); i++) {
				IPCOutputMsg msg = outputQueue.Pop();
				std::string msgStr = msg.ToJSON().dump();
				std::cout<<msgStr<<std::endl;
				socket->send(msgStr.c_str(), msgStr.length());
			}

			std::cout << "Waiting for a message..." << std::endl;
			std::string msg = socket->receive();

			auto jsonMsg = json::parse(msg);
			if(LoadTemplatesMsg::IsMsg(jsonMsg)) {
				std::cout << "LoadTemplates received." << std::endl;
				LoadTemplatesMsg templatesMsg(jsonMsg);

				std::cout<<"Templates: " <<std::endl;
				auto templates = templatesMsg.GetTemplates();
				for(int i = 0; i < templates.size(); i ++) {
					std::cout << templates[i] << std::endl;
					// t.addTemplate(i, templates[i]);
				}
			}

			else if (CompareSingleMsg::IsMsg(jsonMsg)) {
				std::cout << "CompareSingle received." << std::endl;

				CompareSingleMsg compareSingleMsg(jsonMsg);

				std::string background = compareSingleMsg.GetPhoto();
			}


		}
	}


	void PushOutput(IPCOutputMsg msg) {
		outputQueue.Push(msg);	
	}
};


#endif
