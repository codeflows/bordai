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

	/*
	float mThreshold = 70.0f;
	float mBlobMin = 20.0f;
	float mBlobMax = 80.0f;
	// once the surface is avalable pass it to opencv
	// had trouble here with bit depth. surface comes in full color, needed to crush it down
	cv::Mat blurred, thresholded, thresholded2, output;
	
	cv::blur(histogramImage, blurred, cv::Size(10,10));
	
	// make two thresholded images one to display and one
	// to pass to find contours since its process alters the image
	cv::threshold( blurred, thresholded, mThreshold, 255, CV_THRESH_BINARY);
	cv::threshold( blurred, thresholded2, mThreshold, 255, CV_THRESH_BINARY);
	
	// 2d vector to store the found contours
	vector<vector<cv::Point> > contours;
	// find em
	cv::findContours(thresholded, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	
	// loop the stored contours
	for (vector<vector<cv::Point> >::iterator it=contours.begin() ; it < contours.end(); it++ ){
		// center abd radius for current blob
		cv::Point2f center;
		float radius;
		// convert the cuntour point to a matrix 
		vector<cv::Point> pts = *it;
		cv::Mat pointsMatrix = cv::Mat(pts);
		// pass to min enclosing circle to make the blob 
		cv::minEnclosingCircle(pointsMatrix, center, radius);
		
		if (radius > mBlobMin && radius < mBlobMax) {
			Rectf hand (center.x - radius, center.y - radius, center.x + radius, center.y + radius);
			hand *= HISTOGRAM_SCALE;
			mScans.push_back( hand );
		}
	}
	 */
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