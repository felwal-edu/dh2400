#include <math.h>

const int PIN_IN_TOTEM_R = A0;
const int PIN_IN_TOTEM_G = A1;
const int PIN_IN_TOTEM_B = A2;

const int MAX_HEIGHT = 3;
const int TOTEM_COUNT = MAX_HEIGHT * 3;
const int TOTEM_VALUES[] = { 512, 340, 256, 205 }; // TODO update

const int DEBUG = false;

void setup() {
  Serial.begin(9600);

  pinMode(PIN_IN_TOTEM_R, INPUT);
  pinMode(PIN_IN_TOTEM_G, INPUT);
  pinMode(PIN_IN_TOTEM_B, INPUT);

  connectProcessing();
}

void loop() {
  int totemsR = totemRead(PIN_IN_TOTEM_R);
  int totemsG = 0;//totemRead(PIN_IN_TOTEM_G);
  int totemsB = 0;//totemRead(PIN_IN_TOTEM_B);

  d("totemsR: " + String(totemsR));
  //d("totemsG: " + String(totemsG));
  //d("totemsB: " + String(totemsB));

  int r = toRgbVal(totemsR);
  int g = toRgbVal(totemsG);
  int b = toRgbVal(totemsB);

  d("r: " + String(r) + " -> " + hexify(r));

  String hex = hexify(r) + hexify(g) + hexify(b);
  // send to Processing
  Serial.println(hex);

  d("");
  delay(500);
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

/**
 * Returns number of totems corresponding to the TOTEM_VALUES.
 */
int totemRead(int pin) {

  int totemVal = 256;//analogRead(pin);
  d("totemVal: " + String(totemVal));

  for (int i = 0; i < MAX_HEIGHT; i++) {
    int limit = TOTEM_VALUES[i] - (TOTEM_VALUES[i] - TOTEM_VALUES[i + 1]) / 2;

    if (totemVal > limit) {
      d("limit: " + String(limit));
      return i;
    }
  }

  return MAX_HEIGHT;
}

int toRgbVal(int totems) {
  float valPerTotem = 256.0 / MAX_HEIGHT;
  return (int) (totems * valPerTotem - (totems == MAX_HEIGHT ? 1 : 0));
}

String hexify(int dec) {
  // make sure it is of length 2
  String hex = String(dec, HEX);
  return hex.length() == 2 ? hex : "0" + hex;
}

void d(String s) {
  if (DEBUG) {
    Serial.println(s);
  }
}
