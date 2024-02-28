import processing.serial.*;
import processing.sound.*;

final int PORT_NUM = 0;
final color COLOR_BG = color(155, 155, 155);
final int ACORN_TRHESHOLD = 3;
final boolean DEBUG = false;

boolean hasContact = false;
Serial arduinoPort;

SoundFile soundAcorn;
SoundFile soundGameOver;

int acornCount = 0;

void setup() {
  textSize(86);
  fullScreen();

  soundAcorn = new SoundFile(this, "../../PushUp/processing/coin.mp3"); // TODO
  soundGameOver = new SoundFile(this, "../../PushUp/processing/1-up.mp3"); // TODO

  connectArduino();
}

void draw() {
  if (!hasContact && !DEBUG) return;

  background(COLOR_BG);

  text(acornCount + " push-UP", 50, 100);
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
  acornCount = Integer.parseInt(data);
  onDataReceived();
}

void onDataReceived() {
  soundAcorn.play();

  if (acornCount >= ACORN_TRHESHOLD) {
    soundGameOver.play();
  }

  println(acornCount + " acorns");
}

void connectArduino() {
  // https://learn.sparkfun.com/tutorials/connecting-arduino-to-processing/all

  if (DEBUG) return;

  //println(Serial.list()); // see the available ports and find the correct one
  String portName = Serial.list()[PORT_NUM];
  arduinoPort = new Serial(this, portName, 9600);
  arduinoPort.bufferUntil('\n');
}
