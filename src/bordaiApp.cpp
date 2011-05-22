#include "cinder/app/AppBasic.h"
#include "cinder/params/Params.h"

#include "CapturingDevice.h"
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
	CapturingDevice mCamera;
	ImageTracker mTracker;
	params::InterfaceGl mParams;
	Vec2i mWindowSize;
	float mFrameRate;
};

void bordaiApp::prepareSettings(Settings *settings) {
	mFrameRate = 25.0f;
	mWindowSize = Vec2i(SCREEN_START_WIDTH, SCREEN_START_HEIGHT);
	
	settings -> setFrameRate(mFrameRate);
	settings -> setWindowSize(mWindowSize.x, mWindowSize.y);
	settings -> setResizable(true);
}

void bordaiApp::setup() {
	mParams = params::InterfaceGl("bordai", Vec2i(200, 100));
	mParams.addParam("Screen width", &mWindowSize.x, "", true);
	mParams.addParam("Screen height", &mWindowSize.y, "", true);
	mParams.addParam("Framerate", &mFrameRate, "min=5.0 max=70.0 step=5.0 keyIncr=+ keyDecr=-");	
	
	mTracker = ImageTracker( getResourcePath( "haarcascade_frontalface_alt2.xml" ) );
	mCamera.startCapturing(CAMERA_WIDTH, CAMERA_HEIGTH);
}

void bordaiApp::keyDown( KeyEvent event ) {
	if(event.getChar() == 'f') {
		setFullScreen( !isFullScreen() );
	}
	if(event.getChar() == ' ') {
		mCamera.toggleOnOff();
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
		
		glPopMatrix();
		
	}
	
	params::InterfaceGl::draw();
}

CINDER_APP_BASIC( bordaiApp, RendererGl )
