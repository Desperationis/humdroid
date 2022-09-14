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

#include "TemplateMatch.hpp"
#include "TemplateCPU.hpp"

using namespace cv;

class TemplateMatchCPU : public TemplateMatch {
private:
	std::vector<TemplateCPU*> templates;
	Mat* background;

public:
	TemplateMatchCPU() : TemplateMatch() {
		templates.reserve(10);
		background = nullptr;
	}

	~TemplateMatchCPU() {
		delete background;
	}

	void addTemplate(int id, std::string file) override {
		templates.push_back(new TemplateCPU(id, file));
	}

	void setBackground(std::string file) override {
		if(background != nullptr)
			delete background;

		background = new Mat(imread(file, IMREAD_GRAYSCALE));
	}


	std::vector<Match> match() override {
		std::vector<Match> matches;
		for(TemplateCPU* mat : templates) {
			matches.push_back(match(mat));
		}

		return matches;
	}

private:
	Match match(TemplateCPU* template_) {
		Mat* mat = template_->getMat();

		Mat result;
		int result_cols =  background->cols - mat->cols + 1;
		int result_rows = background->rows - mat->rows + 1;

		result.create( result_cols, result_rows, CV_32FC1 );

		matchTemplate(*background, *mat, result, TM_CCOEFF_NORMED);

		double minVal, maxVal;
		Point minLoc, maxLoc;
		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

		Point center(maxLoc.x + mat->cols / 2, maxLoc.y + mat->rows / 2);

		return Match(template_->getID(), center.x, center.y, Match::ORIGIN::CENTER, maxVal);
	}
};
