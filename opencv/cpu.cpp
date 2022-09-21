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

using namespace cv;
using namespace std;
using json = nlohmann::json;


int main( int argc, char** argv )
{
	std::cout<<"OpenCV Version: " << CV_VERSION << std::endl;

	auto msgQueue = std::make_shared<IPCMsgQueue>();

	IPCThread ipcThread;
	ipcThread.Start();
	ipcThread.AttachQueue(msgQueue);

	TemplateMatchCPU t;

	while (true ) {
		// Parse through input messages
		while(msgQueue->inputQueue.Size() > 0) {
			json data = msgQueue->inputQueue.Pop().GetJSON();
			
			if(LoadTemplateMsg::IsMsg(data)) {
				LoadTemplateMsg msg(data);
				t.addTemplate(msg.GetID(), msg.GetPath(), msg.GetGroup());
				std::cout << "LoadTemplate received." << std::endl;
				std::cout<<"Template: " <<std::endl;
				std::cout<<"Path: " << msg.GetPath() << std::endl;
				std::cout<<"ID: " << msg.GetID() << std::endl;
				std::cout<<"Group: " << msg.GetGroup() << std::endl;
			}

			else if(CompareIDMsg::IsMsg(data)) {
				CompareIDMsg msg(data);
				t.setBackground(msg.GetPhoto());

				MatchesMsg matchMsg;
				std::vector<Match> matches = t.match(msg.GetID(), -1);
				for(Match match : matches) {
					if(match.getConfidence() > msg.GetMinConfidence()) {
						matchMsg.AddMatch(match);
					}
				}

				msgQueue->outputQueue.Push(matchMsg);
			}

			else if(CompareGroupMsg::IsMsg(data)) {
				CompareGroupMsg msg(data);
				t.setBackground(msg.GetPhoto());

				MatchesMsg matchMsg;
				std::vector<Match> matches = t.match(-1, msg.GetGroup());
				for(Match match : matches) {
					if(match.getConfidence() > msg.GetMinConfidence()) {
						matchMsg.AddMatch(match);
					}
				}

				msgQueue->outputQueue.Push(matchMsg);
			}
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
