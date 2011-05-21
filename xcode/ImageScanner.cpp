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

void ImageScanner::scan( ci::Surface cameraImage ) {
	cv::Mat grayCameraImage( toOcv( cameraImage, CV_8UC1 ) );
	int histogramWidth = cameraImage.getWidth() / HISTOGRAM_SCALE;
	int histogramHeigth = cameraImage.getHeight() / HISTOGRAM_SCALE;
	cv::Mat histogramImage( histogramHeigth, histogramWidth, CV_8UC1);
	cv::resize( grayCameraImage, histogramImage, histogramImage.size(), 0, 0, cv::INTER_LINEAR );
	
	cv::equalizeHist( histogramImage, histogramImage );
	
	mHistograms.clear();
	
	vector<cv::Rect> newScans;
	mCascade.detectMultiScale( histogramImage, newScans );
	
	for (vector<cv::Rect>::const_iterator aScan = newScans.begin(); aScan != newScans.end(); ++aScan) {
		Rectf scanLocation( fromOcv( *aScan ) );
		scanLocation *= HISTOGRAM_SCALE;
		mHistograms.push_back( Histogram(fromOcv(grayCameraImage), scanLocation) );
	}
}

void ImageScanner::draw(ci::Rectf drawArea) {
	for (vector<Histogram>::const_iterator aHistogram = mHistograms.begin(); aHistogram != mHistograms.end(); ++aHistogram) {
		gl::color( ColorA( 1, 1, 0, 0.45f ) );
		ImageSourceRef img = aHistogram->mHistogramImage;
		gl::drawSolidRect( aHistogram->mScanLocation );
	}	
}