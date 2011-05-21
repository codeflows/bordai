#include "cinder/app/AppBasic.h"
#include "cinder/params/Params.h"

#include "CapturingDevice.h"

using namespace ci;
using namespace ci::app;
using namespace std;

static const int DEFAULT_WIDTH = 640;
static const int DEFAULT_HEIGHT = 480;

class bordaiApp : public AppBasic {
  public:
	void prepareSettings(Settings *settings);
	void setup();
	void keyDown( KeyEvent event );
	void update();
	void draw();
	
  private:
	CapturingDevice mCamera;
	ImageScanner mImageScanner;
	params::InterfaceGl mParams;
	Vec2i mWindowSize;
	float mFrameRate;
};

void bordaiApp::prepareSettings(Settings *settings) {
	mFrameRate = 25.0f;
	mWindowSize = Vec2i(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	
	settings -> setFrameRate(mFrameRate);
	settings -> setWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	settings -> setResizable(true);
}

void bordaiApp::setup() {
	mParams = params::InterfaceGl("bordai", Vec2i(200, 100));
	mParams.addParam("Screen width", &mWindowSize.x, "", true);
	mParams.addParam("Screen height", &mWindowSize.y, "", true);
	mParams.addParam("Framerate", &mFrameRate, "min=5.0 max=70.0 step=5.0 keyIncr=+ keyDecr=-");	
	
	mImageScanner = ImageScanner( getResourcePath( "haarcascade_frontalface_alt2.xml" ) );
	mCamera.startCapturing(DEFAULT_WIDTH, DEFAULT_HEIGHT);
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
	mCamera.bufferCaptured(mImageScanner);
}

void bordaiApp::draw() {
	gl::enableAlphaBlending();
	gl::clear( Color::black() );
	
	if( mCamera.hasSomething() ) {
		
		gl::color( Color::white() );
		gl::setMatricesWindow( mWindowSize.x, mWindowSize.y );
		glPushMatrix();
		
		Rectf drawArea( 0, 0, mWindowSize.x, mWindowSize.y );
		mCamera.draw(drawArea);
		mImageScanner.draw(drawArea);
		
		glPopMatrix();
		
	}
	
	params::InterfaceGl::draw();
}

CINDER_APP_BASIC( bordaiApp, RendererGl )
