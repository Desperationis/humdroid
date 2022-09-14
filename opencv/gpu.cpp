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
using namespace cv;
using namespace std;


cuda::GpuMat TplMatch(cuda::GpuMat &img, cuda::GpuMat& myTemplate) {
	cuda::GpuMat result;
	/// Create the result matrix
	  int result_cols =  img.cols - myTemplate.cols + 1;
	  int result_rows = img.rows - myTemplate.rows + 1;

	  //result.create( result_cols, result_rows, CV_32F );
	  result.create( result_cols, result_rows, CV_8U );

	Ptr<cv::cuda::TemplateMatching> t = cv::cuda::createTemplateMatching(myTemplate.type(), TM_CCOEFF_NORMED);

	t->match(img, myTemplate, result);

	//matchTemplate(img, myTemplate, result, TM_CCOEFF_NORMED);
	//normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
	
	return result;
}


int main( int argc, char** argv )
{
	std::cout<<"Count: " << cv::cuda::getCudaEnabledDeviceCount() << std::endl;
	std::cout<<"Version: " << CV_VERSION << std::endl;


	Mat myTemplate = imread(argv[1], IMREAD_GRAYSCALE);
	Mat myImage = imread(argv[2], IMREAD_GRAYSCALE);

	cv::cuda::GpuMat templateGPU;
	templateGPU.upload(myTemplate);

	cv::cuda::GpuMat imageGPU;
	imageGPU.upload(myImage);


	cuda::GpuMat match = TplMatch(imageGPU, templateGPU);
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	//
	myTemplate = imread(argv[1], IMREAD_GRAYSCALE);
	myImage = imread(argv[2], IMREAD_GRAYSCALE);

	cv::cuda::GpuMat t;
	t.upload(myTemplate);

	cv::cuda::GpuMat i;
	i.upload(myImage);
	match = TplMatch(i, t);
	//

	//Mat m_match;
	//match.download(m_match);

	double minVal, maxVal;
	Point minLoc, maxLoc, matchLoc;
	//minMaxLoc(m_match, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	cv::cuda::minMaxLoc(match, &minVal, &maxVal, &minLoc, &maxLoc);
	matchLoc = maxLoc;

	float minSimilarityValue = std::stof(argv[3]);

	if(maxVal < minSimilarityValue) {
		return 1;
	}

	Point center = Point(matchLoc.x + myTemplate.cols / 2, matchLoc.y + myTemplate.rows / 2);

	std::cout << center.x << " " << center.y << std::endl;

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout<<"Duration (ms): " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;

	return 0;
}
