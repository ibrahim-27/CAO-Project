int IR1 = 14;
int IR2 = 4;
int red = 15;
int green = 13;

// inverted lights...

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(IR1,INPUT);
  pinMode(IR2, INPUT);
  pinMode(red,OUTPUT);
  pinMode(green, OUTPUT);
}

void loop() {
  if(digitalRead(IR1) == HIGH)
  {
    digitalWrite(red, HIGH);
  }
  else
  {
    digitalWrite(red, LOW);
  }

  if(digitalRead(IR2) == HIGH)
  {
    digitalWrite(green, HIGH);
  }
  else
  {
    digitalWrite(green, LOW);
  }

  delay(1000);
}
