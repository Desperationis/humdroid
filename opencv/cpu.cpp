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
using namespace cv;
using namespace std;


Mat TplMatch(Mat &img, Mat& myTemplate) {
	Mat result;
	/// Create the result matrix
	  int result_cols =  img.cols - myTemplate.cols + 1;
	  int result_rows = img.rows - myTemplate.rows + 1;

	  result.create( result_cols, result_rows, CV_32FC1 );

	matchTemplate(img, myTemplate, result, TM_CCOEFF_NORMED);
	//normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
	
	return result;
}


int main( int argc, char** argv )
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	Mat myTemplate = imread(argv[1], IMREAD_GRAYSCALE);
	Mat myImage = imread(argv[2], IMREAD_GRAYSCALE);


	Mat match = TplMatch(myImage, myTemplate);


	double minVal, maxVal;
	Point minLoc, maxLoc, matchLoc;
	minMaxLoc(match, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	matchLoc = maxLoc;

	float minSimilarityValue = std::stof(argv[3]);

	if(maxVal < minSimilarityValue) {
		return 1;
	}

	Point center = Point(matchLoc.x + myTemplate.cols / 2, matchLoc.y + myTemplate.rows / 2);

	std::cout << center.x << " " << center.y << std::endl;
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout<<"Duration (ms): " << std::chrono::duration_cast<std::chrono::milliseconds>(end -         begin).count() << std::endl;



	return 0;
}
