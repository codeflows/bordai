#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
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
	void drawHistogram(ci::Rectf drawArea);

protected:
	class Histogram {
	public:
		Histogram() {}
		
		ci::ImageSourceRef		mHistogramImage;
		std::vector<ci::Rectf>	mScans;
	};
	
private:
	cv::CascadeClassifier	mCascade;
	Histogram				mHistogram;
};