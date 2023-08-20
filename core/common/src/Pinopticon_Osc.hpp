#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

namespace Pinopticon {

    void setupOscSender(ofxOscSender& sender, string& oscSendHost, int oscSendPort) {
        sender.setup(oscSendHost, oscSendPort);
        cout << "\nSending OSC to " << oscSendHost << " on port: " << oscSendPort << endl;
    }

    void setupOscReceiver(ofxOscReceiver& receiver, int oscReceivePort) {
        receiver.setup(oscReceivePort);
        cout << "\nReceiving OSC on port: " << oscReceivePort << endl;
    }

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