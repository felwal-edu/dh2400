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
  if (port.available() > 0) {
    String data = port.readStringUntil('\n').trim();

    // wait for connection
    if (!hasContact) {
      if (data.equals(".")) {
        port.clear();
        hasContact = true;
        port.write(".");
        println("contact");
      }
      else return;
    }

    // recieve data
    hex = data;
    println(hex);
  }
}

void connectArduino() {
    // https://learn.sparkfun.com/tutorials/connecting-arduino-to-processing/all

    String portName = Serial.list()[PORT_NUM];
    arduinoPort = new Serial(this, portName, 9600);
    arduinoPort.bufferUntil('\n');
}
