const int PIN_IN_TOTEM_R = 0;
const int PIN_IN_TOTEM_G = 1;
const int PIN_IN_TOTEM_B = 2;

const int TOTEM_COUNT = 9;

const int VOLT_IN = 5;
const int RES_FIXED = 1;
const int RES_TOTEM = 2; // should be 2x FIXED

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

  // -> 0, 5, A, F
  int r = toHexLetter(totemsR);
  int g = toHexLetter(totemsG);
  int b = toHexLetter(totemsB);

  String hex = "#" + (r + r) + (g + g) + (b + b);

  // send to Processing
  //Serial.println(hex)
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

  for (int totems = 0; totems <= TOTEM_COUNT; totems++) {
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

  return -1;
}

String toHexLetter(int totemCount) {
  // TODO: dynamic totem count / mapping

  switch (totemCount) {
    case 0: return "0";
    case 1: return "5";
    case 2: return "A";
    case 3: return "F";
    default: return "F"; // TODO: some odd color
  }
}
