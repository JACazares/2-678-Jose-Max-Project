#include <Servo.h> // load the servo library
Servo panServo;
Servo tiltServo;

void setup() {
  panServo.attach(2);
  tiltServo.attach(3);
  panServo.write(45);
  tiltServo.write(60);
}

void loop() {
  int left_right = analogRead(A0);
  int up_down = analogRead(A1);

  left_right = map(left_right, 0, 1023, 180, 0);
  up_down = map(up_down, 0, 1023, 0, 180);

  panServo.write(left_right);
  tiltServo.write(up_down);
  delay(20);
}
