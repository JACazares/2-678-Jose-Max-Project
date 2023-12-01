void setup() {
  // put your setup code here, to run once:
  pinMode(7, INPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
//  int periodMiliseconds = 10;
//  int delayMiliseconds = periodMiliseconds / 2;
//  
//  digitalWrite(9, HIGH);
//  delay(delayMiliseconds);
//  digitalWrite(9, LOW);
//  delay(delayMiliseconds);

  int limit = 5;
  if(digitalRead(7) == HIGH)
    limit = 17;
    
  digitalWrite(9, HIGH);
  int i;
  for(i = 0; i < 20; i++)
  {
    delayMicroseconds(100);
    if(i > limit)
      digitalWrite(9, LOW);
  }
}
