#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/Surface.h"
#include "cinder/CinderMath.h"
#include "CinderOpenCv.h"
#include <vector>

class HaarDetector {
public:
	HaarDetector();
	HaarDetector(std::string cascadeFilePath);
	
	void scanTrackables(ci::Surface surface);
	void drawTrackings(ci::Rectf drawArea);
	void drawHistogram(ci::Rectf drawArea);

	cv::CascadeClassifier	mCascade;
	ci::ImageSourceRef		mHistogramImage;
	std::vector<ci::Rectf>	mScans;
};