#define HISTOGRAM_SCALE 2

#include "cinder/app/AppBasic.h"
#include "ImageScanner.h"

using namespace ci;
using namespace ci::app;
using namespace std;

ImageScanner::ImageScanner() {}

ImageScanner::ImageScanner(std::string cascadeFilePath) {
	mCascade.load( cascadeFilePath );
}

void ImageScanner::setImageSize(const int imageWidth, const int imageHeight) {
	mImageWidth = imageWidth;
	mImageHeight = imageHeight;
}

void ImageScanner::scan( ci::Surface cameraImage ) {
	cv::Mat grayCameraImage( toOcv( cameraImage, CV_8UC1 ) );
	int histogramWidth = cameraImage.getWidth() / HISTOGRAM_SCALE;
	int histogramHeigth = cameraImage.getHeight() / HISTOGRAM_SCALE;
	cv::Mat histogramImage( histogramHeigth, histogramWidth, CV_8UC1);
	cv::resize( grayCameraImage, histogramImage, histogramImage.size(), 0, 0, cv::INTER_LINEAR );
	
	cv::equalizeHist( histogramImage, histogramImage );
	
	mScans.clear();
	
	vector<cv::Rect> newScans;
	mCascade.detectMultiScale( histogramImage, newScans );
	
	for (vector<cv::Rect>::const_iterator aScan = newScans.begin(); aScan != newScans.end(); ++aScan) {
		Rectf scanLocation( fromOcv( *aScan ) );
		scanLocation *= HISTOGRAM_SCALE;
		mScans.push_back( scanLocation );
	}
}

void ImageScanner::draw() {
	for (vector<Rectf>::const_iterator aScan = mScans.begin(); aScan != mScans.end(); ++aScan) {
		float scalex = getWindowWidth() / (float)mImageWidth;
		float scaley = getWindowHeight() / (float)mImageHeight;
		Rectf loc = *aScan;
		loc.set(loc.getX1() * scalex, loc.getY1() * scaley, loc.getX2() * scalex, loc.getY2() * scaley);
		gl::color( ColorA( 1, 1, 0, 0.45f ) );
		gl::drawSolidRect( loc );
	}	
}