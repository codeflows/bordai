#define HISTOGRAM_SCALE 2

#include "HaarDetector.h"

using namespace ci;
using namespace ci::app;
using namespace std;

HaarDetector::HaarDetector() {}

HaarDetector::HaarDetector(string cascadeFilePath) {
	mCascade.load( cascadeFilePath );
}

void HaarDetector::scanTrackables(Surface cameraImage) {	
	cv::Mat grayCameraImage( toOcv( cameraImage, CV_8UC1 ) );
	int histogramWidth = cameraImage.getWidth() / HISTOGRAM_SCALE;
	int histogramHeigth = cameraImage.getHeight() / HISTOGRAM_SCALE;
	cv::Mat histogramImage( histogramHeigth, histogramWidth, CV_8UC1);
	cv::resize( grayCameraImage, histogramImage, histogramImage.size(), 0, 0, cv::INTER_LINEAR );
	
	cv::equalizeHist( histogramImage, histogramImage );
	
	mScans.clear();
	mHistogramTextures.clear();
	mHistogramTextures.push_back(gl::Texture(fromOcv(histogramImage)));

	vector<cv::Rect> newScans;
	mCascade.detectMultiScale( histogramImage, newScans );
		
	for (vector<cv::Rect>::const_iterator aScan = newScans.begin(); aScan != newScans.end(); ++aScan) {
		Rectf scanLocation( fromOcv( *aScan ) );
		mScans.push_back( scanLocation );
	}
}

void HaarDetector::drawTrackings() {
	gl::color( ColorA( 1, 1, 0, 0.45f ) );
	
	for (vector<Rectf>::const_iterator aScan = mScans.begin(); aScan != mScans.end(); ++aScan) {
		gl::drawSolidRect( *aScan );
	}
}