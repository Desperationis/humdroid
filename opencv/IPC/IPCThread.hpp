#ifndef IPCTHREAD_HPP
#define IPCTHREAD_HPP

#include <chrono>
#include <thread>
#include <iostream>
#include <functional>
#include <atomic>
#include <memory>
#include <sstream>

#include "IPC/IPCSocket.h"
#include "IPC/IPCMsgQueue.hpp"
#include "IPC/IPCInputMsg.hpp"
#include "IPC/IPCOutputMsg.hpp"
#include "util/MutexQueue.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class IPCThread {
private:
	IPCSocket* outSocket;
	std::thread* outThread;

	IPCSocket* inSocket;
	std::thread* inThread;

	std::shared_ptr<IPCMsgQueue> queue = nullptr;
	std::atomic<bool> running;
	std::atomic<bool> interruptFlag;


public:
	IPCThread() {
		outThread = inThread = nullptr;
		outSocket = inSocket = nullptr;
		queue = nullptr;
	}

	void AttachQueue(std::shared_ptr<IPCMsgQueue> queue) {
		this->queue = queue;
	}

	std::shared_ptr<IPCMsgQueue> GetQueue() {
		return queue;
	}

	bool IsRunning() {
		if (inThread == nullptr || outThread == nullptr)
			return false;

		return running.load();
	}

	void Start() {
		inSocket = new IPCSocket(6069);
		outSocket = new IPCSocket(6070);


		inThread = new std::thread(std::bind(&IPCThread::InputLoop, this));
		outThread = new std::thread(std::bind(&IPCThread::OutputLoop, this));
	}

	void OutputLoop() {
		outSocket->ListenForClient();
		auto duration = std::chrono::milliseconds(500);
		while(true) {
			if(queue != nullptr) {
				std::cout << "Checking output queue->.." << std::endl;
				for(int i = 0; i < queue->outputQueue.Size(); i++) {
					IPCOutputMsg msg = queue->outputQueue.Pop();
					std::string msgStr = msg.Serialize();
					std::cout<<msgStr<<std::endl;
					std::cout<<msgStr.length()<<std::endl;
					int n = outSocket->Send(msgStr.c_str(), msgStr.length());
					std::cout<<"Send out: " << n << std::endl;
				}
			}

			std::this_thread::sleep_for(duration);
		}

	}


	void InputLoop() {
		inSocket->ListenForClient();
		while(true) {
			std::string msg = inSocket->Receive();
			std::cout<<"Input message received: " << msg <<std::endl;
			std::stringstream msgStream(msg);
			std::string parsed;

			// Parse message through delimeter, $, if there's multiple
			while(std::getline(msgStream, parsed, '$')) {
				auto jsonMsg = json::parse(parsed);
				queue->inputQueue.Push(IPCInputMsg(jsonMsg));
			}
		}
	}
};


#endif
