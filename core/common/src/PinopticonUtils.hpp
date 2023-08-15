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

    int getTimestamp() {
        int returns = 0;
#if OF_VERSION_MAJOR >= 0 && OF_VERSION_MINOR >= 10
		returns = (int) ofGetSystemTimeMillis();   
#else
        returns = (int) ofGetSystemTime();         
#endif
        return returns;
    }


    // ~ ~ ~ TO BUFFER ~ ~ ~

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

    // silently fails
    void floatsToBuffer(float* data, ofBuffer& buffer) {
        char const * fChars = reinterpret_cast<char const *>(data);
        std::string fString(fChars, fChars + sizeof data);
        buffer.set(fString); 
    }


    // ~ ~ ~ FROM BUFFER ~ ~ ~

    void bufferToImage(ofBuffer& buffer, ofImage& image, int w, int h, bool isRgb) {
        if (isRgb) {
            image.allocate(w, h, OF_IMAGE_COLOR);
        } else {
            image.allocate(w, h, OF_IMAGE_GRAYSCALE);
        }
#if OF_VERSION_MAJOR >= 0 && OF_VERSION_MINOR >= 10
        image.load(buffer);
#else
        cout << "Find alternative for loading buffer into image on older oF versions" << endl;
#endif
    }

    // ~ ~ ~ 3D ~ ~ ~

    float rawDepthToMeters(int depthValue) {
      if (depthValue < 2047) {
        return (float)(1.0 / ((double)(depthValue) * -0.0030711016 + 3.3309495161));
      }
      return 0.0;
    }


    // ~ ~ ~ OSC ~ ~ ~

    void sendOscVideo(ofxOscSender& sender, string hostName, string sessionId, ofBuffer& videoBuffer, int timestamp) {
        ofxOscMessage m;
        m.setAddress("/video"); // ssbi

        m.addStringArg(hostName);    
        m.addStringArg(sessionId);    
        m.addBlobArg(videoBuffer);
        m.addIntArg(timestamp);
        
        sender.sendMessage(m);
    }

    void sendOscBlobs(ofxOscSender& sender, string hostName, string sessionId, int index, float x, float y, int timestamp) {
        ofxOscMessage m;
        m.setAddress("/blob"); // ssiffi

        m.addStringArg(hostName);   
        m.addStringArg(sessionId);
        m.addIntArg(index);  
        m.addFloatArg(x);
        m.addFloatArg(y);
        m.addIntArg(timestamp);

        sender.sendMessage(m);
    }

    void sendOscBlobs(ofxOscSender& sender, string hostName, string sessionId, int index, float x, float y, float z, int timestamp) {
        ofxOscMessage m;
        m.setAddress("/blob"); // ssifffi

        m.addStringArg(hostName);
        m.addStringArg(sessionId);
        m.addIntArg(index);
        m.addFloatArg(x);
        m.addFloatArg(y);
        m.addFloatArg(z);
        m.addIntArg(timestamp);

        sender.sendMessage(m);
    }

    void sendOscContours(ofxOscSender& sender, string hostName, string sessionId, int index, ofBuffer& contourColorBuffer, ofBuffer& contourPointsBuffer, int timestamp) {
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

    void sendOscPixel(ofxOscSender& sender, string hostName, string sessionId, float x, float y, int timestamp) {
        ofxOscMessage m;
        m.setAddress("/pixel"); // ssffi

        m.addStringArg(hostName);   
        m.addStringArg(sessionId);   
        m.addFloatArg(x);
        m.addFloatArg(y);
        m.addIntArg(timestamp);
        
        sender.sendMessage(m);
    }

}