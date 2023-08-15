#pragma once

#include "ofMain.h"
#include "ofxHTTP.h"
#include "ofxJSONElement.h"
#include "ofxCrypto.h"

// Don't include this unless you need it--ofxHTTP has many dependencies and
// may make your project less portable.

namespace PinopticonUtils {

    // ~ ~ ~ WS ~ ~ ~

    void sendWsVideo(ofxHTTP::SimpleWebSocketServer& wsServer, string hostName, string sessionId, ofBuffer& videoBuffer, int timestamp) { 
        string msg = "{\"unique_id\":\"" + sessionId + "\",\"hostname\":\"" + hostName + "\",\"video\":\"" + ofxCrypto::base64_encode(videoBuffer) + "\",\"timestamp\":\"" + ofToString(timestamp) + "\"}";
        wsServer.webSocketRoute().broadcast(ofxHTTP::WebSocketFrame(cleanString(msg)));
    }

    void sendWsBlobs(ofxHTTP::SimpleWebSocketServer& wsServer, string hostName, string sessionId, int index, float x, float y, int timestamp) {          
        string msg = "{\"unique_id\":\"" + sessionId + "\",\"hostname\":\"" + hostName + "\",\"index\":\"" + ofToString(index) + "\",\"x\":\"" + ofToString(x) + "\",\"y\":\"" + ofToString(y) + "\",\"timestamp\":\"" + ofToString(timestamp) + "\"}";
        wsServer.webSocketRoute().broadcast(ofxHTTP::WebSocketFrame(cleanString(msg)));
    }

    void sendWsContours(ofxHTTP::SimpleWebSocketServer& wsServer, string hostName, string sessionId, int index, ofBuffer& contourColorBuffer, ofBuffer& contourPointsBuffer, int timestamp) {
        string msg = "{\"unique_id\":\"" + sessionId + "\",\"hostname\":\"" + hostName + "\",\"index\":" + ofToString(index) + ",\"colors\":\"" + ofxCrypto::base64_encode(contourColorBuffer) + "\",\"points\":\"" + ofxCrypto::base64_encode(contourPointsBuffer) + "\",\"timestamp\":" + ofToString(timestamp) + "}";
        wsServer.webSocketRoute().broadcast(ofxHTTP::WebSocketFrame(cleanString(msg)));
    }

    void sendWsPixel(ofxHTTP::SimpleWebSocketServer& wsServer, string hostName, string sessionId, float x, float y, int timestamp) {   
        string msg = "{\"unique_id\":\"" + sessionId + "\",\"hostname\":\"" + hostName + "\",\"x\":\"" + ofToString(x) + "\",\"y\":\"" + ofToString(y) + "\",\"timestamp\":\"" + ofToString(timestamp) + "\"}";
        wsServer.webSocketRoute().broadcast(ofxHTTP::WebSocketFrame(cleanString(msg)));
    }

}