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
	
	vector<cv::Rect> newScans;
	mCascade.detectMultiScale( histogramImage, newScans );
	
	mHistogramImage = fromOcv(histogramImage);
	
	for (vector<cv::Rect>::const_iterator aScan = newScans.begin(); aScan != newScans.end(); ++aScan) {
		Rectf scanLocation( fromOcv( *aScan ) );
		mScans.push_back( scanLocation );
	}
}

void HaarDetector::drawTrackings(Rectf drawArea) {
	gl::Texture histogramTexture = gl::Texture(mHistogramImage);
	float x = drawArea.getWidth() / (float)histogramTexture.getWidth();
	float y = drawArea.getHeight() / (float)histogramTexture.getHeight();
	
	for (vector<Rectf>::const_iterator aScan = mScans.begin(); aScan != mScans.end(); ++aScan) {
		Rectf scanLocation = *aScan;
		Rectf scaledLoc(scanLocation.getUpperLeft(), scanLocation.getLowerRight());
		
		// scale to screen resolution
		scaledLoc.x1 *= x;
		scaledLoc.x2 *= x;
		scaledLoc.y1 *= y;
		scaledLoc.y2 *= y;

		// move to drawing area
		scaledLoc.x1 += drawArea.x1;
		scaledLoc.x2 += drawArea.x1;
		scaledLoc.y1 += drawArea.y1;
		scaledLoc.y2 += drawArea.y1;
		
		gl::drawSolidRect( scaledLoc );
	}
}

void HaarDetector::drawHistogram(Rectf drawArea) {
	gl::Texture histogramTexture = gl::Texture(mHistogramImage);
	gl::draw(histogramTexture, drawArea);
	histogramTexture.disable();
}