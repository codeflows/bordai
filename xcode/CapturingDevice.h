#pragma once

#include "cinder/Capture.h"
#include "cinder/gl/Texture.h"
#include "CapturingDevice.h"
#include "ImageScanner.h"
#include <vector>

class CapturingDevice {
public:	
	void setLensSize(const int lensWidth, const int lensHeigth);
	void setImageScanner( ImageScanner scanner );
	void startCapturing();
	void bufferCaptured();
	void draw();
	void toggleOnOff();
	bool hasSomething();
			
private:
	int mLensWidth, mLensHeight;
	ImageScanner mImageScanner;
	std::vector<ci::Capture>		mCaptures;
	std::vector<ci::gl::Texture>	mDeviceNameTextures;
	std::vector<ci::gl::Texture>	mTextureBuffer;	
};