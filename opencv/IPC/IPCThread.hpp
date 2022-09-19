#ifndef IPCTHREAD_HPP
#define IPCTHREAD_HPP

#include <thread>
#include <iostream>
#include <functional>
#include <atomic>
#include <memory>

#include "IPC/IPCSocket.h"
#include "IPC/IPCMsgQueue.hpp"
#include "IPC/Messages.hpp"
#include "util/MutexQueue.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class IPCThread {
private:
	IPCSocket* socket;
	std::shared_ptr<IPCMsgQueue> queue = nullptr;
	std::thread* thread;
	std::atomic<bool> running;
	std::atomic<bool> interruptFlag;


public:
	IPCThread() {
		thread = nullptr;
		queue = nullptr;
		socket = nullptr;
	}

	void AttachQueue(std::shared_ptr<IPCMsgQueue> queue) {
		this->queue = queue;
	}

	std::shared_ptr<IPCMsgQueue> GetQueue() {
		return queue;
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
			if(queue != nullptr) {
				std::cout << "Checking output queue->.." << std::endl;
				for(int i = 0; i < queue->outputQueue.Size(); i++) {
					IPCOutputMsg msg = queue->outputQueue.Pop();
					std::string msgStr = msg.ToJSON().dump();
					std::cout<<msgStr<<std::endl;
					socket->send(msgStr.c_str(), msgStr.length());
				}
			}

			std::cout << "Waiting for a message..." << std::endl;
			std::string msg = socket->receive();

			auto jsonMsg = json::parse(msg);
			if(LoadTemplatesMsg::IsMsg(jsonMsg)) {
				std::cout << "LoadTemplates received." << std::endl;
				LoadTemplatesMsg templatesMsg(jsonMsg);
				if(queue != nullptr)
					queue->templateQueue.Push(templatesMsg);

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
				
				if(queue != nullptr)
					queue->compareQueue.Push(compareSingleMsg);

				std::string background = compareSingleMsg.GetPhoto();
			}


		}
	}
};


#endif
