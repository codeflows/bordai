#include "cinder/app/AppBasic.h"

#include "CapturingDevice.h"

#define WIDTH 800	
#define HEIGHT 600

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
};

void bordaiApp::prepareSettings(Settings *settings) {
	settings -> setFrameRate(60.0f);
	settings -> setWindowSize(WIDTH, HEIGHT);
	mCamera.setLensSize(WIDTH, HEIGHT);
	mCamera.setImageScanner(ImageScanner( getResourcePath( "haarcascade_frontalface_alt2.xml" ) ));
}

void bordaiApp::setup() {
	mCamera.startCapturing();
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
	mCamera.bufferCaptured();
}

void bordaiApp::draw()
{
	gl::enableAlphaBlending();
	gl::clear( Color::black() );
	
	if( mCamera.hasSomething() ) {
		mCamera.draw();
	}
}

CINDER_APP_BASIC( bordaiApp, RendererGl )
