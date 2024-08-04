#include <Stepper.h>

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
// for your motor


const unsigned int FEED_STEPPED_BLUE_PIN   = 10;
const unsigned int FEED_STEPPED_YELLOW_PIN = 11;
const unsigned int FEED_STEPPED_BROWN_PIN  = 12;
const unsigned int FEED_STEPPED_BLACK_PIN  = 13;


Stepper myStepper(stepsPerRevolution, FEED_STEPPED_BLUE_PIN, FEED_STEPPED_YELLOW_PIN, FEED_STEPPED_BROWN_PIN, FEED_STEPPED_BLACK_PIN);

const unsigned int SHOOTER_PIN = 9;


void turn_shooter_off() {
  digitalWrite(SHOOTER_PIN,LOW);
}

void turn_shooter_on() {
  digitalWrite(SHOOTER_PIN,HIGH);
}

void setup_shooter() {
  Serial.println("Setting up shooter...\n");
  pinMode(SHOOTER_PIN, OUTPUT);
  turn_shooter_off();
}

void setup_feeder() {
  myStepper.setSpeed(10);
}


void setup() {
  Serial.begin(9600);
  setup_shooter();
  setup_feeder();
  Serial.println("Setup Complete!");
}

const unsigned int DELAY = 500;

void loop() {
  turn_shooter_on();
  myStepper.step(-stepsPerRevolution);

  turn_shooter_off();

  delay(3000);
}
