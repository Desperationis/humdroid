#ifndef IPCMSGQUEUE_HPP
#define IPCMSGQUEUE_HPP

#include <iostream>
#include "util/MutexQueue.hpp"
#include "IPC/Messages.hpp"

struct IPCMsgQueue {
	MutexQueue<IPCOutputMsg> outputQueue;
	MutexQueue<LoadTemplatesMsg> templateQueue;
	MutexQueue<CompareSingleMsg> compareQueue;
};


#endif
