import oscP5.*;
import netP5.*;

PVector dot1 = new PVector(0,0);
PVector dot2 = new PVector(0,0);

String ipNumber = "127.0.0.1";
int sendPort = 9998;
int receivePort = 7110;
int datagramSize = 1000000;
OscP5 oscP5;
NetAddress myRemoteLocation;

void oscSetup() {
  OscProperties op = new OscProperties();
  op.setListeningPort(receivePort);
  op.setDatagramSize(datagramSize);
  oscP5 = new OscP5(this, op);
  myRemoteLocation = new NetAddress(ipNumber, sendPort);
}

// Receive message example
void oscEvent(OscMessage msg) {
  println(msg);
  if (msg.checkAddrPattern("/lidar") && msg.checkTypetag("ssibi")) {    
    int index = msg.get(2).intValue();
    byte[] readPointsBytes = msg.get(3).blobValue();
   
    ArrayList<PVector> points = new ArrayList<PVector>();
    for (int i = 0; i < readPointsBytes.length; i += 12) { //+=16) { 
      byte[] bytesX = { readPointsBytes[i], readPointsBytes[i+1], readPointsBytes[i+2], readPointsBytes[i+3] };
      byte[] bytesY = { readPointsBytes[i+4], readPointsBytes[i+5], readPointsBytes[i+6], readPointsBytes[i+7] };
      byte[] bytesZ = { readPointsBytes[i+8], readPointsBytes[i+9], readPointsBytes[i+10], readPointsBytes[i+11] };

      float x = asFloat(bytesX);
      float y = asFloat(bytesY);
      float z = asFloat(bytesZ);

      if (!Float.isNaN(x) && !Float.isNaN(y)) { // && !Float.isNaN(z)) {
        PVector p = new PVector(x, y, z);
        points.add(p);
      }
    }
    
    Stroke newStroke = new Stroke(index, points);

    boolean doReplace = false;
    int replaceIndex = 0;
    
    for (int i=0; i<strokesBuffer.size(); i++) {
      Stroke s = strokesBuffer.get(i);
      if (s.index == index) {
        replaceIndex = i;
        doReplace = true;
        break;
      }
    }
        
    if (doReplace) {
      strokesBuffer.set(replaceIndex, newStroke);
    } else {
      strokesBuffer.add(newStroke);
    }
  
    int time = millis();
    for (int i=0; i<strokesBuffer.size(); i++) {
      Stroke s = strokesBuffer.get(i);
      if (time > s.timestamp + s.lifespan) {
        strokesBuffer.remove(i);
      }
    }
  }
}
