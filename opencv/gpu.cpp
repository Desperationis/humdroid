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

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
	std::cout<<"Count: " << cv::cuda::getCudaEnabledDeviceCount() << std::endl;
	std::cout<<"Version: " << CV_VERSION << std::endl;

	TemplateMatchGPU t;

	t.addTemplate(23, argv[1]);
	t.addTemplate(0, argv[1]);
	t.addTemplate(5, argv[1]);
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

	return 0;
}
