int IN_A = 10;
int IN_B = 8;

void setup() {
  // put your setup code here, to run once:
  pinMode(IN_A, OUTPUT);
  pinMode(IN_B, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int delays[] =  {1000, 1000, 1000, 750, 250, 1000, 750, 250, 2000, 1000,
                   1000, 1000, 750, 250, 1000, 750, 250, 2000, 1000, 750,
                   250, 1000, 750, 250, 250, 250, 500, 500, 500, 1000,
                   750, 250, 250, 250, 500, 500, 500, 1000, 750, 250,
                   250, 250, 500, 500, 500, 1000, 750, 250, 1000, 750,
                   250, 2000};
                   
  int E = 125, G = 150, B = 175, D = 200, F = 137;
  int pitches[] = {G, G, G, E, B, G, E, B, G, D,
                   D, D, E, B, F, E, B, G, 250, G,
                   G, 250, 225, 225, 212, 212, 212};
  int N = 52;

  unsigned int i = 0;
  int curr_dir = 0;
  for(i = 0; i < N; i++)
  {
    drive(200, curr_dir);
    delay(delays[i]/2);
    curr_dir = 1 - curr_dir;
  }
  drive(0, LOW);
  delay(2000);
}

void drive(int motor_speed, int motor_direction)
{
  if(motor_direction == HIGH)
    motor_speed = 255 - motor_speed;

  analogWrite(IN_A, motor_speed);
  digitalWrite(IN_B, motor_direction);
}
