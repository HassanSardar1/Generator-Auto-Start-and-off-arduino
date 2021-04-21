#include <arduino-timer.h>
#include <Servo.h>
Servo myservo;
//SERVO CHOKE AUTO
unsigned long currentMillis; // TO Count Arduino time
byte servoPos = 180; // Starting point 
int servoPause = 500;// Turning speed 
unsigned long previousMillisServo; // To reset current millis
const int pushButton1 = A0; //Declared 'A' button
const int pushButton2 = A1; //Declared 'B' button
const int pushButton3 = A2; //Declared 'D' button
      int pushButton4 = A3; //Declared 'C' button
const int LIGHT_HAI_YA_NAHI = A4;   //LIGHT HAI_YA_NAHI.........Logical INPUT
const int GENERATOR_START_STOP = A5;   //GENERAOT_START HAI_YA_NAHI.......Logical INPUT
const int Led_GEN_OFF = 8;
const int Led_LIGHT_OFF = 7;
int pushed2 =0;// status of each button
int pushed4 =0;// status of each button
const int relayPin1 = 5; //Declared Relay1
const int relayPin2 = 4; //Declared Relay2
const int relayPin3 = 3; //Declared Relay3
int relay2Status =HIGH; //Declared Relay2 STATUS OFF
auto timer = timer_create_default(); // create a timer with default settings

//<<<<<<<<<<<<<<--------------------------------------------------------END DELARATION--------------------------------------------------->>>>>>>>>>>>>>>>>>>>>>>>>>

bool START_GEN (void *) {  digitalWrite(relayPin1, !digitalRead(relayPin1));  return true; } // Toggle Gnerator Starting Function

//<<<<<<<<<<<<<<--------------------------------------------------------END FUNCTION AREA--------------------------------------------------->>>>>>>>>>>>>>>>>>>>>>>>>>
void setup() {
 
Serial.begin(9600);                                  //Screen DElaring
myservo.attach(6);                                 //SERVO ATTACH PIN
myservo.write(servoPos);                           //INITIALIZE SERVO POSITION
pinMode(pushButton1, INPUT_PULLUP);                      //TAKING ASS INPUT
pinMode(pushButton2, INPUT_PULLUP);                     //TAKING ASS INPUT
pinMode(pushButton3, INPUT_PULLUP);                    //TAKING ASS INPUT
pinMode(pushButton4, INPUT_PULLUP);                   //TAKING ASS INPUT
pinMode(LIGHT_HAI_YA_NAHI, INPUT_PULLUP);     //INPUT PULL BC RELAY ATTACH BUTTON
pinMode(GENERATOR_START_STOP, INPUT_PULLUP); //INPUT PULL BC RELAY ATTACH BUTTON
pinMode(relayPin1, OUTPUT);                 //Relay output for START
pinMode(relayPin2, OUTPUT);                //Relay output for STOP
pinMode(relayPin3, OUTPUT);               //Relay output for LIGHT
pinMode(Led_GEN_OFF, OUTPUT);            //LED output for GENERATOR OFF
pinMode(Led_LIGHT_OFF, OUTPUT);         //LED output for LIGHT OFF
digitalWrite(relayPin1, HIGH);         //SETTING HIGH
digitalWrite(relayPin2, HIGH);        //SETTING HIGH
digitalWrite(relayPin3, HIGH);       //SETTING HIGH
digitalWrite(pushButton1, HIGH);    //SETTING HIGH
digitalWrite(pushButton2, HIGH);    //SETTING HIGH
digitalWrite(pushButton3, HIGH);     //SETTING HIGH
digitalWrite(pushButton4, HIGH);        //SETTING HIGH
digitalWrite(LIGHT_HAI_YA_NAHI, HIGH);   //SETTING HIGH
digitalWrite(GENERATOR_START_STOP, HIGH);  //SETTING HIGH
timer.every(4000, START_GEN);              //Call START GEN Fucntion Every 4 SEC
}

void increment(unsigned long &mint) {         //ReSET Time For SELF interval
 mint = millis()  + 36000;}                 

unsigned long minut= 36000;                    //36000INITIAL 16*2+2 SECONDS..


void go(unsigned long &mint2) {                // //ReSET Time For Choke Break At first
 mint2 = millis()  + 18000;}
unsigned long minut2= 18000;                       //16 seconds INITIAL 8*2+2 SECONDS..



