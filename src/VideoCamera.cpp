#include "cinder/app/AppBasic.h"
#include "cinder/Text.h"
#include "cinder/Surface.h"
#include "VideoCamera.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void VideoCamera::startCapturing(const int lensWidth, const int lensHeight) {
	mCapture = Capture( lensWidth, lensHeight );
	mCapture.start();
	mCameraTexture = gl::Texture();
}

void VideoCamera::stopCapturing() {
	mCapture.stop();
	mCameraTexture = gl::Texture();
}

void VideoCamera::bufferCaptured(ImageTracker &tracker) {
	if( mCapture.checkNewFrame() ) {
		Surface8u capturedSurface = mCapture.getSurface();
		mCameraTexture = gl::Texture( capturedSurface );
		tracker.scanTrackables(capturedSurface);
	}
}

void VideoCamera::draw(Rectf drawArea) {
	gl::draw(mCameraTexture, drawArea);
	mCameraTexture.disable();
}

void VideoCamera::togglePause() {
	mCapture.isCapturing() ? mCapture.stop() : mCapture.start();
}

bool VideoCamera::hasSomething() {
	return mCameraTexture ? true : false;
}