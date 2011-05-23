#define pragma once

#include "cinder/Rect.h"
#include "CinderOpenCv.h"
#include <vector>

class StoryCardDetector {
public:
	StoryCardDetector();
	
	void findSquares( const cv::Mat& image, std::vector<ci::Rectf> &squares );
};