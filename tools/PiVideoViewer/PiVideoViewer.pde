PImage img;

void setup() {
  size(640, 480, P2D);
  img = createImage(640, 480, RGB);
  
  oscSetup();
}

void draw() {
  background(0);
  if (videoBytes != null) img = fromJpeg(videoBytes);
  image(img, 0, 0, width, height);
  
  surface.setTitle("" + frameRate);  
}
