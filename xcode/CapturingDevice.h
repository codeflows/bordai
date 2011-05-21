#pragma once

#include "cinder/Capture.h"
#include "cinder/gl/Texture.h"
#include "CapturingDevice.h"
#include "ImageScanner.h"
#include <vector>

class CapturingDevice {
public:	
	void setLensSize(const int lensWidth, const int lensHeigth);
	void startCapturing();
	void bufferCaptured(ImageScanner &mImageScanner);
	void draw();
	void toggleOnOff();
	bool hasSomething();
			
private:
	int mLensWidth, mLensHeight;
	ci::Capture	mCapture;
	ci::gl::Texture mCameraTexture;	
};