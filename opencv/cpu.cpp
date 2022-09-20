#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <chrono>

#include "opencv_wrapper/TemplateMatchCPU.hpp"
#include "IPC/IPCSocket.h"
#include "IPC/IPCThread.hpp"
#include "IPC/IPCMsgQueue.hpp"
#include "IPC/IPCOutputMsg.hpp"
#include "IPC/IPCInputMsg.hpp"
#include "nlohmann/json.hpp"
#include <signal.h>

using namespace cv;
using namespace std;
using json = nlohmann::json;


void loop() {
	IPCSocket socket(8000);
	socket.ListenForClient();
	while(true) {
		if(socket.Connected())
			std::cout << "Connected" << std::endl;
		else {
			std::cout << "Not connected" << std::endl;
			socket.Close();
			socket.ListenForClient();
		}
		socket.Receive();
	}
}

void loop2() {
	signal(SIGPIPE, SIG_IGN);
	IPCSocket socket(8000);
	socket.ListenForClient();
	while(true) {
		if(socket.Connected(true))
			std::cout << "Connected" << std::endl;
		else {
			std::cout << "Not connected" << std::endl;
			socket.Close();
			socket.ListenForClient();
		}
		socket.Send("amongus", strlen("amongus"));
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

int main( int argc, char** argv )
{
	signal(SIGPIPE, SIG_IGN);
	//std::thread thread(loop2);

	//thread.join();





	std::cout<<"OpenCV Version: " << CV_VERSION << std::endl;

	auto msgQueue = std::make_shared<IPCMsgQueue>();

	IPCThread ipcThread;
	ipcThread.Start();
	ipcThread.AttachQueue(msgQueue);

	TemplateMatchCPU t;

	while (true ) {
		// Add templates 
		while(msgQueue->templateQueue.Size() > 0) {
			std::vector<std::string> templates = msgQueue->templateQueue.Pop().GetTemplates();
			for(int i = 0; i < templates.size(); i ++) {
				t.addTemplate(i, templates[i]);
			}
		}
	
		// Match templates, if requested
		while(msgQueue->compareQueue.Size() > 0) {
			CompareSingleMsg msg = msgQueue->compareQueue.Pop();
			t.setBackground(msg.GetPhoto());

			MatchesMsg matchMsg;

			std::vector<Match> matches = t.match();
			for(Match match : matches) {
				if(match.getConfidence() > msg.GetMinConfidence()) {
					matchMsg.AddMatch(match);
				}
			}

			msgQueue->outputQueue.Push(matchMsg);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}




	/*
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout<<"Duration (ms): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
	*/
	return 0;
}
