#define HISTOGRAM_SCALE 2

#include "cinder/app/AppBasic.h"
#include "ImageTracker.h"

using namespace ci;
using namespace ci::app;
using namespace std;

ImageTracker::ImageTracker() {}

ImageTracker::ImageTracker(string cascadeFilePath) {
	mCascade.load( cascadeFilePath );
}

void ImageTracker::scanTrackables(Surface cameraImage) {
	cv::Mat colorCameraImage( toOcv( cameraImage ) );
	mCardDetector.findSquares(colorCameraImage, mHistogram.mStoryCards);
	
	cv::Mat grayCameraImage( toOcv( cameraImage, CV_8UC1 ) );
	int histogramWidth = cameraImage.getWidth() / HISTOGRAM_SCALE;
	int histogramHeigth = cameraImage.getHeight() / HISTOGRAM_SCALE;
	cv::Mat histogramImage( histogramHeigth, histogramWidth, CV_8UC1);
	cv::resize( grayCameraImage, histogramImage, histogramImage.size(), 0, 0, cv::INTER_LINEAR );
	
	cv::equalizeHist( histogramImage, histogramImage );
	
	mHistogram.mScans.clear();
	
	vector<cv::Rect> newScans;
	mCascade.detectMultiScale( histogramImage, newScans );
	
	mHistogram.mHistogramImage = fromOcv(histogramImage);
	
	for (vector<cv::Rect>::const_iterator aScan = newScans.begin(); aScan != newScans.end(); ++aScan) {
		Rectf scanLocation( fromOcv( *aScan ) );
		mHistogram.mScans.push_back( scanLocation );
	}
}

void ImageTracker::drawTrackings(Rectf drawArea) {
	gl::Texture histogramTexture = gl::Texture(mHistogram.mHistogramImage);
	float x = drawArea.getWidth() / (float)histogramTexture.getWidth();
	float y = drawArea.getHeight() / (float)histogramTexture.getHeight();
	
	for (vector<Rectf>::const_iterator aScan = mHistogram.mScans.begin(); aScan != mHistogram.mScans.end(); ++aScan) {
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
	
	for (vector<Rectf>::const_iterator aScan = mHistogram.mStoryCards.begin(); aScan != mHistogram.mStoryCards.end(); ++aScan) {
		Rectf scanLocation = *aScan;
		Rectf scaledLoc(scanLocation.getUpperLeft(), scanLocation.getLowerRight());
		
		// scale to screen resolution
		scaledLoc.x1 *= x / (float)HISTOGRAM_SCALE;
		scaledLoc.x2 *= x / (float)HISTOGRAM_SCALE;
		scaledLoc.y1 *= y / (float)HISTOGRAM_SCALE;
		scaledLoc.y2 *= y / (float)HISTOGRAM_SCALE;
		
		// move to drawing area
		scaledLoc.x1 += drawArea.x1;
		scaledLoc.x2 += drawArea.x1;
		scaledLoc.y1 += drawArea.y1;
		scaledLoc.y2 += drawArea.y1;
		
		gl::drawSolidRect( scaledLoc );
		
	}
}

void ImageTracker::drawHistogram(Rectf drawArea) {
	gl::Texture histogramTexture = gl::Texture(mHistogram.mHistogramImage);
	gl::draw(histogramTexture, drawArea);
	histogramTexture.disable();
}