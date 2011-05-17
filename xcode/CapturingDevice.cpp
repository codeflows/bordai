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

void CapturingDevice::setImageScanner( ImageScanner scanner ) {
	mImageScanner = scanner;
	mImageScanner.setImageSize(mLensWidth, mLensHeight);
}

void CapturingDevice::startCapturing() {
	vector<Capture::DeviceRef> allDevices (Capture::getDevices());
	for (vector<Capture::DeviceRef>::const_iterator aDevice = allDevices.begin(); aDevice != allDevices.end(); ++aDevice) {
		Capture::DeviceRef device = *aDevice;
		console() << "Located device " << device->getName() << " with id " << device->getUniqueId() << std::endl;
		try {
			if (device->checkAvailable()) {
				mCaptures.push_back( Capture( mLensWidth, mLensHeight, device ) );
				mCaptures.back().start();
				mTextureBuffer.push_back( gl::Texture() );

				TextLayout layout;
				layout.setFont( Font( "Arial", 18 ) );
				layout.setColor( Color( 1, 1, 0 ) );
				layout.addLine( device->getName() );
				mDeviceNameTextures.push_back( gl::Texture( layout.render( true ) ) );				
			}
		}
		catch (CaptureExc &) {
			console() << "Cannot use device " << device->getName() << std::endl;
		}
	}
}

void CapturingDevice::bufferCaptured() {
	for( vector<Capture>::const_iterator aCapture = mCaptures.begin(); aCapture != mCaptures.end(); ++aCapture ) {
		if( aCapture->checkNewFrame() ) {
			Surface8u capturedSurface = aCapture->getSurface();
			mTextureBuffer[aCapture - mCaptures.begin()] = gl::Texture( capturedSurface );
			mImageScanner.scan(capturedSurface);
		}
	}	
}

void CapturingDevice::draw() {
	float width = getWindowWidth() / mCaptures.size();	
	float height = width / ( mLensWidth / (float)mLensHeight );
	float x = 0, y = ( getWindowHeight() - height ) / 2.0f;	
	
	for( vector<Capture>::iterator aCapture = mCaptures.begin(); aCapture != mCaptures.end(); ++aCapture ) {	
		int frameIndex = aCapture-mCaptures.begin();
		// draw the latest frame
		gl::color( Color::white() );
		if( mTextureBuffer[frameIndex] ) {
			gl::Texture cameraTexture = mTextureBuffer[frameIndex];
			gl::draw(cameraTexture , Rectf( x, y, x + width, y + height ) );
			cameraTexture.disable();
			mImageScanner.draw();
		}
		
		// draw the name
		gl::color( Color::black() );	
		gl::draw( mDeviceNameTextures[frameIndex], Vec2f( x + 10 + 1, y + 10 + 1 ) );
		gl::color( Color( 0.5, 0.75, 1 ) );
		gl::draw( mDeviceNameTextures[frameIndex], Vec2f( x + 10, y + 10 ) );
		
		x += width;
	}	
}

void CapturingDevice::toggleOnOff() {
	mCaptures.back().isCapturing() ? mCaptures.back().stop() : mCaptures.back().start();
}

bool CapturingDevice::hasSomething() {
	return !mCaptures.empty();
}