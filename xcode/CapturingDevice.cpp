#include "cinder/app/AppBasic.h"
#include "cinder/Text.h"
#include "cinder/Surface.h"
#include "CapturingDevice.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void CapturingDevice::setLensSize(const int lensWidth, const int lensHeight) {
	mLensWidth = lensWidth;
	mLensHeight = lensHeight;
}

void CapturingDevice::startCapturing() {
	mCapture = Capture( mLensWidth, mLensHeight );
	mCapture.start();
	mCameraTexture = gl::Texture();
}

void CapturingDevice::bufferCaptured(ImageScanner &imageScanner) {
	if( mCapture.checkNewFrame() ) {
		Surface8u capturedSurface = mCapture.getSurface();
		mCameraTexture = gl::Texture( capturedSurface );
		imageScanner.scan(capturedSurface);
	}
}

void CapturingDevice::draw() {
	float width = getWindowWidth();
	float height = width / ( mLensWidth / (float)mLensHeight );
	float y = ( getWindowHeight() - height ) / 2.0f;
	
	gl::draw(mCameraTexture , Rectf( 0, y, width, y + height ) );

	mCameraTexture.disable();
		
}

void CapturingDevice::toggleOnOff() {
	mCapture.isCapturing() ? mCapture.stop() : mCapture.start();
}

bool CapturingDevice::hasSomething() {
	return mCameraTexture ? true : false;
}