#pragma once

#include "cinder/Capture.h"
#include "cinder/gl/Texture.h"
#include "VideoCamera.h"
#include "HaarDetector.h"
#include "StoryCardDetector.h"
#include <vector>

class VideoCamera {
public:	
	void startCapturing(const int lensWidth, const int lensHeigth);
	void stopCapturing();
	void bufferCaptured(HaarDetector &haarDetector, StoryCardDetector &storyCardDetector);
	void draw();
	void togglePause();
	bool hasSomething();
			
private:
	ci::Capture	mCapture;
	ci::gl::Texture mCameraTexture;
};