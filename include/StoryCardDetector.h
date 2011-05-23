#pragma once

#include "cinder/Rect.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
#include "CinderOpenCv.h"
#include <vector>

class StoryCardDetector {
public:
	StoryCardDetector();
	
	void scanTrackables(ci::Surface surface);
	void drawTrackings(ci::Rectf drawArea);
	void drawHistogram(ci::Rectf drawArea);
	
protected:
	ci::gl::Texture			mHistogramTexture;
	std::vector<ci::Rectf>  mStoryCards;	
};