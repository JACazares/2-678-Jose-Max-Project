
int sonarPin = 12;
int redLEDPin = 2;
int greenLEDPin = 4;
int blueLEDPin = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(sonarPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long tau = pulseIn(sonarPin, HIGH);
  
  unsigned long speedSound = 343;
  double distance = (tau * speedSound / 2.0) / 1000000.0;
  if(distance > 0.6)
  {
    digitalWrite(redLEDPin, LOW);
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(blueLEDPin, LOW);
  }
  else if(distance > 0.3)
  {
    digitalWrite(redLEDPin, LOW);
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(blueLEDPin, HIGH);
  }
  else
  {
    digitalWrite(redLEDPin, HIGH);
    digitalWrite(greenLEDPin, LOW);
    digitalWrite(blueLEDPin, LOW);
  }
  
  Serial.println(distance);
  delay(20);
}
