import processing.serial.*;
import processing.sound.*;

final int PORT_NUM = 0;
final color COLOR_BG = color(132, 132, 64);
final int ACORN_TRHESHOLD = 3;
final boolean DEBUG = false;

boolean hasContact = false;
Serial arduinoPort;

SoundFile soundAcorn;
SoundFile soundGameOver;

int acornCount = 0;
boolean isGameOver = false;

void setup() {
  fullScreen();

  soundAcorn = new SoundFile(this, "../../PushUp/processing/coin.mp3");
  soundGameOver = new SoundFile(this, "minecraft-rabbit-death.mp3");

  connectArduino();
}

void draw() {
  if (!hasContact) return;

  background(COLOR_BG);
  drawCounter();
  drawGameOver();
}

void drawCounter() {
  textSize(86);
  textAlign(LEFT);
  text(acornCount + " acorns", 50, 100);
}

void drawGameOver() {
  if (isGameOver) {
    textSize(128);
    textAlign(CENTER);
    text("You win!", displayWidth / 2, displayHeight / 2);
  }
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
  println(acornCount + " acorns");

  if (acornCount >= ACORN_TRHESHOLD) {
    isGameOver = true;
    delay(900);
    soundGameOver.play();
  }
}

void connectArduino() {
  // https://learn.sparkfun.com/tutorials/connecting-arduino-to-processing/all

  if (DEBUG) return;

  //println(Serial.list()); // see the available ports and find the correct one
  String portName = Serial.list()[PORT_NUM];
  arduinoPort = new Serial(this, portName, 9600);
  arduinoPort.bufferUntil('\n');
}
