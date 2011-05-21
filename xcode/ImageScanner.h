#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/Surface.h"
#include "cinder/CinderMath.h"
#include "CinderOpenCv.h"
#include <vector>

class ImageScanner {
public:
	ImageScanner();
	ImageScanner(std::string cascadeFilePath);
	
	void scan(ci::Surface surface);
	void draw(ci::Rectf drawArea);
	
private:
	cv::CascadeClassifier	mCascade;
	std::vector<ci::Rectf>	mScans;
};