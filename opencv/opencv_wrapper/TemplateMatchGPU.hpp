#ifndef TEMPLATEMATCHGPU_HPP
#define TEMPLATEMATCHGPU_HPP

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

#include "TemplateMatch.hpp"
#include "TemplateGPU.hpp"

using namespace cv;
using namespace cv::cuda;

/**
 * Uses CUDA-based Template Matching via OpenCV.
*/
class TemplateMatchGPU : public TemplateMatch {

private:
	std::vector<TemplateGPU*> templates;
	cuda::GpuMat* background;
	//cuda::TemplateMatching templMatch;


public:
	TemplateMatchGPU() : TemplateMatch() {
		templates.reserve(10);
		background = nullptr;
	}

	~TemplateMatchGPU() {
		delete background;
	}

	void addTemplate(int id, std::string file) override {
		templates.push_back(new TemplateGPU(id, file));
	}

	void setBackground(std::string file) override {
		if(background != nullptr)
			delete background;

		Mat back = imread(file, IMREAD_GRAYSCALE);
		background = new cuda::GpuMat();
		background->upload(back);
	}


	std::vector<Match> match() override {
		std::vector<Match> matches;
		for(TemplateGPU* mat : templates) {
			matches.push_back(match(mat));
		}

		return matches;
	}

private:
	Match match(TemplateGPU* template_) {
		cuda::GpuMat* mat = template_->getMat();

		cuda::GpuMat result;
		int result_cols =  background->cols - mat->cols + 1;
		int result_rows = background->rows - mat->rows + 1;

		// CV_32F is also an option; Only 2 methods are implemented however
		result.create( result_cols, result_rows, CV_8U );

		Ptr<cuda::TemplateMatching> t = cuda::createTemplateMatching(mat->type(), TM_CCOEFF_NORMED);

		t->match(*background, *mat, result);

		double minVal, maxVal;
		Point minLoc, maxLoc;
		cv::cuda::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

		Point center(maxLoc.x + mat->cols / 2, maxLoc.y + mat->rows / 2);

		return Match(template_->getID(), center.x, center.y, Match::ORIGIN::CENTER, maxVal);
	}
};


#endif
