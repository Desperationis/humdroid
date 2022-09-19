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

#include "IPC/IPCSocket.h"
#include "IPC/IPCThread.hpp"
#include "IPC/IPCMsgQueue.hpp"
#include "IPC/Messages.hpp"
#include "nlohmann/json.hpp"

using namespace cv;
using namespace std;
using json = nlohmann::json;


int main( int argc, char** argv )
{
	std::cout<<"OpenCV Version: " << CV_VERSION << std::endl;




	IPCThread ipcThread;
	auto msgQueue = std::make_shared<IPCMsgQueue>();
	ipcThread.Start();
	ipcThread.AttachQueue(msgQueue);


	while (true ) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		Match match(0, 0, 0, Match::ORIGIN::CENTER, 0.69);
		MatchesMsg msg;
		msg.AddMatch(match);

		msgQueue->outputQueue.Push(msg);
	}




	/*
	TemplateMatchCPU t;










	std::cout << "Waiting for images..." << std::endl;

	IPCSocket socket;
	std::string msg = socket.receive();

	std::cout<<"Message received: " << msg << std::endl;

	auto j3 = json::parse(msg);
	auto templates = j3["loadTemplates"]["templates"];

	std::cout<<"Templates: " <<std::endl;
	for(int i = 0; i < templates.size(); i ++) {
		std::cout << templates[i] << std::endl;
		t.addTemplate(i, templates[i]);
	}






















	t.setBackground(argv[2]);

	std::vector<Match> matches = t.match(); // Dummy call for CUDA init

	// float minSimilarityValue = std::stof(argv[3]);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	matches = t.match();

	for(int i = 0; i < matches.size(); i++) {
		Match match = matches[i];
		std::cout << "ID: " << match.getID() << "\tCOORD: " << match.getX() << " " << match.getY() << std::endl;
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout<<"Duration (ms): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
	*/
	return 0;
}
