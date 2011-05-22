#include "cinder/app/AppBasic.h"
#include "cinder/params/Params.h"

#include "VideoCamera.h"
#include "ImageTracker.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGTH 480
#define SCREEN_START_WIDTH 600
#define SCREEN_START_HEIGHT 750

using namespace ci;
using namespace ci::app;
using namespace std;

class bordaiApp : public AppBasic {
  public:
	void prepareSettings(Settings *settings);
	void setup();
	void keyDown( KeyEvent event );
	void update();
	void draw();
	
  private:
	VideoCamera mCamera;
	ImageTracker mTracker;
	params::InterfaceGl mParams;
	Vec2i mWindowSize, mCameraLensSize;
	float mFrameRate;
};

void bordaiApp::prepareSettings(Settings *settings) {
	mFrameRate = 25.0f;
	mWindowSize = Vec2i(SCREEN_START_WIDTH, SCREEN_START_HEIGHT);
	mCameraLensSize = Vec2i(CAMERA_WIDTH, CAMERA_HEIGTH);
	
	settings -> setFrameRate(mFrameRate);
	settings -> setWindowSize(mWindowSize.x, mWindowSize.y);
	settings -> setResizable(true);
}

void bordaiApp::setup() {
	mParams = params::InterfaceGl("bordai", Vec2i(300, 175));
	mParams.addParam("Screen width", &mWindowSize.x, "", true);
	mParams.addParam("Screen height", &mWindowSize.y, "", true);
	mParams.addParam("Camera width", &mCameraLensSize.x, "min=128 max=1024 step=64 keyIncr=W keyDecr=w");
	mParams.addParam("Camera height", &mCameraLensSize.y, "min=128 max=1024 step=64 keyIncr=H keyDecr=h");
	mParams.addSeparator();
	mParams.addText("Press space to apply camera resolution");
	mParams.addSeparator();
	mParams.addParam("Framerate", &mFrameRate, "min=5.0 max=70.0 step=5.0 keyIncr=+ keyDecr=-");
	
	mTracker = ImageTracker( getResourcePath( "haarcascade_frontalface_alt2.xml" ) );
	mCamera.startCapturing(mCameraLensSize.x, mCameraLensSize.y);
}

void bordaiApp::keyDown( KeyEvent event ) {
	char c = event.getChar();
	if(c == 'f') {
		setFullScreen( !isFullScreen() );
	}
	if(c == 'p' || c == 'P') {
		mCamera.togglePause();
	}
	if(c == ' ') {
		mCamera.stopCapturing();
		mCamera.startCapturing(mCameraLensSize.x, mCameraLensSize.y);
	}	
}

void bordaiApp::update() {
	setFrameRate(mFrameRate);
	mWindowSize = getWindowSize();
	mCamera.bufferCaptured(mTracker);
}

void bordaiApp::draw() {
	gl::enableAlphaBlending();
	gl::clear( Color::black() );
	
	if( mCamera.hasSomething() ) {
		
		gl::color( Color::white() );
		gl::setMatricesWindow( mWindowSize.x, mWindowSize.y );
		glPushMatrix();
		
		Rectf cameraArea( 0, 0, mWindowSize.x, mWindowSize.y / 2.0f );
		Rectf histogramArea( 0, mWindowSize.y / 2.0f, mWindowSize.x, mWindowSize.y );
		mCamera.draw(cameraArea);
		mTracker.drawHistogram(histogramArea);
		
		gl::color( ColorA( 1, 1, 0, 0.45f ) );
		mTracker.drawTrackings(cameraArea);
		mTracker.drawTrackings(histogramArea);
		
		glPopMatrix();
		
	}
	
	params::InterfaceGl::draw();
}

CINDER_APP_BASIC( bordaiApp, RendererGl )
