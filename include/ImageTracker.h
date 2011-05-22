#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/Surface.h"
#include "cinder/CinderMath.h"
#include "CinderOpenCv.h"
#include "StoryCardDetector.h"
#include <vector>

class ImageTracker {
public:
	ImageTracker();
	ImageTracker(std::string cascadeFilePath);
	
	void scanTrackables(ci::Surface surface);
	void drawTrackings(ci::Rectf drawArea);
	void drawHistogram(ci::Rectf drawArea);

protected:
	class Histogram {
	public:
		Histogram() {}
		
		ci::ImageSourceRef		mHistogramImage;
		std::vector<ci::Rectf>	mScans;
		std::vector<std::vector<cv::Point> > mStoryCards;
	};
	
private:
	cv::CascadeClassifier	mCascade;
	Histogram				mHistogram;
	StoryCardDetector		mCardDetector;
};