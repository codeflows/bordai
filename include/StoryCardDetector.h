#define pragma once

#include "CinderOpenCv.h"
#include <vector>

class StoryCardDetector {
public:
	StoryCardDetector();
	
	void findSquares( const cv::Mat& image, std::vector<std::vector<cv::Point> >& squares );
};