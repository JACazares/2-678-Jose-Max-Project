const int AIN1  = 9;
const int AIN2  = 10;
const int PWMA  = 11;
const int BIN1  = 7;
const int BIN2  = 6;
const int PWMB  = 5;
const int STDBY = 8;

bool leftSpeeding = false, rightSpeeding = false;

void setup() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(STDBY, OUTPUT);
   // whatever else you need to do...
  
  digitalWrite(STDBY, HIGH);
  Serial.begin(9600);
}

void loop() {
  int sensorRight = analogRead(A5);
  int sensorMiddle = analogRead(A6);
  int sensorLeft = analogRead(A7);

  int tresholdSensorValue = 900;
  int baseMotorSpeed = 100;
  int motorSpeedLeft = baseMotorSpeed, motorSpeedRight = baseMotorSpeed;

  Serial.print(sensorLeft);
  Serial.print(' ');
  Serial.print(sensorMiddle);
  Serial.print(' ');
  Serial.println(sensorRight);

  if(leftSpeeding && rightSpeeding)
  {
    leftSpeeding = false;
    rightSpeeding = false;
  }

  if(!rightSpeeding)
  {
    if(sensorLeft < tresholdSensorValue)
    {
      //speed up left or slow down right
      double error = tresholdSensorValue - sensorLeft;
      motorSpeedLeft = baseMotorSpeed * (1.0 + error/100.0);
      motorSpeedRight = baseMotorSpeed / (1.0 + error/100.0);
      leftSpeeding = true;
    }
    else
    {
      motorSpeedLeft = baseMotorSpeed;
      motorSpeedRight = baseMotorSpeed;
      leftSpeeding = false;
    }
  }

  if(!leftSpeeding)
  {
    if(sensorRight < tresholdSensorValue)
    {
      double error = tresholdSensorValue - sensorRight;
      motorSpeedRight = baseMotorSpeed * (1.0 + error/100.0);
      motorSpeedLeft = baseMotorSpeed / (1.0 + error/100.0);
      rightSpeeding = true;
    }
    else
    {
      motorSpeedRight = baseMotorSpeed;
      motorSpeedLeft = baseMotorSpeed;
      rightSpeeding = false;
    }
  }

  motorWrite(motorSpeedLeft, BIN1, BIN2, PWMB);
  motorWrite(motorSpeedRight, AIN1, AIN2, PWMA);

  // delay(20);
}

//----------------------------------------------
void motorWrite(int motorSpeed, int xIN1, int xIN2, int PWMx)
{
 
  if (motorSpeed > 0)          // it's forward
  {  digitalWrite(xIN1, LOW);
     digitalWrite(xIN2, HIGH);
  }
  else                         // it's reverse
  {  digitalWrite(xIN1, HIGH);
     digitalWrite(xIN2, LOW);
  } 

    motorSpeed = abs(motorSpeed);
    motorSpeed = constrain(motorSpeed, 0, 255);   // Just in case...
    analogWrite(PWMx, motorSpeed);
}

