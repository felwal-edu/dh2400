const int PIN_IN_IR = 5;
const int PIN_OUT_MOTOR = 6;

const int ACORN_THRESHOLD = 3;
const int DEBUG = false;

int acornCounter = 0;
int pickupState = false;

void setup() {
  Serial.begin(9600);

  pinMode(PIN_IN_IR, INPUT);
  pinMode(PIN_OUT_MOTOR, OUTPUT);

  connectProcessing();
}

void loop() {
  int isPickingUp = digitalRead(PIN_IN_IR);

  // if same old state, do nothing
  if (pickupState == isPickingUp) {
    delay(200);
    return;
  }

  if (!isPickingUp) {
    acornCounter++;

    if (acornCounter >= ACORN_THRESHOLD) {
      openFloor();
    }

    sendToProcessing(String(acornCounter));
    d("counter: " + String(acornCounter));
    delay(2000);
  }

  pickupState = isPickingUp;
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

void openFloor() {
  // TODO motor
  digitalWrite(PIN_OUT_MOTOR, HIGH);
}

void d(String s) {
  if (DEBUG) {
    Serial.println(s);
  }
}
