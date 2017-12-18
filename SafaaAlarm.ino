int ledBlue = 4;
int ledRed = 5;
/*
 * A constant expression is an expression with a value that is determined during compilation.
 * That value can be evaluated at runtime, but cannot be changed.
 */
constexpr int zoneLEDs[] = {6,7,8};
constexpr int sensors[] = {9,10,11};  //INPUTS
unsigned long runTime;
unsigned long instantTime = 0;
//police lights
int flashSpeed = 50;
const int alarm = 2;
const int ALARM_DURATION = 5000; //ALTER TIME
boolean ringing = false;
long policeSteps = 0;
long swapLights = 0; 

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
  pinMode(alarm,OUTPUT);  //check if INPUT_PULLUP
  Serial.println("Start...");
}

void loop(){
  testTimer();
  //testLEDs();
  //testButtons();
  //switchZoneLED(getFiredSensor());
  //ringAlarm();
  
  //delay(10);
}
void testTimer(){
  runTime = millis();
  const double OFF_TIME = 70;
  const double swapLightsEvery = 500;
  boolean policeRed = false;
  ringing = true;
  
  if(ringing && (instantTime <= ALARM_DURATION)){
    if(policeRed){
      digitalWrite(ledBlue,LOW);
      if(digitalRead(ledRed) == LOW && (runTime-policeSteps) >= OFF_TIME){
      digitalWrite(ledRed,HIGH);
        policeSteps = runTime;
        Serial.println("if");
      }
      else if(digitalRead(ledRed) == HIGH && (runTime - policeSteps) >= OFF_TIME){
        digitalWrite(ledRed,LOW);
        policeSteps = runTime;
        Serial.println("else if");
      }
      instantTime = runTime;
      policeRed = true;
    }
    else{
      digitalWrite(ledRed,LOW);
      if(digitalRead(ledBlue) == LOW && (runTime-policeSteps) >= OFF_TIME){
        digitalWrite(ledBlue,HIGH);
        policeSteps = runTime;
        Serial.println("if");
      }
      else if(digitalRead(ledBlue) == HIGH && (runTime - policeSteps) >= OFF_TIME){
        digitalWrite(ledBlue,LOW);
        policeSteps = runTime;
        Serial.println("else if");
      }
      instantTime = runTime;
    }
  }
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
  
  delay(ALARM_DURATION);
  systemReset();  //********RESETING SYSTEM*********
}

