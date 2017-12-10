int ledBlue = 4;
int ledRed = 5;
int zoneLEDs[] = {6,7,8};
int sensors[] = {9,10,11};  //INPUTS
const int alarm = 2;  //interrupt?
const int ALARM_DURATION = 60000;
boolean ringing = false;
const int flashSpeed = 200; //POLICE LIGHTS

void setup() {
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
  attachInterrupt(digitalPinToInterrupt(1),policeLights,HIGH);
}

void loop(){
  switchZoneLED(getFiredSensor());
  ringAlarm();
}

int getFiredSensor(){
  int count = 0;
  while(!(ringing)){
    if(sensors[count] == HIGH){
      ringing = true; //RINGING TRUE
      return count;
    }
  }
}

void switchZoneLED(int led){
  digitalWrite(led,HIGH);
}

void systemReset(){
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
  while(ringing){
    digitalWrite(alarm,HIGH);
  }
  delay(ALARM_DURATION);
  systemReset();  //********RESETING SYSTEM*********
}

