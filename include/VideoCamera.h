#pragma once

#include "cinder/Capture.h"
#include "cinder/gl/Texture.h"
#include "VideoCamera.h"
#include "ImageTracker.h"
#include <vector>

class VideoCamera {
public:	
	void startCapturing(const int lensWidth, const int lensHeigth);
	void stopCapturing();
	void bufferCaptured(ImageTracker &tracker);
	void draw(ci::Rectf drawArea);
	void togglePause();
	bool hasSomething();
			
private:
	ci::Capture	mCapture;
	ci::gl::Texture mCameraTexture;	
};