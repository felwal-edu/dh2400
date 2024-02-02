import processing.serial.*;

final int PORT_NUM = 0;
final int DEBUG = false;

boolean hasContact = false;
Serial arduinoPort;
String hex = "000000";

void setup() {
  fullScreen();
  connectArduino();
}

void draw() {
  if (!hasContact && !DEBUG) return;

  int dec = unhex(hex);

  // avoid unhex(0000XX) resulting in greyscale when passed to background()
  if (hex.substring(0, 4).equals("0000")) {
    background(0, 0, dec);
  }
  else {
    background(dec);
  }
}

void serialEvent(Serial port) {
  if (port.available() <= 0) return;

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
  hex = data;
  println(hex);
}

void connectArduino() {
  // https://learn.sparkfun.com/tutorials/connecting-arduino-to-processing/all

  if (DEBUG) return;

  //println(Serial.list()); // see the available ports and find the correct one
  String portName = Serial.list()[PORT_NUM];
  arduinoPort = new Serial(this, portName, 9600);
  arduinoPort.bufferUntil('\n');
}
