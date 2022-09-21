#ifndef TEMPLATECPU_HPP
#define TEMPLATECPU_HPP

#include <opencv2/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <chrono>


using namespace cv;

class TemplateCPU {
private:
	int id;
	int group;
	Mat* mat;
public:
	TemplateCPU(int id, std::string filename, int group) {
		this->id = id;
		this->group = group;

		mat = new Mat(imread(filename, IMREAD_GRAYSCALE));
	}

	int getID() {
		return id;
	}

	Mat* getMat() {
		return mat;
	}

	int getGroup() {
		return group;
	}
};

#endif
