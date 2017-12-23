int ledBlue = 4;
int ledRed = 5;
/*
 * A constant expression is an expression with a value that is determined during compilation.
 * That value can be evaluated at runtime, but cannot be changed.
 */
constexpr int zoneLEDs[] = {6,7,8};
constexpr int sensors[] = {9,10,11};  //INPUTS
unsigned long margin = 0;
//police lights
int flashSpeed = 50;
const int alarm = 2;
boolean ringing = false;
const long ALARM_DURATION = 5000; //ALTER TIME
boolean trip = false; //used to set the final time recorded for margin as a reference point

void setup(){
  Serial.begin(9600);
  //police lights
  pinMode(ledBlue,OUTPUT);
  pinMode(ledRed,OUTPUT);
  //zone leds
  pinMode(zoneLEDs[0],OUTPUT);
  pinMode(zoneLEDs[1],OUTPUT);
  pinMode(zoneLEDs[2],OUTPUT);
  //sensors INPUTS
  pinMode(sensors[0],INPUT);
  pinMode(sensors[1],INPUT);
  pinMode(sensors[2],INPUT);
  //alarm
  pinMode(alarm,OUTPUT);
  margin = 0;
  Serial.println("Start...");
}

void loop(){
  //testLEDs();
  //testButtons();
  testTimer();
  //delay(10);
}
void testTimer(){
  unsigned long runTime = millis();
  switchZoneLED(getFiredSensor());
  setMargin();
  if((ringing) & (margin <= runTime)){
    Serial.println("margin reached");
    systemReset();
  }
  else{
    ringAlarm();
  }
}
void setMargin(){
  if(!(ringing)){
    margin = millis() + ALARM_DURATION;
  }
}

int getFiredSensor(){
  if(!(ringing)){
    Serial.println("gettingFired");
    for(int i=0; i<3; i++){
      if(digitalRead(sensors[i]) == HIGH){
        ringing = true; //RINGING TRUE
        trip = true;
        Serial.println("SENSOR OFF");
        return sensors[i];
      }
    }
  }
}

void switchZoneLED(int led){
  if((ringing)){
    switch(led){
      case sensors[0]: digitalWrite(zoneLEDs[0],HIGH);
      break;
      case sensors[1]: digitalWrite(zoneLEDs[1],HIGH);
      break;
      case sensors[2]: digitalWrite(zoneLEDs[2],HIGH); 
      break;
    }
  }
}

void policeLights(){
  if(ringing){
    for(int i=0; i<6; i++){
      digitalWrite(ledRed,HIGH);
      delay(flashSpeed);
      digitalWrite(ledRed,LOW);
      delay(flashSpeed);
    }
    for(int i=0; i<6; i++){
      digitalWrite(ledBlue,HIGH);
      delay(flashSpeed);
      digitalWrite(ledBlue,LOW);
      delay(flashSpeed);
    }
  }
}

void ringAlarm(){
  if(ringing){
    digitalWrite(alarm,HIGH);
    policeLights();
  }
}

void systemReset(){
  Serial.println("ACCESS RES");
  ringing = false;
  margin = 0;
  trip = false;
  digitalWrite(alarm,LOW);
  //reset OUTPUT pins
  for(int i=4; i<9; i++){
      digitalWrite(i,LOW);
  }
  Serial.println("SYSTEM RESETING...");
  delay(2000);
  systemRestart();
}
void systemRestart(){
  for(int i=0; i<4; i++){
    digitalWrite(6,HIGH);
    digitalWrite(7,HIGH);
    digitalWrite(8,HIGH);
    delay(200);
    digitalWrite(6,LOW);
    digitalWrite(7,LOW);
    digitalWrite(8,LOW);
    delay(200);
  }
}

//************************TESTS************************

void testButtons(){
  if(digitalRead(9) == HIGH){
    Serial.println("ON 9");
  }
  if(digitalRead(10) == HIGH){
    Serial.println("ON 10");
  }
  if(digitalRead(11) == HIGH){
    Serial.println("ON 11");
  }
  
  else{
    Serial.println("OFF");
  }
}
void testLEDs(){
  for(int i=2; i<9; i++){
    digitalWrite(i,HIGH);
    Serial.println(i);
    delay(200);
    digitalWrite(i,LOW);
    delay(200);
  }
}
