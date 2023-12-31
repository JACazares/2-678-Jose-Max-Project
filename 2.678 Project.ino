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
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);

  digitalWrite(4, LOW);
  digitalWrite(3, LOW);
  digitalWrite(2, LOW);
  
  digitalWrite(STDBY, HIGH);
  Serial.begin(9600);
}

int baseMotorSpeed = 125;
int tresholdDifference = 60;
int offLineDelay = 300;
double turnConstant = 0.25;
int sensorLeft, sensorMiddle, sensorRight;

int offLineCounter = 0;
bool offLine = false, turning = false, startLoop = false, onNarrow = false;
static unsigned long offLineTimer = 0, turningTimer = 0;
unsigned long loopTimer = 0;

void turn(int& motorSpeedLeft, int& motorSpeedRight)
{
  if(offLineCounter > 7)
  {
    motorSpeedLeft = 0;
    motorSpeedRight = 0;
    return;
  }
  else if(offLine && offLineCounter < 3)
  {
    motorSpeedLeft = 255;
    motorSpeedRight = 255;
    return;
  }
  else if (offLine && offLineCounter == 3)
  {
    motorSpeedLeft = -255;
    motorSpeedRight = 255;
    return;
  }
  else if (offLine && offLineCounter == 4)
  {
    motorSpeedLeft = 255;
    motorSpeedRight = -255;

    // Now shorten off line delay to ensure we go around the loop
    offLineDelay = 75;
    return;
  }
  else if (offLine && offLineCounter == 5)
  {
    motorSpeedLeft = 255;
    motorSpeedRight = -255;
    startLoop = true;
    loopTimer = millis();

    offLineDelay = 200;
    return;
  }
  else if (offLine && offLineCounter == 6)
  {
    motorSpeedLeft = 255;
    motorSpeedRight = -255;
    return;
  }

  if (startLoop)
  {
    if (millis() - loopTimer < 300)
    {
      motorSpeedLeft = 255;
      motorSpeedRight = -255;
      return;
    }
    else
    {
      startLoop = false;
    }
  }

  int difference = sensorRight - sensorLeft;

  if(abs(difference) < tresholdDifference && !offLine)
  {
    motorSpeedLeft = baseMotorSpeed;
    motorSpeedRight = baseMotorSpeed;

    turningTimer = 0;
    turning = false;
    turnConstant = 0.25;
  }
  else
  {
    if(turningTimer == 0)
      turningTimer = millis();
    else if(millis() - turningTimer > 100 && !turning)
    {
      turning = true;
    }

    if (turning) 
    {
      turnConstant = 0.25 + (double)(millis() - turningTimer)/1000.0;
    }

    if(sensorMiddle < 700 && (sensorLeft > 500 || sensorRight > 500))
    {
      if(difference > 0)
        difference = difference + 2*(800 - sensorMiddle);
      else
        difference = difference - 2*(800 - sensorMiddle);
    }

    motorSpeedLeft = baseMotorSpeed + turnConstant*(difference);
    motorSpeedRight = baseMotorSpeed - turnConstant*(difference);
    //  motorSpeedLeft = baseMotorSpeed * (1.0 + difference/600.0);
    //  motorSpeedRight = baseMotorSpeed / (1.0 + difference/600.0);
  }
}

// This function checks if the middle sensor is less than 700 and, if it is
// starts a timer, if the timer is greater than 300ms, it adds one to a counter

void checkOffLine()
{
  int sensorOffTreshold = 400;
  if(sensorMiddle < sensorOffTreshold && sensorLeft < sensorOffTreshold && sensorRight < sensorOffTreshold && !turning)
  {
    if(offLineTimer == 0)
      offLineTimer = millis();
    else if(millis() - offLineTimer > offLineDelay && !offLine)
    {
      offLineCounter++;
      offLine = true;
    }
  }
  else
  {
    offLineTimer = 0;
    offLine = false;
  }
}


void loop() {
  sensorRight = analogRead(A5);
  sensorMiddle = analogRead(A6);
  sensorLeft = analogRead(A7);

  int motorSpeedLeft = baseMotorSpeed, motorSpeedRight = baseMotorSpeed;

  turn(motorSpeedLeft, motorSpeedRight);
  checkOffLine();

  motorWrite(motorSpeedLeft, BIN1, BIN2, PWMB);
  motorWrite(motorSpeedRight, AIN1, AIN2, PWMA);

//  Serial.print(motorSpeedLeft);
//  Serial.print(" ");
//  Serial.println(motorSpeedRight);
  Serial.println(offLineCounter);

  if (offLineCounter % 3 == 1)
  {
    digitalWrite(4, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
  }
  else if (offLineCounter % 3 == 2)
  {
    digitalWrite(4, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(2, LOW);
  }
  else if (offLineCounter % 3 == 0)
  {
    digitalWrite(4, LOW);
    digitalWrite(3, LOW);
    digitalWrite(2, HIGH);
  }

  // int sensorDifference = -(100.0/(double)sensorRight - 100.0/(double)sensorLeft)*1000.0;
  // >> 0, right > left -> turn right
  // << 0, right < left -> turn left

  Serial.print(sensorLeft);
  Serial.print(' ');
  Serial.print(sensorMiddle);
  Serial.print(' ');
  Serial.println(sensorRight);

  // if(leftSpeeding && rightSpeeding)
  // {
    // leftSpeeding = false;
    // rightSpeeding = false;
  // }


  // if(!leftSpeeding)
  // {
  //   // Turn Left
  //   if(sensorDifference < tresholdSensorValue)
  //   {
  //     double error = 900 - sensorRight;
  //     motorSpeedRight = baseMotorSpeed * (1.0 + error/100.0);
  //     motorSpeedLeft = baseMotorSpeed / (1.0 + error/100.0);
  //     rightSpeeding = true;
  //   }
  //   else
  //   {
  //     motorSpeedRight = baseMotorSpeed;
  //     motorSpeedLeft = baseMotorSpeed;
  //     rightSpeeding = false;
  //   }
  // }
}

//----------------------------------------------
void motorWrite(int motorSpeed, int xIN1, int xIN2, int PWMx)
{
 
  if(motorSpeed > 0)          // it's forward
  {
    digitalWrite(xIN1, LOW);
    digitalWrite(xIN2, HIGH);
  }
  else                         // it's reverse
  {
    digitalWrite(xIN1, HIGH);
    digitalWrite(xIN2, LOW);
  } 

  motorSpeed = abs(motorSpeed);
  motorSpeed = constrain(motorSpeed, 0, 255);   // Just in case...
  analogWrite(PWMx, motorSpeed);
}
