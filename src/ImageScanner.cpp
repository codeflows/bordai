#define HISTOGRAM_SCALE 2

#include "cinder/app/AppBasic.h"
#include "ImageScanner.h"

using namespace ci;
using namespace ci::app;
using namespace std;

ImageScanner::ImageScanner() {}

ImageScanner::ImageScanner(string cascadeFilePath) {
	mCascade.load( cascadeFilePath );
}

void ImageScanner::scan(Surface cameraImage) {
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

void ImageScanner::draw(ci::Rectf drawArea) {
	gl::Texture histogramTexture = gl::Texture(mHistogram.mHistogramImage);
	float x = drawArea.getWidth() / (float)histogramTexture.getWidth();
	float y = drawArea.getHeight() / (float)histogramTexture.getHeight();
	
	for (vector<Rectf>::const_iterator aScan = mHistogram.mScans.begin(); aScan != mHistogram.mScans.end(); ++aScan) {
		Rectf sLoc = *aScan;
		Rectf scaledScanLocation(sLoc.getX1() * x, sLoc.getY1() * y, sLoc.getX2() * x, sLoc.getY2() * y);
		
		gl::drawSolidRect( scaledScanLocation );
	}	
}

void ImageScanner::drawHistogram(ci::Rectf drawArea) {
	gl::Texture histogramTexture = gl::Texture(mHistogram.mHistogramImage);
	gl::draw(histogramTexture, drawArea);
	histogramTexture.disable();
}