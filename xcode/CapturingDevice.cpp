/*
 *  CapturingDevice.cpp
 *  targeter
 *
 *  Created by Kai Rasi on 5/15/11.
 *  Copyright 2011 Reaktor Innovations. All rights reserved.
 *
 */
#include "cinder/app/AppBasic.h"
#include "cinder/Text.h"
#include "CapturingDevice.h"

using namespace ci;
using namespace ci::app;
using namespace std;

CapturingDevice::CapturingDevice() {}

void CapturingDevice::setLensSize(const int lensWidth, const int lensHeight) {
	mLensWidth = lensWidth;
	mLensHeight = lensHeight;
}

void CapturingDevice::startCapturing() {
	vector<Capture::DeviceRef> allDevices (Capture::getDevices());
	for (vector<Capture::DeviceRef>::const_iterator aDevice = allDevices.begin(); aDevice != allDevices.end(); ++aDevice) {
		Capture::DeviceRef device = *aDevice;
		console() << "Located device " << device->getName() << " with id " << device->getUniqueId() << std::endl;
		try {
			if (device->checkAvailable()) {
				// bindNameTexture(device);
				mCaptures.push_back( Capture( mLensWidth, mLensHeight, device ) );
				mCaptures.back().start();

				// bindCapture(device);
				TextLayout layout;
				layout.setFont( Font( "Arial", 16 ) );
				layout.setColor( Color( 1, 1, 0 ) );
				layout.addLine( device->getName() );
				mDeviceNameTextures.push_back( gl::Texture( layout.render( true ) ) );				
			}
		}
		catch (CaptureExc &) {
			console() << "Cannot use device " << device->getName() << std::endl;
		}
	}		
}

void CapturingDevice::toggleOnOff() {
	mCaptures.back().isCapturing() ? mCaptures.back().stop() : mCaptures.back().start();
}


/*
void bindCapturingToAvailableDevices() {

		*/
	/*
	 // list out the devices
	 vector<Capture::DeviceRef> devices( Capture::getDevices() );
	 for( vector<Capture::DeviceRef>::const_iterator deviceIt = devices.begin(); deviceIt != devices.end(); ++deviceIt ) {
	 Capture::DeviceRef device = *deviceIt;
	 console() << "Found Device " << device->getName() << " ID: " << device->getUniqueId() << std::endl;
	 try {
	 if( device->checkAvailable() ) {
	 mCaptures.push_back( Capture( WIDTH, HEIGHT, device ) );
	 mCaptures.back().start();
	 
	 // placeholder text
	 mTextures.push_back( gl::Texture() );
	 
	 // render the name as a texture
	 TextLayout layout;
	 layout.setFont( Font( "Arial", 24 ) );
	 layout.setColor( Color( 1, 1, 1 ) );
	 layout.addLine( device->getName() );
	 mNameTextures.push_back( gl::Texture( layout.render( true ) ) );
	 }
	 console() << "device is NOT available" << std::endl;
	 }
	 catch( CaptureExc & ) {
	 console() << "Unable to initialize device: " << device->getName() << endl;
	 }
	 }	
	 
*/
//}
