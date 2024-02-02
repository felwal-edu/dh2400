const int PIN_IN_TOTEM_R = A0;
const int PIN_IN_TOTEM_G = A1;
const int PIN_IN_TOTEM_B = A2;

const int TOTEM_MAX_HEIGHT = 3;
const int TOTEM_VALUES[] = { 1023, 540, 480, 400 };

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
  int totemsG = totemRead(PIN_IN_TOTEM_G);
  int totemsB = totemRead(PIN_IN_TOTEM_B);

  //d("totemsR: " + String(totemsR));
  //d("totemsG: " + String(totemsG));
  //d("totemsB: " + String(totemsB));

  int r = toRgbVal(totemsR);
  int g = toRgbVal(totemsG);
  int b = toRgbVal(totemsB);

  //d("r: " + String(g) + " -> " + hexify(g));

  String hex = hexify(r) + hexify(g) + hexify(b);
  sendToProcessing(hex);

  d("");
  delay(200);
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
  if (DEBUG) {
    return;
  }

  Serial.println(data);
}

/**
 * Returns number of totems corresponding to the TOTEM_VALUES.
 */
int totemRead(int pin) {
  int totemVal = analogRead(pin);
  d("totemVal: " + String(totemVal));

  for (int i = 0; i < TOTEM_MAX_HEIGHT; i++) {
    int limit = TOTEM_VALUES[i] - (TOTEM_VALUES[i] - TOTEM_VALUES[i + 1]) / 2;

    if (totemVal > limit) {
      //d("limit: " + String(limit));
      return i;
    }
  }

  return TOTEM_MAX_HEIGHT;
}

int toRgbVal(int totems) {
  float valPerTotem = 256.0 / TOTEM_MAX_HEIGHT;
  return (int) (totems * valPerTotem - (totems == TOTEM_MAX_HEIGHT ? 1 : 0));
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
