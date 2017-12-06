int ledBlue = 2;
int ledRed = 3;

void setup() {
  Serial.begin(9600);
}

void loop() {
  flash(ledBlue);
  flash(ledRed);
}

void flash(const int led){
  digitalWrite(led,HIGH);
  delay(200);
  digitalWrite(led,LOW);
  delay(200);
}

