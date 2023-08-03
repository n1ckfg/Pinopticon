boolean trigger = false;

void setup() {
  size(640, 480, P3D);
  oscSetup();
}

void draw() {
  if (trigger) {
    background(0,255,0);
  } else {
    background(255,0,0);
  }
}
