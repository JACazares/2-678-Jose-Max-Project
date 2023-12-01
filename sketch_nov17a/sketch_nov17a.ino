float duty = 0.677;
int v_set = (3.0/5.0) * 1023;
int v_meas;
int pwm = 100;

void setup() {
  // put your setup code here, to run once:
  analogWrite(5, pwm);
  TCCR0B &= ~2;
}

void loop() {
  // put your main code here, to run repeatedly:
  v_meas = 0;
  for(int i = 0; i < 20; i++)
    v_meas = v_meas + 0.05 * analogRead(A0);

  if(v_meas > v_set)  pwm--;
  if(v_meas < v_set)  pwm++;

  if(pwm > 215) pwm = 215;
  if(pwm < 0) pwm = 0;

  analogWrite(5, pwm);
  delay(40);
}
