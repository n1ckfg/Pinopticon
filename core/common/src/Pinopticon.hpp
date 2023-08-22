#pragma once

#include "ofMain.h"

namespace Pinopticon {

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
        string returns;
        ofBuffer hostNameFile;

        try {
#ifdef TARGET_OSX
            returns = cleanString(ofSystem("hostname"));
            returns = ofSplitString(returns, ".")[0];
#endif
#ifdef TARGET_WIN32
            returns = cleanString(ofSystem("hostname"));
            returns = ofSplitString(returns, ".")[0];
#endif
#ifdef TARGET_LINUX
            hostNameFile = ofBufferFromFile("/etc/hostname");
            returns = cleanString(hostNameFile.getText());
#endif
        } catch (std::exception &e) {
            cout << "Exception: " << e.what() << endl;
        }

        if (returns.size() < 1) returns = "unknown";            

        cout << "Hostname: " << returns << endl;
        return returns;
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

}
