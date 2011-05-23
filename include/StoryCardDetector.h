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
	std::vector<ci::gl::Texture>	mHistogramTextures;
	
protected:
	std::vector<ci::Rectf>			mStoryCards;	
};