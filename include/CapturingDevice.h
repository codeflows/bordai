#pragma once

#include "cinder/Capture.h"
#include "cinder/gl/Texture.h"
#include "CapturingDevice.h"
#include "ImageScanner.h"
#include <vector>

class CapturingDevice {
public:	
	void startCapturing(const int lensWidth, const int lensHeigth);
	void bufferCaptured(ImageScanner &mImageScanner);
	void draw(ci::Rectf drawArea);
	void toggleOnOff();
	bool hasSomething();
			
private:
	ci::Capture	mCapture;
	ci::gl::Texture mCameraTexture;	
};