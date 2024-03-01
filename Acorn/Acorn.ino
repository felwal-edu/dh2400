#include <Servo.h>;

const int PIN_IN_UV = 2; // echo
const int PIN_OUT_UV = 3; // trig
const int PIN_OUT_SERVO = 9;

const float UV_DIST_THRESHOLD = 8;
const int ACORN_THRESHOLD = 3;
const int DEBUG = true;

Servo servo;

int acornCounter = 0;
int acornFallingState = false;

void setup() {
  Serial.begin(9600);

  pinMode(PIN_IN_UV, INPUT);
  pinMode(PIN_OUT_UV, OUTPUT);
  servo.attach(PIN_OUT_SERVO);

  //servo.write(180);
  closeFloor();

  connectProcessing();
}

void loop() {
  //return;

  writeUv();
  int distance = readUvDistance();
  //d("distance: " + String(distance));
  Serial.println(distance);

  int isAcornFalling = distance <= UV_DIST_THRESHOLD;

  if (isAcornFalling) {
    d("|");
  }
  else {
    //d("-");
  }

  //return;

  // if same old state, do nothing
  if (acornFallingState == isAcornFalling) {
    delay(10);
    return;
  }

  if (!isAcornFalling) {
    acornCounter++;

    if (acornCounter >= ACORN_THRESHOLD) {
      openFloor();
    }

    sendToProcessing(String(acornCounter));
    d("acorns: " + String(acornCounter));

    delay(1000);
  }

  acornFallingState = isAcornFalling;
}

void writeUv() {
  digitalWrite(PIN_OUT_UV, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_OUT_UV, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_OUT_UV, LOW);
}

float readUvDistance() {
  int duration = pulseIn(PIN_IN_UV, HIGH);
  float distance = (duration * 0.0343) / 2;

  //d("distance: " + String(distance));

  return distance;
}

void connectProcessing() {
  // https://learn.sparkfun.com/tutorials/connecting-arduino-to-processing/all
  // Port busy? Close the Serial Monitor.

  if (DEBUG) {
    return;
  }

  while (Serial.available() <= 0) {
    Serial.println(".");
    delay(300);
  }
}

void sendToProcessing(String data) {
  if (!DEBUG) {
    Serial.println(data);
  }
}

void closeFloor() {
  d("floor: closed");
  servo.write(0);
}

void openFloor() {
  d("floor: open");
  servo.write(90);
  delay(1000);

  //closeFloor();
}

void d(String s) {
  if (DEBUG) {
    Serial.println(s);
  }
}
