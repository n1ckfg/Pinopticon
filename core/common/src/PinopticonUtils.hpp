#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

namespace PinopticonUtils {

    string cleanString(string input) {
        ofStringReplace(input, "\n", "");
        ofStringReplace(input, "\r", ""); 
        return input;   
    }

    // a randomly generated id that isn't saved
    string getSessionId() {
        string sessionId = "RPi_" + ofGetTimestampString("%y%m%d%H%M%S%i");
        return cleanString(sessionId);
    }

    // the RPi network hostname 
    string getHostName() {
        ofBuffer hostNameFile = ofBufferFromFile("/etc/hostname");
        return cleanString(hostNameFile.getText());
    }

    void imageToBuffer(ofImage& img, ofBuffer& buffer, int quality) {
        switch(quality) {
            case 5:
                ofSaveImage(img, buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_BEST);
                break;
            case 4:
                ofSaveImage(img, buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_HIGH);
                break;
            case 3:
                ofSaveImage(img, buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_MEDIUM);
                break;
            case 2:
                ofSaveImage(img, buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_LOW);
                break;
            case 1:
                ofSaveImage(img, buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_WORST);
                break;
        }
    }

    void pixelsToBuffer(ofPixels& pixels, ofBuffer& buffer, int quality) {
        ofImage img;
        img.setFromPixels(pixels);
        imageToBuffer(img, buffer, quality);
    }

    void fboToBuffer(ofFbo& fbo, ofBuffer& buffer, int quality) {
        // jpegs have no alpha, so fbo must be initialized with GL_RGB, not GL_RGBA!
        ofPixels pixels;
        ofImage img;
        fbo.readToPixels(pixels);
        img.setFromPixels(pixels);
        imageToBuffer(img, buffer, quality);
    }

    void floatsToBuffer(float& data, ofBuffer& buffer) {
        char const * fChars = reinterpret_cast<char const *>(data);
        std::string fString(fChars, fChars + sizeof data);
        buffer.set(fString); 
    }

    float rawDepthToMeters(int depthValue) {
      if (depthValue < 2047) {
        return (float)(1.0 / ((double)(depthValue) * -0.0030711016 + 3.3309495161));
      }
      return 0.0;
    }

    // ~ ~ ~ OSC ~ ~ ~

    void sendOscVideo(string hostName, string sessionId, ofBuffer& videobuffer, int timestamp) {
        ofxOscMessage m;
        m.setAddress("/video");

        m.addStringArg(hostName);    
        m.addStringArg(sessionId);    
        m.addBlobArg(videoBuffer);
        m.addIntArg(timestamp);
        
        sender.sendMessage(m);
    }

    void sendOscBlobs(string hostName, string sessionId, int index, float x, float y, int timestamp) {
        ofxOscMessage m;
        m.setAddress("/blob"); // ssiffi

        m.addStringArg(hostName);   
        m.addStringArg(sessionId);
        m.addIntArg(index);  
        m.addFloatArg(x / (float) width);
        m.addFloatArg(y / (float) height);
        m.addIntArg(timestamp);

        sender.sendMessage(m);
    }

    void sendOscContours(string hostName, string sessionId, int index, ofBuffer& contourColorBuffer, ofBuffer& contourPointsBuffer, int timestamp) {
        ofxOscMessage m;
        m.setAddress("/contour"); // ssibbi
        
        m.addStringArg(hostName);
        m.addStringArg(sessionId);
        m.addIntArg(index);
        m.addBlobArg(contourColorBuffer);
        m.addBlobArg(contourPointsBuffer);
        m.addIntArg(timestamp);

        sender.sendMessage(m);
    }

    void sendOscPixel(string hostName, string sessionId, float x, float y, int timestamp) {
        ofxOscMessage m;
        m.setAddress("/pixel"); // ssffi

        m.addStringArg(hostName);   
        m.addStringArg(sessionId);   
        m.addFloatArg(x / (float) width);
        m.addFloatArg(y / (float) height);
        m.addIntArg(timestamp);
        
        sender.sendMessage(m);
    }

}