void loop() {
       currentMillis = millis();                      //Getting current arduino time;
       int START_BUTTON = digitalRead(pushButton1);     // giving name to buttons 
       int STOP_BUTTON = digitalRead(pushButton2);        // giving name to buttons 
       int CHOKE_BUTTON = digitalRead(pushButton3);         // giving name to buttons 
       int AUTO_MANUAL = digitalRead(pushButton4);            // giving name to buttons 
       int LIGHT = digitalRead(LIGHT_HAI_YA_NAHI);              // giving name to buttons 
       int GENERATOR = digitalRead(GENERATOR_START_STOP);          // giving name to buttons 
       if(AUTO_MANUAL == HIGH){
      pushed4 = 1-pushed4;}       // Creating PUSH BUTTON
      
    if(STOP_BUTTON == HIGH  && relay2Status == LOW){
      pushed2 = 1-pushed2; }     // CREATIng PUSH BUTton
      relay2Status = STOP_BUTTON;
      
//<<<<<<<<<<<<<<<<<<<------------------------------------------------------------TIME RESET CONDITIONS------------------------------------------------------------>>>>>>>>>>>>>>>>>>>       

if (millis() > minut && LIGHT==LOW or millis()> minut && pushed4==HIGH or millis()> minut && LIGHT==HIGH && GENERATOR==HIGH)
{
  if (minut >= currentMillis +36000){}else{increment(minut);}
  if (minut2>= currentMillis +18000){}else{go      (minut2);}
}


//<<<<<<<<<<<<<<<<<<<------------------------------------------------------------AUTO   START  System------------------------------------------------------------>>>>>>>>>>>>>>>>>>>
if(pushed4==LOW){
               
 if(LIGHT==HIGH && GENERATOR==HIGH){ if (millis() < minut) { timer.tick();
if(currentMillis - previousMillisServo >= servoPause && millis() > minut2){moveTheServo();}
                                                                                            }else{ 
myservo.write(180); digitalWrite(relayPin1, HIGH);}   } else{ myservo.write(180); digitalWrite(relayPin1, HIGH);} //<---------------GENERATOR START AUTO

if(LIGHT==LOW && GENERATOR==LOW ){digitalWrite(relayPin2, LOW);} else{digitalWrite(relayPin2, HIGH); } //<---------------GENERATOR OFF AUTO
START_BUTTON==LOW;  // AUTO SYSTEM SO KEPT OFF
pushed2==LOW;      //  AUTO SYSTEM SO KEPT OFF
CHOKE_BUTTON==LOW;//   AUTO SYSTEM SO KEPT OFF

//<<<<<<<<<<<<<<<<<<<------------------------------------------------------------AUTO System END------------------------------------------------------------>>>>>>>>>>>>>>>>>>>
                }else { //IF MANUAL START FUNCTION ON THEN BELOW CODE WILL BE RESPONSIBLE FOR MANAGE
//<<<<<<<<<<<<<<<<<<<------------------------------------------------------------Manual System------------------------------------------------------------>>>>>>>>>>>>>>>>>>>
if(START_BUTTON==HIGH && LIGHT==HIGH && GENERATOR==LOW && pushed2==LOW){digitalWrite(relayPin1, LOW); } else{digitalWrite(relayPin1, HIGH);} //<---------------GENERATOR START MANUAL
if(pushed2==HIGH){digitalWrite(relayPin2, LOW); } else{digitalWrite(relayPin2, HIGH);}                                                     //<---------------GENERATOR OFF MANUAL
if(CHOKE_BUTTON==HIGH && GENERATOR==LOW && pushed2 ==LOW){  
                                                          if (currentMillis - previousMillisServo >= servoPause) { moveTheServo();} 
                                                         }else{
                                                               if(GENERATOR==HIGH){myservo.write(180);}} }                               //<---------------COKE ON / OFF MANUAL
//<<<<<<<<<<<<<<<<<------------------------------------------------------------Manual System END------------------------------------------------------------>>>>>>>>>>>>>>>>>>               
            
        if(LIGHT == LOW){                              //<---------------LIGHT ON OFF TESTING
    digitalWrite(Led_LIGHT_OFF, LOW); 
  digitalWrite(relayPin3, HIGH);
  
    } 
    else{
        digitalWrite(Led_LIGHT_OFF, HIGH);
        digitalWrite(relayPin3, HIGH);
      }
    if(GENERATOR == HIGH){                               //<---------------GENERATOR ON OFF TESTING
   
    digitalWrite(Led_GEN_OFF, HIGH); 

    } 
    else{
     digitalWrite(Led_GEN_OFF, LOW); 
}
}
void moveTheServo()
{
  Serial.print("moving the servo to ");
  bool newMove=true; //otherwise it does all the moves all the time
  if (servoPos == 180 && newMove) 
  {
    servoPos = 170;
    newMove=false;
  }
if (servoPos == 170&& newMove) 
  {
    servoPos = 160;
    newMove=false;
  }
  if (servoPos == 160 && newMove) 
  {
    servoPos = 150;
    newMove=false;
  }
  if (servoPos == 150 && newMove) 
  {
    servoPos = 140;
    newMove=false;
  }
  
   if (servoPos == 140 && newMove) 
  {
    servoPos = 130;
    newMove=false;
  }
  if (servoPos == 130 && newMove) 
  {
    servoPos = 120;
    newMove=false;
  }
  if (servoPos == 120 && newMove) 
  {
    servoPos = 110;
    newMove=false;
  }
  if (servoPos == 110 && newMove) 
  {
    servoPos = 100;
    newMove=false;
  }
  if (servoPos == 100 && newMove) 
  {
    servoPos = 90;
    newMove=false;
  }
   if (servoPos == 90 && newMove) 
  {
    servoPos = 80;
    newMove=false;
  }
     if (servoPos == 80 && newMove) 
  {
    servoPos = 180;
    newMove=false;
  }
  
  Serial.println(servoPos);
  myservo.write(servoPos);

  previousMillisServo = currentMillis;
  
}//MOVE THE CHOKE SERVO
