#define pragma once

#include "cinder/Rect.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
#include "CinderOpenCv.h"
#include <vector>

#ifndef STORYCARD_INCLUDED
#define STORYCARD_INCLUDED

class StoryCardDetector {
public:
	StoryCardDetector();
	
	void scanTrackables(ci::Surface surface);
	void drawTrackings(ci::Rectf drawArea);
	void drawHistogram(ci::Rectf drawArea);
	
protected:
	ci::ImageSourceRef		mHistogramImage;
	std::vector<ci::Rectf>  mStoryCards;	
};

#endif