
static const float _voltsPrBitDivided  = 0.0064453f;

void setup(){
  analogReference(EXTERNAL);
 Serial.begin(19200);
}

void loop(){
  int verdi,x,y,z;
  
  float spenning;
  
  verdi = analogRead(4);
  x = analogRead(0);
  y = analogRead(1);
  z = analogRead(2);
  
  spenning = verdi *  _voltsPrBitDivided;
  Serial.print(spenning);
  Serial.print(' ');
  Serial.print(x);
  Serial.print(' ');
  Serial.print(y);
  Serial.print(' ');
  Serial.println(z);
  
  delay(50);
  
  
}

