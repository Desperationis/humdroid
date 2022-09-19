#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaarithm.hpp>
#include <iostream>
#include <chrono>

#include "opencv_wrapper/TemplateMatchGPU.hpp"
#include "IPC/IPCSocket.h"
#include "IPC/Messages.hpp"
#include "nlohmann/json.hpp"

using namespace cv;
using namespace std;
using json = nlohmann::json;

int main( int argc, char** argv )
{
	std::cout<<"CUDA Device Count: " << cv::cuda::getCudaEnabledDeviceCount() << std::endl;
	std::cout<<"OpenCV Version: " << CV_VERSION << std::endl;

	TemplateMatchGPU t;




	std::cout << "Waiting for images..." << std::endl;
	IPCSocket socket;
	std::string msg = socket.receive();

	std::cout<<"Message received: " << msg << std::endl;

	auto j3 = json::parse(msg);
	std::cout<<LoadTemplatesMsg::IsMsg(j3)<<std::endl;
	LoadTemplatesMsg loadMsg(j3);

	std::cout<<"Templates: " <<std::endl;
	auto templates = loadMsg.GetTemplates();
	for(int i = 0; i < templates.size(); i ++) {
		std::cout << templates[i] << std::endl;
		t.addTemplate(i, templates[i]);
	}



	while(true) {
		std::cout << "Waiting for compare request..." << std::endl;
		std::string compareMsg = socket.receive();

		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		std::cout<<"Message received: " << compareMsg << std::endl;
		auto compareJSON = json::parse(compareMsg);
		CompareSingleMsg msg(compareJSON);
		std::string background = msg.GetPhoto();

		t.setBackground(background);
		std::vector<Match> matches = t.match();

		MatchesMsg matchesMsg;

		for(int i = 0; i < matches.size(); i++) {
			Match match = matches[i];
			std::cout << "ID: " << match.getID() << "\tCOORD: " << match.getX() << " " << match.getY() << std::endl;
			matchesMsg.AddMatch(match);
		}

		std::cout<<"Sending out message..."<<std::endl;
		std::string dump = matchesMsg.GetJSON().dump();
		socket.send(dump.c_str(), dump.length());



		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		std::cout<<"Duration (ms): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;

	}




	// float minSimilarityValue = std::stof(argv[3]);


	return 0;
}
