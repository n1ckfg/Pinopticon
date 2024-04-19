import peasy.PeasyCam;

PeasyCam cam;

float dotSize = 20;
ArrayList<Stroke> strokesBuffer;
boolean clicked = false;
boolean isDrawing = false;
int fps = 60;
int markTime = 0;
Frame frame;

PImage testImg;

void setup() {
  size(640, 480, P3D);
  frameRate(60);
  cam =  new PeasyCam(this, width/2, height/2, 400, 50);
  strokesBuffer = new ArrayList<Stroke>();
  frame = new Frame(strokesBuffer);
  oscSetup();
  fps = int((1.0/float(fps)) * 1000);
 
  testImg = createImage(256, 256, RGB);
  testImg.loadPixels();
}

void draw() {
  background(0);
  
  int time = millis();
  if (time > markTime + fps) {
    markTime = time;
    frame = new Frame(strokesBuffer);
  }
  
  frame.draw();
  
  if (frame.strokes.size() > 0) {
    Stroke stroke = frame.strokes.get(0);
    
    for (int x=0; x<testImg.width; x++) {
      for (int y=0; y<testImg.height; y++) {
        int loc = x + y * testImg.width;
        testImg.pixels[loc] = color(stroke.points.get(x).y);
      }
    }
    
    testImg.updatePixels();
    image(testImg, 0, 0);
  }  
        
  surface.setTitle("" + frameRate);
}

void createStroke(int index, ArrayList<PVector> points) {
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
