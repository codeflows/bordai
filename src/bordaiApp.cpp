#include "cinder/app/AppBasic.h"
#include "cinder/params/Params.h"

#include "CapturingDevice.h"

#define WIDTH 640
#define HEIGHT 480

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
	params::InterfaceGl mParams;
	int mCameraLensWidth, mCameraLensHeight;
};

void bordaiApp::prepareSettings(Settings *settings) {
	settings -> setFrameRate(25.0f);
	settings -> setWindowSize(WIDTH, HEIGHT);
}

void bordaiApp::setup() {
	mCameraLensWidth = WIDTH;
	mCameraLensHeight = HEIGHT;
	
	mParams = params::InterfaceGl("bordai", Vec2i(10, 10));
	mParams.addParam("Lens width", &mCameraLensWidth, "min=256 max=1024 step=64 keyIncr=W keyDecr=w");
	mParams.addParam("Lens height", &mCameraLensHeight, "min=256 max=1024 step=64 keyIncr=H keyDecr=h");
	
	mCamera.setImageScanner(ImageScanner( getResourcePath( "haarcascade_frontalface_alt2.xml" ) ));
	mCamera.setLensSize(mCameraLensWidth, mCameraLensHeight);
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
	mCamera.setLensSize(mCameraLensWidth, mCameraLensHeight);
	mCamera.bufferCaptured();
}

void bordaiApp::draw() {
	gl::enableAlphaBlending();
	gl::clear( Color::black() );
	
	if( mCamera.hasSomething() ) {
		mCamera.draw();
	}
	
	params::InterfaceGl::draw();
}

CINDER_APP_BASIC( bordaiApp, RendererGl )
