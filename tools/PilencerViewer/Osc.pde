import oscP5.*;
import netP5.*;

String ipNumber = "127.0.0.1";
int sendPort = 9998;
int receivePort = 7110;
OscP5 oscP5;
NetAddress myRemoteLocation;

void oscSetup() {
  oscP5 = new OscP5(this, receivePort);
  myRemoteLocation = new NetAddress(ipNumber, sendPort);
}

// Receive message example
void oscEvent(OscMessage msg) {
  if (msg.checkAddrPattern("/pilencer") && msg.checkTypetag("sif")) {
    
    //String hostname = msg.get(0).stringValue();
    trigger = boolean(msg.get(1).intValue());
    
    println(trigger);
  }
}
