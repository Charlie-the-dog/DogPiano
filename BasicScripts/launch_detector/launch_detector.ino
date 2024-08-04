#include <Stepper.h>
#include <avr/interrupt.h>

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
// for your motor


const unsigned int FEED_STEPPED_BLUE_PIN   = 10;
const unsigned int FEED_STEPPED_YELLOW_PIN = 11;
const unsigned int FEED_STEPPED_BROWN_PIN  = 12;
const unsigned int FEED_STEPPED_BLACK_PIN  = 13;

const unsigned int LAUNCH_DETECT_PIN = 1;

Stepper myStepper(stepsPerRevolution, FEED_STEPPED_BLUE_PIN, FEED_STEPPED_YELLOW_PIN, FEED_STEPPED_BROWN_PIN, FEED_STEPPED_BLACK_PIN);

const unsigned int SHOOTER_PIN = 9;

volatile bool shooter_on = false;

void turn_shooter_off() {
  digitalWrite(SHOOTER_PIN,LOW);
  shooter_on = false;
}

void turn_shooter_on() {
  digitalWrite(SHOOTER_PIN,HIGH);
  delay(100); // 40 msec delay to wait for voltage/current lim transient
  shooter_on = true;
}

void setup_shooter() {
  Serial.println("Setting up shooter...\n");
  pinMode(SHOOTER_PIN, OUTPUT);
  turn_shooter_off();
}

void setup_feeder() {
  myStepper.setSpeed(10);
}


volatile unsigned int launch_count = 0;
volatile unsigned long last_launch_time_ms = 0;

void setup_launch_detector() {
  pinMode(LAUNCH_DETECT_PIN,INPUT);
  launch_count = 0;
  
  attachInterrupt(digitalPinToInterrupt(LAUNCH_DETECT_PIN), launch_detect_interrupt, RISING);
}


void launch_detect_interrupt() {
  if(shooter_on){
    const unsigned long cur_time = millis();

    if((cur_time - last_launch_time_ms) > 40) {
      launch_count++;
      last_launch_time_ms = cur_time;
    }
    
  }
}

void setup() {
  Serial.begin(9600);
  setup_shooter();
  setup_feeder();
  setup_launch_detector();
  delay(1000);
  Serial.println("Setup Complete!");
}

const unsigned int DELAY = 500;

void loop() {
  turn_shooter_on();
  myStepper.step(-stepsPerRevolution);

  turn_shooter_off();

  delay(3000);
  Serial.println("Done cycle...");
  if(launch_count > 0)
  {
    Serial.print("Launch detected! ");
    Serial.print("Count: ");
    Serial.println(launch_count);
    launch_count = 0;
  }
}
