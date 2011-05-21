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

protected:
	class Histogram {
	public:
		Histogram(ci::ImageSourceRef histogramImage, ci::Rectf scanLocation) {
			mHistogramImage = histogramImage;
			mScanLocation = scanLocation;
		}
		ci::ImageSourceRef mHistogramImage;
		ci::Rectf mScanLocation;
	};
	
private:
	cv::CascadeClassifier	mCascade;
	std::vector<Histogram>	mHistograms;
};