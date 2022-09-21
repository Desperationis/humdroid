#ifndef IPCMSGQUEUE_HPP
#define IPCMSGQUEUE_HPP

#include <iostream>
#include "util/MutexQueue.hpp"
#include "IPC/IPCInputMsg.hpp"
#include "IPC/IPCOutputMsg.hpp"

struct IPCMsgQueue {
	MutexQueue<IPCOutputMsg> outputQueue;
	MutexQueue<LoadTemplateMsg> templateQueue;
	MutexQueue<CompareSingleMsg> compareQueue;
};


#endif
