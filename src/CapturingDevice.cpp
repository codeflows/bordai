#include "cinder/app/AppBasic.h"
#include "cinder/Text.h"
#include "cinder/Surface.h"
#include "CapturingDevice.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void CapturingDevice::startCapturing(const int lensWidth, const int lensHeight) {
	mCapture = Capture( lensWidth, lensHeight );
	mCapture.start();
	mCameraTexture = gl::Texture();
}

void CapturingDevice::bufferCaptured(ImageTracker &tracker) {
	if( mCapture.checkNewFrame() ) {
		Surface8u capturedSurface = mCapture.getSurface();
		mCameraTexture = gl::Texture( capturedSurface );
		tracker.scanTrackables(capturedSurface);
	}
}

void CapturingDevice::draw(Rectf drawArea) {
	gl::draw(mCameraTexture, drawArea);
	mCameraTexture.disable();
}

void CapturingDevice::toggleOnOff() {
	mCapture.isCapturing() ? mCapture.stop() : mCapture.start();
}

bool CapturingDevice::hasSomething() {
	return mCameraTexture ? true : false;
}