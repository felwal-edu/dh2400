const int PIN_IN_TOTEM_R = 0;
const int PIN_IN_TOTEM_G = 1;
const int PIN_IN_TOTEM_B = 2;

const int MAX_HEIGHT = 3;
const int TOTEM_COUNT = MAX_HEIGHT * 3;

const int VOLT_IN = 5;
const int RES_FIXED = 1;
const int RES_TOTEM = 2; // should be 2x FIXED for widest span

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

  int r = toRgbVal(totemsR);
  int g = toRgbVal(totemsG);
  int b = toRgbVal(totemsB);

  String hex = hexify(r) + hexify(g) + hexify(b);
  Serial.println(hex);
}

void connectProcessing() {
  // https://learn.sparkfun.com/tutorials/connecting-arduino-to-processing/all

  while (Serial.available() <= 0) {
    Serial.println(".");
    delay(300);
  }
}

int totemRead(int pin) {
  // returns number of totems

  // 0 -> 682
  // 1 -> 511.5
  // 2 -> 409.2
  // 3 -> 341

  int inVal = analogRead(pin);
  int inVals[TOTEM_COUNT] = {};

  //Serial.println("inVal: " + inVal);

  for (int totems = 0; totems <= MAX_HEIGHT; totems++) {
    int resistors = totems + 1; // one in the base
    int resTot = RES_TOTEM / resistors;
    int voltOut = VOLT_IN * (resTot / (resTot + RES_FIXED));
    int inVal_ = 1023 / VOLT_IN * voltOut;
    inVals[totems] = inVal_;

    // since no previous inval, just check if larger
    if (totems == 0 && inVal > inVal_) {
      return totems;
    }

    int diffToPrev = inVals[totems - 1] - inVal_;

    // use +- half the diff as boundary
    if (inVal > inVal_ + diffToPrev / 2) {
      return totems - 1;
    }
  }

  return MAX_HEIGHT;
}

int toRgbVal(int totems) {
  // +1 for empty pole
  int valPerTotem = 256 / (MAX_HEIGHT + 1);

  return totems * valPerTotem - totems == 0 ? 0 : 1;
}

String hexify(int dec) {
  // make sure it is of length 2
  String hex = String(dec, HEX);
  return hex.length() == 2 ? hex : "0" + hex;
}
