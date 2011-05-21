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
	mImageScanner.setImageSize(mLensWidth, mLensHeight);
}

void CapturingDevice::setImageScanner( ImageScanner scanner ) {
	mImageScanner = scanner;
}

void CapturingDevice::startCapturing() {
	mCapture = Capture( mLensWidth, mLensHeight );
	mCapture.start();
	mCameraTexture = gl::Texture();
}

void CapturingDevice::bufferCaptured() {
	if( mCapture.checkNewFrame() ) {
		Surface8u capturedSurface = mCapture.getSurface();
		mCameraTexture = gl::Texture( capturedSurface );
		mImageScanner.scan(capturedSurface);
	}
}

void CapturingDevice::draw() {
	float width = getWindowWidth();
	float height = width / ( mLensWidth / (float)mLensHeight );
	float y = ( getWindowHeight() - height ) / 2.0f;
	
	gl::color( Color::white() );

	gl::setMatricesWindow( getWindowWidth(), getWindowHeight() );
	glPushMatrix();
	gl::draw(mCameraTexture , Rectf( 0, y, width, y + height ) );

	mCameraTexture.disable();
	mImageScanner.draw();
		
	glPopMatrix();
		
}

void CapturingDevice::toggleOnOff() {
	mCapture.isCapturing() ? mCapture.stop() : mCapture.start();
}

bool CapturingDevice::hasSomething() {
	return mCameraTexture ? true : false;
}