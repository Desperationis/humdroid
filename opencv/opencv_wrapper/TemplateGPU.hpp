#ifndef TEMPLATEGPU_HPP
#define TEMPLATEGPU_HPP

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
#include <vector>
#include <string>

using namespace cv;
using namespace cv::cuda;

class TemplateGPU {
private:
	int id;
	cuda::GpuMat* mat;
public:
	TemplateGPU(int id, std::string filename) {
		this->id = id;

		Mat temp = imread(filename, IMREAD_GRAYSCALE);
		mat = new cv::cuda::GpuMat();
		mat->upload(temp);
	}

	int getID() {
		return id;
	}

	cuda::GpuMat* getMat() {
		return mat;
	}
};

#endif
