import processing.serial.*;
import processing.sound.*;

final int PORT_NUM = 0;
final color COLOR_BG = color(64, 94, 132);
final int ONEUP_THRESHOLD = 10;
final boolean DEBUG = false;

boolean hasContact = false;
Serial arduinoPort;
int pushupCount = 0;
int oneupCount = 0;

SoundFile soundCoin;
SoundFile soundOneup;

void setup() {
  textSize(86);
  fullScreen();

  soundCoin = new SoundFile(this, "coin.mp3");
  soundOneup = new SoundFile(this, "1-up.mp3");

  connectArduino();
}

void draw() {
  if (!hasContact && !DEBUG) return;

  background(COLOR_BG);
  
  text(pushupCount + " push-UP", 50, 100);
  if (oneupCount > 0) {
    text(oneupCount + " one-UP", 50, 200);
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
  pushupCount = Integer.parseInt(data);
  onDataReceived();
}

void onDataReceived() {  
  if (pushupCount % ONEUP_THRESHOLD == 0) {
    soundOneup.play();
    //pushupCount = 0;
    oneupCount++;
  }
  else {
    soundCoin.play();
  }
  
  println(oneupCount + " oneups, " + pushupCount + " pushups");
}

void connectArduino() {
  // https://learn.sparkfun.com/tutorials/connecting-arduino-to-processing/all

  if (DEBUG) return;

  //println(Serial.list()); // see the available ports and find the correct one
  String portName = Serial.list()[PORT_NUM];
  arduinoPort = new Serial(this, portName, 9600);
  arduinoPort.bufferUntil('\n');
}
