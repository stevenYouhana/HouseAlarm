int ledBlue = 4;
int ledRed = 5;
/*
 * A constant expression is an expression with a value that is determined during compilation.
 * That value can be evaluated at runtime, but cannot be changed.
 */
constexpr int zoneLEDs[] = {6,7,8};
constexpr int sensors[] = {9,10,11};  //INPUTS
const int alarm = 2;  //interrupt?
const int ALARM_DURATION = 3000; //ALTER TIME
boolean ringing = false;
const int flashSpeed = 50; //POLICE LIGHTS

void setup(){
  Serial.begin(9600);
  pinMode(3,INPUT);
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
  pinMode(alarm,OUTPUT);  //check if INPUT_PULLUP
  Serial.println("Start...");
  attachInterrupt(digitalPinToInterrupt(3),policeLights,HIGH);
}

void loop(){
  //testLEDs();
  //testButtons();
  switchZoneLED(getFiredSensor());
  ringAlarm();

  delay(10);
}
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
int getFiredSensor(){
  Serial.println("gettingFired");
  while(!(ringing)){
    Serial.println("pass IF");
    for(int i=0; i<3; i++){
      if(digitalRead(sensors[i]) == HIGH){
        ringing = true; //RINGING TRUE
        Serial.println("SENSOR OFF");
        return sensors[i];
      }
    }
  }
}


void switchZoneLED(int led){
  switch(led){
    case sensors[0]: digitalWrite(zoneLEDs[0],HIGH);
    break;
    case sensors[1]: digitalWrite(zoneLEDs[1],HIGH);
    break;
    case sensors[2]: digitalWrite(zoneLEDs[2],HIGH); 
    break;
  }
}

void systemReset(){
  Serial.println("ACCESS RES");
  ringing = false;
  digitalWrite(3,LOW);
  digitalWrite(alarm,LOW);
  //reset OUTPUT pins
  for(int i=4; i<9; i++){
      digitalWrite(i,LOW);
  }
}

void policeLights(){
  if(ringing){
    for(int i=0; i<6; i++){
      digitalWrite(ledRed,HIGH);
      //delay(flashSpeed);
      digitalWrite(ledRed,LOW);
      //delay(flashSpeed);
    }
    for(int i=0; i<6; i++){
      digitalWrite(ledBlue,HIGH);
      //delay(flashSpeed);
      digitalWrite(ledBlue,LOW);
      //delay(flashSpeed);
    }
  }
}

void ringAlarm(){
  digitalWrite(3,HIGH);
  if(ringing){
    digitalWrite(alarm,HIGH);
  }
  delay(ALARM_DURATION);
  systemReset();  //********RESETING SYSTEM*********
}

