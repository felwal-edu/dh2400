const int PIN_IN_PUSHSUP = A0;
const int PUSH_UP_THRESHOLD = 22; // TODO calibrate
const int DEBUG = false;

int pushupState = false;
int pushupCounter = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_IN_PUSHSUP, INPUT);
  connectProcessing();
}

void loop() {
  bool pushedUp = pushupRead(PIN_IN_PUSHSUP);

  // if same old state, do nothing
  if (pushupState == pushedUp) {
    delay(200);
    return;
  }

  if (pushedUp) {
    pushupCounter++;

    sendToProcessing(String(pushupCounter));
    d("counter: " + String(pushupCounter));
    delay(1000);
  }

  pushupState = pushedUp;
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

bool pushupRead(int pin) {
  int inVal = analogRead(pin);
  bool pushedUp = inVal <= PUSH_UP_THRESHOLD;
  d(String(inVal));

  return pushedUp;
}

void sendToProcessing(String data) {
  if (!DEBUG) {
    Serial.println(data);
  }
}

void d(String s) {
  if (DEBUG) {
    Serial.println(s);
  }
}
