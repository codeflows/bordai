#include "cinder/app/AppBasic.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
#include "cinder/Capture.h"
#include "cinder/Text.h"
#include "CinderOpenCv.h"

#include "CapturingDevice.h"

#define WIDTH 640
#define HEIGHT 480
#define HISTOGRAM_SCALE 2

using namespace ci;
using namespace ci::app;
using namespace std;

class targeterApp : public AppBasic {
  public:
	void prepareSettings(Settings *settings);
	void setup();
	void keyDown( KeyEvent event );
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void updateTargets( Surface cameraImage );
	
  private:
	CapturingDevice mCamera;
	vector<gl::Texture>	mTextures;
	vector<gl::Texture>	mNameTextures;
	vector<Surface>		mRetainedSurfaces;
	cv::CascadeClassifier mCascade;
	vector<Rectf>		mTargets;
};

void targeterApp::prepareSettings(Settings *settings) {
	settings -> setWindowSize(WIDTH, HEIGHT);
	settings -> setFrameRate(60.0f);
	mCamera.setLensSize(WIDTH, HEIGHT);
}

void targeterApp::setup() {
	mCascade.load( getResourcePath( "haarcascade_frontalface_alt.xml" ) );
	mCamera.startCapturing();	
}

void targeterApp::keyDown( KeyEvent event ) {
	if(event.getChar() == 'f') {
		setFullScreen( !isFullScreen() );
	}
	if(event.getChar() == ' ') {
		mCamera.toggleOnOff();
	}
}

void targeterApp::mouseDown( MouseEvent event ) {}

void targeterApp::update() {
	
	for( vector<Capture>::iterator cIt = mCamera.mCaptures.begin(); cIt != mCamera.mCaptures.end(); ++cIt ) {
		if( cIt->checkNewFrame() ) {
			Surface8u surf = cIt->getSurface();
			mTextures[cIt - mCaptures.begin()] = gl::Texture( surf );
			updateTargets( surf );
		}
	}
}
	
void targeterApp::updateTargets( Surface cameraImage ) {
	cv::Mat grayCameraImage( toOcv( cameraImage, CV_8UC1 ) );
	int histogramWidth = cameraImage.getWidth() / HISTOGRAM_SCALE;
	int histogramHeigth = cameraImage.getHeight() / HISTOGRAM_SCALE;
	cv::Mat histogramImage( histogramHeigth, histogramWidth, CV_8UC1);
	cv::resize( grayCameraImage, histogramImage, histogramImage.size(), 0, 0, cv::INTER_LINEAR );
	
	cv::equalizeHist( histogramImage, histogramImage );
	
	mTargets.clear();
	
	vector<cv::Rect> targets;
	mCascade.detectMultiScale( histogramImage, targets );
	
	for (vector<cv::Rect>::const_iterator targetsIter = targets.begin(); targetsIter != targets.end(); ++targetsIter) {
		Rectf targetRect( fromOcv( *targetsIter ) );
		targetRect *= HISTOGRAM_SCALE;
		mTargets.push_back( targetRect );
	}
}

void targeterApp::draw()
{
	gl::enableAlphaBlending();
	gl::clear( Color::black() );
	
	if( mCamera.mCaptures.empty() )
		return;
	
	float width = getWindowWidth() / mCamera.mCaptures.size();	
	float height = width / ( WIDTH / (float)HEIGHT );
	float x = 0, y = ( getWindowHeight() - height ) / 2.0f;
	
	for( vector<Capture>::iterator cIt = mCamera.mCaptures.begin(); cIt != mCamera.mCaptures.end(); ++cIt ) {	
		// draw the latest frame
		gl::color( Color::white() );
		if( mTextures[cIt-mCaptures.begin()] ) {
			gl::Texture latestFromCamera = mTextures[cIt-mCaptures.begin()];
			gl::draw(latestFromCamera , Rectf( x, y, x + width, y + height ) );
			latestFromCamera.disable();
			for (vector<Rectf>::const_iterator targetIter = mTargets.begin(); targetIter != mTargets.end(); ++targetIter) {
				gl::color( ColorA( 0, 0, 1, 0.35f ) );
				gl::drawSolidRect( *targetIter );
				gl::color( ColorA( 1, 1, 0, 0.45f ) );
				Rectf rec = *targetIter;
				float scalex = getWindowWidth() / (float)WIDTH;
				float scaley = getWindowHeight() / (float)HEIGHT;
				rec.set(rec.getX1() * scalex, rec.getY1() * scaley, rec.getX2() * scalex, rec.getY2() * scaley);
				gl::drawSolidRect( rec );
			}
		}
		
		// draw the name
		gl::color( Color::black() );	
		gl::draw( mNameTextures[cIt-mCaptures.begin()], Vec2f( x + 10 + 1, y + 10 + 1 ) );
		gl::color( Color( 0.5, 0.75, 1 ) );
		gl::draw( mNameTextures[cIt-mCaptures.begin()], Vec2f( x + 10, y + 10 ) );
		
		x += width;
	}	
}


CINDER_APP_BASIC( targeterApp, RendererGl )
