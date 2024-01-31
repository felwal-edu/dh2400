import processing.serial.*;

final int PORT_NUM = 0;

boolean hasContact = false;
Serial arduinoPort;
String hex = "000000";

void setup() {
  connectArduino();
}

void draw() {
  if (!hasContact) return;

  background(unhex(hex));
}

void serialEvent(Serial port) {
  if (port.available() <= 0) return;

  String data = port.readStringUntil('\n').trim();

  // wait for connection
  if (!hasContact) {
    if (data.equals(".")) {
      println(".");
      port.clear();
      port.write("!");
    }
    else {
      hasContact = true;
      println("contact");
    }
    return;
  }

  // recieve data
  hex = data;
  println(hex);
}

void connectArduino() {
  // https://learn.sparkfun.com/tutorials/connecting-arduino-to-processing/all

  //println(Serial.list()); // find the correct port
  String portName = Serial.list()[PORT_NUM];
  arduinoPort = new Serial(this, portName, 9600);
  arduinoPort.bufferUntil('\n');
}
