#pragma once

#include "ofMain.h"
#include "ofxHTTP.h"
#include "ofxJSONElement.h"
#include "ofxCrypto.h"

// Don't include this unless you need it--ofxHTTP has many dependencies and
// may make your project less portable.

namespace Pinopticon {

    // * stream video *
    // https://github.com/bakercp/ofxHTTP/blob/master/libs/ofxHTTP/include/ofx/HTTP/IPVideoRoute.h
    // https://github.com/bakercp/ofxHTTP/blob/master/libs/ofxHTTP/src/IPVideoRoute.cpp
    void setupMjpeg(ofxHTTP::SimpleIPVideoServer& streamServer, int streamPort, int maxClientConnections, int maxClientBitRate, int maxClientFrameRate, int maxClientQueueSize, int maxStreamWidth, int maxStreamHeight, string defaultIndex) {
        ofxHTTP::SimpleIPVideoServerSettings streamSettings;
        streamSettings.setPort(streamPort);
        streamSettings.ipVideoRouteSettings.setMaxClientConnections(maxClientConnections); // default 5
        streamSettings.ipVideoRouteSettings.setMaxClientBitRate(maxClientBitRate); // default 1024
        streamSettings.ipVideoRouteSettings.setMaxClientFrameRate(maxClientFrameRate); // default 30
        streamSettings.ipVideoRouteSettings.setMaxClientQueueSize(maxClientQueueSize); // default 10
        streamSettings.ipVideoRouteSettings.setMaxStreamWidth(maxStreamWidth); // default 1920
        streamSettings.ipVideoRouteSettings.setMaxStreamHeight(maxStreamHeight); // default 1080
        streamSettings.fileSystemRouteSettings.setDefaultIndex(defaultIndex); // default "live_view.html"
        streamServer.setup(streamSettings);
        cout << "\nStarting MJPEG stream..." << endl;
        streamServer.start();
    }

    // * post form *
    // https://bakercp.github.io/ofxHTTP/classofx_1_1_h_t_t_p_1_1_simple_post_server_settings.html
    // https://github.com/bakercp/ofxHTTP/blob/master/libs/ofxHTTP/src/PostRoute.cpp
    template <class ListenerClass>
    void setupHttp(ListenerClass* listener, ofxHTTP::SimplePostServer& postServer, int postPort, string uploadRedirect) {
        ofxHTTP::SimplePostServerSettings postSettings;
        postSettings.setPort(postPort);
        postSettings.postRouteSettings.setUploadRedirect(uploadRedirect); // default "result.html"
        postServer.setup(postSettings);
        postServer.postRoute().registerPostEvents(listener);
        cout << "\nStarting HTTP server..." << endl;
        postServer.start();
    }

    // * websockets *
    // https://github.com/bakercp/ofxHTTP/blob/master/libs/ofxHTTP/include/ofx/HTTP/WebSocketConnection.h
    // https://github.com/bakercp/ofxHTTP/blob/master/libs/ofxHTTP/src/WebSocketConnection.cpp
    // events: connect, open, close, idle, message, broadcast        
    template <class ListenerClass>
    void setupWsServer(ListenerClass* listener, ofxHTTP::SimpleWebSocketServer& wsServer, int wsPort) {
        ofxHTTP::SimpleWebSocketServerSettings wsSettings;
        wsSettings.setPort(wsPort);
        wsServer.setup(wsSettings);
        wsServer.webSocketRoute().registerWebSocketEvents(listener);
        cout << "\nStarting websocket server..." << endl;
        wsServer.start();
    }

    /*
    bool setupWsClient(Poco::Net::WebSocket& wsClient, string name, int wsPort) {
        Poco::Net::HTTPClientSession websocket(name, wsPort);    
        Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/?encoding=text", Poco::Net::HTTPMessage::HTTP_1_1);
        request.set("origin", "http://www.websocket.org");
        Poco::Net::HTTPResponse response;
        
        try {
            wsClient = new Poco::Net::WebSocket(websocket, request, response);
            cout << "\nWebsocket client connecting to ws://" + name << " on port " << wsPort << endl;
            return true;
        } catch (std::exception &e) {
            cout << "Exception " << e.what();
            return false;
        }
    }
    */
    
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