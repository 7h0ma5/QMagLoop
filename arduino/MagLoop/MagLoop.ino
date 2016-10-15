#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

#define RPM 20

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_StepperMotor* motor = AFMS.getStepper(200, 2);

enum State {
  READY,
  ACTIVE
};

struct Status {
  State state;
  int target;
};

Status status = { .state = READY, .target = 0 };

void setup() {
  Serial.begin(115200);
  AFMS.begin();

  motor->setSpeed(1);
  motor->release();

  delay(1000);

  for (int i = 0; i < 100; i++) {
    Serial.println("R;");
    Serial.println("T;0;");
  }
}

void read_command() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil(';');

    if (cmd == "S") { // STOP
      status.target = 0;
      status.state = READY;
      Serial.println("T;0;");
      Serial.println("R;");
    }
    else if (cmd == "G") { // GOTO
      String target = Serial.readStringUntil(';');
      status.target = target.toInt();
      status.state = ACTIVE;
      Serial.println("A;");
      Serial.print("T;");
      Serial.print(status.target);
      Serial.println(";");
    }
  }
}

void loop() {
  read_command();

  switch (status.state) {
    case ACTIVE:
      if (status.target < 0) {
        motor->onestep(BACKWARD, DOUBLE);
        delay(60000/(RPM*200));
        status.target += 1;
        Serial.println("S;-1;");
        Serial.print("T;");
        Serial.print(status.target);
        Serial.println(";");
      }
      else if (status.target > 0) {
        motor->onestep(FORWARD, DOUBLE);
        delay(60000/(RPM*200));
        status.target -= 1;
        Serial.println("S;1;");
        Serial.print("T;");
        Serial.print(status.target);
        Serial.println(";");
      }
      else {
        motor->release();
        status.state = READY;
        Serial.println("T;0;");
        Serial.println("R;"); 
      }

      break;

    case READY: break;
  }
}
