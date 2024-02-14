import processing.serial.*;

final int PORT_NUM = 0;
final color COLOR_BG = color(64, 94, 132);
final boolean DEBUG = false;

boolean hasContact = false;
Serial arduinoPort;
int pushupCount = 0;

void setup() {
  textSize(128);
  fullScreen();
  connectArduino();
}

void draw() {
  if (!hasContact && !DEBUG) return;

  background(COLOR_BG);
  text(pushupCount + " push-ups!", displayWidth / 3, displayHeight / 2);
}

void serialEvent(Serial port) {
  if (port.available() <= 0) {
    println("port unavailable");
    return;
  }

  String data = port.readStringUntil('\n').trim();

  // wait for connection
  if (data.equals(".")) {
    port.clear();
    port.write(".");
    println("contact");
    hasContact = true;
    return;
  }

  // recieve data
  pushupCount = Integer.parseInt(data);
  println(pushupCount);
}

void connectArduino() {
  // https://learn.sparkfun.com/tutorials/connecting-arduino-to-processing/all

  if (DEBUG) return;

  //println(Serial.list()); // see the available ports and find the correct one
  String portName = Serial.list()[PORT_NUM];
  arduinoPort = new Serial(this, portName, 9600);
  arduinoPort.bufferUntil('\n');
}
