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
	float mFrameRate;
};

void bordaiApp::prepareSettings(Settings *settings) {
	mCameraLensWidth = WIDTH;
	mCameraLensHeight = HEIGHT;
	mFrameRate = 25.0f;
	
	settings -> setFrameRate(mFrameRate);
	settings -> setWindowSize(mCameraLensWidth, mCameraLensHeight);
	settings -> setResizable(true);
}

void bordaiApp::setup() {
	mParams = params::InterfaceGl("bordai", Vec2i(50, 50));
	mParams.addParam("Width", &mCameraLensWidth, "", true);
	mParams.addParam("Height", &mCameraLensHeight, "", true);
	mParams.addParam("Framerate", &mFrameRate, "min=5.0 max=70.0 step=5.0 keyIncr=F keyDecr=f");
	
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
	setFrameRate(mFrameRate);
	mCameraLensWidth = getWindowWidth();
	mCameraLensHeight = getWindowHeight();
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
