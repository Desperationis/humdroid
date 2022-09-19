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

	TemplateMatchCPU t;

	while (true ) {
		while(msgQueue->templateQueue.Size() > 0) {
			std::vector<std::string> templates = msgQueue->templateQueue.Pop().GetTemplates();
			for(int i = 0; i < templates.size(); i ++) {
				t.addTemplate(i, templates[i]);
			}
		}
		
		while(msgQueue->compareQueue.Size() > 0) {
			CompareSingleMsg msg = msgQueue->compareQueue.Pop();
			t.setBackground(msg.GetPhoto());

			MatchesMsg matchMsg;

			std::vector<Match> matches = t.match();
			for(int i = 0; i < matches.size(); i++) {
				Match match = matches[i];
				std::cout << "ID: " << match.getID() << "\tCOORD: " << match.getX() << " " << match.getY() << std::endl;


				matchMsg.AddMatch(match);

			}

			msgQueue->outputQueue.Push(matchMsg);
		}


		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}




	/*









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
