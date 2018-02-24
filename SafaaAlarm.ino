const byte ledBlue = 4;
const byte ledRed = 5;
//led to keep flashing if alarm is gone off
const byte goneOff = 3;
unsigned long goneOffprev = 0;
int goneOffFlashing = 500;
int goneOffState = LOW;
int resetButton = 12;

/*
 * A constant expression is an expression with a value that is determined during compilation.
 * That value can be evaluated at runtime, but cannot be changed.
 */
constexpr byte zoneLEDs[] = {6,7,8};
constexpr byte sensors[] = {9,10,11};  //INPUTS
unsigned long margin = 0;
//POLICE LIGHTS TIMER VARIABLES
int policeShift = 40;
unsigned long previousePoliceShift = 0;
boolean policeState = false;
int policeCountFlip = 0;
//police lights
int flashSpeed = 50;
const int alarm = 2;
boolean onZones[] = {false,false,false};
boolean ringing = false;
const long ALARM_DURATION = 60000; //ALTER TIME
boolean trip = false; //used to set the final time recorded for margin as a reference point
const long resetTime = 3000; //hard reset time of the system
  
void setup(){
  Serial.begin(9600);
  //police lights
  pinMode(ledBlue,OUTPUT);
  pinMode(ledRed,OUTPUT);
  pinMode(goneOff,OUTPUT);
  //zone leds
  pinMode(zoneLEDs[0],OUTPUT);
  pinMode(zoneLEDs[1],OUTPUT);
  pinMode(zoneLEDs[2],OUTPUT);
  //sensors INPUTS
  pinMode(sensors[0],INPUT);
  pinMode(sensors[1],INPUT);
  pinMode(sensors[2],INPUT);
  pinMode(resetButton,INPUT);
  //alarm
  pinMode(alarm,OUTPUT);
  Serial.println("Start...");
}

void loop(){
  //testLEDs();
  //testButtons();
  mainOperation();
}
void mainOperation(){
  unsigned long runTime = millis();
  switchZoneLED(getFiredSensor());
  setMargin();
  if((ringing) && (margin <= runTime)){
    Serial.println("margin reached");
    systemReset();
  }
  else{
    ringAlarm();
  }
  goneOffLed();
  hardReset();
}
void setMargin(){
  if(!(ringing)){
    margin = millis() + ALARM_DURATION;
  }
}

int getFiredSensor(){
  if(!(ringing)){
    Serial.println("standing by...");
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
  if(ringing){
    switch(led){
      case sensors[0]: digitalWrite(zoneLEDs[0],HIGH); onZones[0] = true;
      break;
      case sensors[1]: digitalWrite(zoneLEDs[1],HIGH); onZones[1] = true;
      break;
      case sensors[2]: digitalWrite(zoneLEDs[2],HIGH); onZones[2] = true;
      break;
    }
  }
}

void policeLights(){
  /*
   * unsigned long policeShift = 50;
   * unsigned long previousePoliceShift = 0;
   * boolean policeState = false;
   * int policeCountFlip = 0;
   */
  if(policeCountFlip == 18){
    policeCountFlip = 0;
  }
  if(policeCountFlip <= 6){
    digitalWrite(ledRed, policeState);
    if((millis() - previousePoliceShift) >= policeShift){
      digitalWrite(ledBlue,HIGH); //overlap
      policeState = !policeState;
      digitalWrite(ledRed, policeState);
      policeCountFlip++;
      previousePoliceShift = millis();
      digitalWrite(ledBlue,LOW);
    }
  }
  else{
    if((millis() - previousePoliceShift) >= policeShift){
      digitalWrite(ledRed,HIGH);  //overlap
      policeState = !policeState;
      digitalWrite(ledBlue, policeState);
      policeCountFlip++;
      previousePoliceShift = millis();
      digitalWrite(ledRed,LOW);
    }
  }
}
void ringAlarm(){
  if(ringing){
    digitalWrite(alarm,HIGH);
    policeLights();
  }
}
void goneOffLed(){
  if(onZones[0] || onZones[1] || onZones[2]){
    if((millis() - goneOffprev) >= 500){
      goneOffState = !goneOffState;
      digitalWrite(goneOff, goneOffState);
      goneOffprev = millis();
    }
  }
}
void hardReset(){
  unsigned long holder = 0;
  while(digitalRead(resetButton) == HIGH){
    //if ringing, keep ringing within the 3 button HIGH duration
    ringAlarm();
    if((millis()-holder) >= resetTime){
      if((millis()-holder) == resetTime){
        systemHardReset();
      }
      holder = millis();    
    }
  }
}
void systemReset(){
  Serial.println("ACCESS RES");
  ringing = false;
  margin = 0;
  trip = false;
  digitalWrite(alarm,LOW);
  //reset OUTPUT pins
  digitalWrite(alarm,LOW);
  digitalWrite(ledRed,LOW);
  digitalWrite(ledBlue,LOW);
  Serial.println("SYSTEM RESETING...");
  delay(2000);
  systemRestart();
}
void systemHardReset(){
  onZones[0] = false;
  onZones[1] = false;
  onZones[2] = false;
  for(int i=0; i<12; i++){
    digitalWrite(i,LOW);
  }
  ringing = false;
  margin = 0;
  trip = false;
  for(int c=0; c<3; c++){
    for(int i=0; i<3; i++){
      digitalWrite(zoneLEDs[i],HIGH);
      delay(50);
      digitalWrite(zoneLEDs[i],LOW);
    }
    for(int i=3; i>=0; i--){
      digitalWrite(zoneLEDs[i],HIGH);
      delay(50);
      digitalWrite(zoneLEDs[i],LOW);
    }
  }
}
void systemRestart(){
   for(int i=0; i<4; i++){
        digitalWrite(zoneLEDs[0],HIGH);
        digitalWrite(zoneLEDs[1],HIGH);
        digitalWrite(zoneLEDs[2],HIGH);
        delay(200);
        digitalWrite(zoneLEDs[0],LOW);
        digitalWrite(zoneLEDs[1],LOW);
        digitalWrite(zoneLEDs[2],LOW);
        delay(200);
    }
  for(int on=0; on<3; on++){
    Serial.print(onZones[on]);
    if(onZones[on]){
      digitalWrite(on+zoneLEDs[0],HIGH);
    }
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
