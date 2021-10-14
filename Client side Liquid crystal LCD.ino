#include <arduino-timer.h>
#include <Servo.h>
#include <Wire.h> 
#include <SPI.h>  
#include "RF24.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
Servo myservo;
RF24 myRadio (8, 9);
byte addresses[][6] = {"0"};

struct package {
  int id = 80;
  float temperature = 18.3;
  char automan[4] = "OK";
  char  stopper[4] = "OK";
  char  light[4] = "OK";
  char  gen[4] = "OK";
  char  start[4] = "OK";
};

unsigned long currentMillis; // TO Count Arduino time
byte servoPos = 180; // Starting point 
int servoPause = 500;// Turning speed 
unsigned long previousMillisServo; // To reset current millis
const int LIGHT_HAI_YA_NAHI = A1;   //LIGHT HAI_YA_NAHI.........Logical INPUT
const int GENERATOR_START_STOP = A2;   //GENERAOT_START HAI_YA_NAHI.......Logical INPUT
const int relayPin1 = 5; //Declared Relay1
const int relayPin2 = 4; //Declared Relay2
const int relayPin3 = 3; //Declared Relay3
const int relayPin4 = 2; //Declared Relay3
int relay2Status =HIGH; //Declared Relay2 STATUS OFF
int relay1Status = HIGH;
auto timer = timer_create_default(); // create a timer with default settings

typedef struct package Package;
Package dataRecieve;
Package dataTransmit;

//<<<<<<<<<<<<<<--------------------------------------------------------END DELARATION--------------------------------------------------->>>>>>>>>>>>>>>>>>>>>>>>>>

bool START_GEN (void *) {  
strcpy(dataTransmit.start, "on");
lcd.setCursor(0,1); lcd.print("Starting Generator.."); digitalWrite(relayPin1, !digitalRead(relayPin1));  return true; } // Toggle Gnerator Starting Function

//<<<<<<<<<<<<<<--------------------------------------------------------END FUNCTION AREA--------------------------------------------------->>>>>>>>>>>>>>>>>>>>>>>>>>
void setup() {
  Serial.begin(9600);
 lcd.begin();  
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS );
  
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
         
myservo.attach(6);                                 //SERVO ATTACH PIN
pinMode(LIGHT_HAI_YA_NAHI, INPUT_PULLUP);     //INPUT PULL BC RELAY ATTACH BUTTON
pinMode(GENERATOR_START_STOP, INPUT_PULLUP); //INPUT PULL BC RELAY ATTACH BUTTON
pinMode(relayPin1, OUTPUT);                 //Relay output for START
pinMode(relayPin2, OUTPUT);                //Relay output for STOP
pinMode(relayPin3, OUTPUT);               //Relay output for LIGHT
pinMode(relayPin4, OUTPUT);               //Relay output for LIGHT
digitalWrite(relayPin1, HIGH);         //SETTING HIGH
digitalWrite(relayPin2, HIGH);        //SETTING HIGH
digitalWrite(relayPin3, HIGH);       //SETTING HIGH
digitalWrite(relayPin4, HIGH);       //SETTING HIGH
digitalWrite(LIGHT_HAI_YA_NAHI, HIGH);   //SETTING HIGH
digitalWrite(GENERATOR_START_STOP, HIGH);  //SETTING HIGH
timer.every(4000, START_GEN);              //Call START GEN Fucntion Every 4 SEC
}

void increment(unsigned long &mint) {         //ReSET Time For SELF interval
  mint=0;
 mint = millis()  + 36000;}                 

unsigned long minut= 36000;                    //36000INITIAL 16*2+2 SECONDS..


void go(unsigned long &mint2) {                // //ReSET Time For Choke Break At first
 mint2=0;
 mint2 = millis()  + 18000;}
unsigned long minut2= 18000;                       //16 seconds INITIAL 8*2+2 SECONDS..
int START_BUTTON = 0;
int STOP_BUTTON = 0;
boolean AUTO_MANUAL=LOW;
int CHOKE_BUTTON = LOW; 
int GASS = LOW;

 

void loop() {




  
 if ( myRadio.available()) {
    while (myRadio.available()){
      myRadio.read( &dataRecieve, sizeof(dataRecieve) );
    } }
    delay(50);
   myRadio.stopListening();
  myRadio.openWritingPipe(addresses[0]);
  myRadio.write(&dataTransmit, sizeof(dataTransmit));
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
    String msg1 = dataRecieve.automan;
     String msg2 = dataRecieve.start;
      String msg3 = dataRecieve.stopper;
       String msg4 = dataRecieve.light;
       String msg5 = dataRecieve.gen;
  
        
     if(msg1=="on")
    {
      AUTO_MANUAL = LOW;
    }
    if (msg1=="off")
    {
      AUTO_MANUAL = HIGH;
    }
      if (msg2=="on")
    {
      START_BUTTON = HIGH;
    }
      if (msg2=="off")
    {
      START_BUTTON = LOW;
    } 
    
    if(msg3=="on"){
      STOP_BUTTON = HIGH;
    }
    
    if(msg3=="off"){
      
      STOP_BUTTON = LOW;
      
    }
    if(msg5 == "on"){
    GASS = HIGH;
    }
    if(msg5 == "off"){
     GASS = LOW;
     
    }
    
    currentMillis = millis();                     
  int LIGHT = digitalRead(LIGHT_HAI_YA_NAHI);              // giving name to buttons 
  int GENERATOR = digitalRead(GENERATOR_START_STOP);          // giving name to buttons 
if(AUTO_MANUAL==HIGH){
lcd.setCursor(0,0);
lcd.print("Manual Mode");}else if (millis() < minut ){ 
      lcd.setCursor(9,0);
        for(int n = 0; n < 11; n++)
        {
                lcd.print(" ");
        }
      lcd.setCursor(0,0);
    lcd.print("Auto Mode");};
    if(STOP_BUTTON == HIGH  && relay2Status == LOW){
      relay2Status = STOP_BUTTON;
      relay1Status = relayPin1;}
      
//<<<<<<<<<<<<<<<<<<<------------------------------------------------------------TIME RESET CONDITIONS------------------------------------------------------------>>>>>>>>>>>>>>>>>>>       
if (LIGHT==LOW or AUTO_MANUAL==HIGH or GENERATOR==LOW){
  
  minut=0;
  minut=36000;
  }
if (millis() > minut && LIGHT==LOW or millis()> minut && AUTO_MANUAL==HIGH or millis()> minut && GENERATOR==LOW or LIGHT==LOW or AUTO_MANUAL==HIGH or GENERATOR==LOW)
{

  
  if (minut >= currentMillis +36000){}else{increment(minut);}
  if (minut2>= currentMillis +18000){}else{go      (minut2);}
}
//<<<<<<<<<<<<<<<<<<<------------------------------------------------------------AUTO   START  System------------------------------------------------------------>>>>>>>>>>>>>>>>>>>
if(AUTO_MANUAL==LOW){
if(LIGHT == HIGH || millis() < minut){ digitalWrite(relayPin4, LOW);}else{digitalWrite(relayPin4, HIGH);}

          if(LIGHT==HIGH){
          strcpy( dataTransmit.light, "off");
          }
          if(LIGHT==LOW){
          strcpy( dataTransmit.light, "on");
          }
          if(GENERATOR==HIGH){
          strcpy( dataTransmit.gen, "off");
          }
          if(GENERATOR==LOW){
          strcpy( dataTransmit.gen, "on");
          } 
          if(GENERATOR==LOW && LIGHT == LOW ){
          strcpy( dataTransmit.stopper, "on");
          }
          else
          {
          strcpy( dataTransmit.stopper, "off");
          }
          if(digitalRead(relayPin1)==HIGH){
          strcpy(dataTransmit.start, "off");
          }
          if(currentMillis - previousMillisServo >= servoPause && millis() > minut2 && digitalRead(relayPin1) == 0)
          {     dataTransmit.id = myservo.read();
                
          }
  if(digitalRead(relayPin1) == 1 && millis() < minut ){lcd.setCursor(0,1); for(int n = 0; n < 20; n++)
        {
                lcd.print(" ");
        }}
  lcd.setCursor(0,3);
  if(millis() < minut ){
  if(LIGHT==HIGH){
  lcd.setCursor(0,3);
  lcd.print("LIGHT OFF"); 
  }else{
    lcd.setCursor(0,3);lcd.print("LIGHT ON");lcd.setCursor(8,3);
        for(int n = 0; n < 1; n++)
        {
                lcd.print(" ");
        };
         lcd.setCursor(0,1);
        for(int n = 0; n < 20; n++)
        {
                lcd.print(" ");
        }
         lcd.setCursor(0,2);
        for(int n = 0; n < 20; n++)
        {
                lcd.print(" ");
        }
        }
 
  if(GENERATOR==HIGH){
        lcd.setCursor(9,3);
        for(int n = 0; n < 1; n++)
        {
                lcd.print(" ");}
                lcd.setCursor(17,3);
        for(int n = 0; n < 2; n++)
        {
                lcd.print(" ");
        }lcd.setCursor(10,3);lcd.print("GEN OFF");}else{lcd.setCursor(10,3);lcd.print("GEN ON");lcd.setCursor(16,3);
        for(int n = 0; n < 2; n++)
        {
                lcd.print(" ");
        };
         lcd.setCursor(0,1);
        for(int n = 0; n < 20; n++)
        {
                lcd.print(" ");
        }
         lcd.setCursor(0,2);
        for(int n = 0; n < 20; n++)
        {
                lcd.print(" ");
        }
        }
        }
               
 if(LIGHT==HIGH && GENERATOR==HIGH){ if (millis() < minut) { timer.tick();
if(currentMillis - previousMillisServo >= servoPause && millis() > minut2 && digitalRead(relayPin1) == 0){moveTheServo(); }
                                                                                            }else{ 
myservo.write(180); digitalWrite(relayPin1, HIGH);}   } else{ myservo.write(180); digitalWrite(relayPin1, HIGH);} //<---------------GENERATOR START AUTO

if(LIGHT==LOW && GENERATOR==LOW ){lcd.setCursor(0,1); lcd.print("Stopping Generator"); digitalWrite(relayPin2, LOW);} else{digitalWrite(relayPin2, HIGH); } //<---------------GENERATOR OFF AUTO
START_BUTTON==LOW;  // AUTO SYSTEM SO KEPT OFF
CHOKE_BUTTON==LOW;//   AUTO SYSTEM SO KEPT OFF
if (millis() > minut && AUTO_MANUAL==LOW ){
  lcd.setCursor(0,0);
            dataTransmit.temperature = 19.3;
        
        lcd.print("System Failed To");
        lcd.setCursor(15,1);
           for(int n = 0; n <5; n++)
        {
                lcd.print(" ");
        }
        lcd.setCursor(0,1);
        lcd.print("Start Generator");
        lcd.setCursor(0,2);
        lcd.print("AutomatiCally Please");
        lcd.setCursor(0,3);
        lcd.print("Reset To Try Again");
        }else{
        dataTransmit.temperature = 18.3;
             }
                                             
//<<<<<<<<<<<<<<<<<<<------------------------------------------------------------AUTO System END------------------------------------------------------------>>>>>>>>>>>>>>>>>>>
                }else { //IF MANUAL START FUNCTION ON THEN BELOW CODE WILL BE RESPONSIBLE FOR MANAGE
//<<<<<<<<<<<<<<<<<<<------------------------------------------------------------Manual System------------------------------------------------------------>>>>>>>>>>>>>>>>>>>
if(GASS == HIGH){digitalWrite(relayPin4, LOW);}else{digitalWrite(relayPin4, HIGH);}
lcd.setCursor(11,0); 
        for(int n = 11; n < 20; n++)
        {
          
                lcd.print(" ");
        }
         lcd.setCursor(17,3);
           for(int n = 0; n <1; n++)
        {
                lcd.print(" ");
        }
          lcd.setCursor(9,3);
           for(int n = 0; n <1; n++)
        {
                lcd.print(" ");
        }
        
if(CHOKE_BUTTON==LOW && STOP_BUTTON==HIGH){
        lcd.setCursor(0,2); 
        for(int n = 0; n < 20; n++)
        {
          
                lcd.print(" ");
        }}
if(GENERATOR==HIGH){lcd.setCursor(10,3);lcd.print("GEN OFF");}else{lcd.setCursor(10,3);lcd.print("GEN ON");lcd.setCursor(16,3);
        for(int n = 0; n < 2; n++)
        {
                lcd.print(" ");
        };}
         if(LIGHT==HIGH){lcd.setCursor(0,3);lcd.print("LIGHT OFF"); }else{lcd.setCursor(0,3);lcd.print("LIGHT ON");lcd.setCursor(8,3);
        for(int n = 0; n < 2; n++)
        {
                lcd.print(" ");
        };}
if(START_BUTTON==HIGH && LIGHT==HIGH && GENERATOR==HIGH && STOP_BUTTON==LOW){digitalWrite(relayPin1, LOW); lcd.setCursor(0,1);lcd.print("Starting Generator.."); } else{lcd.setCursor(0,1);for(int n = 0; n < 20; n++)
        {
                lcd.print(" ");
        }digitalWrite(relayPin1, HIGH);} //<---------------GENERATOR START MANUAL
if(STOP_BUTTON==HIGH){digitalWrite(relayPin2, LOW);lcd.setCursor(0,2);lcd.print("Stopping Generator.."); } else{digitalWrite(relayPin2, HIGH);}                                                     //<---------------GENERATOR OFF MANUAL
if (msg4=="on"){CHOKE_BUTTON=HIGH;}
if (msg4=="off"){CHOKE_BUTTON=LOW;}
if(CHOKE_BUTTON==HIGH && GENERATOR==HIGH && STOP_BUTTON ==LOW){  
  
   lcd.setCursor(0,2);
  lcd.print("Moving Choke");
    lcd.setCursor(12,2);
        for(int n = 0; n < 3; n++)
        {
                lcd.print(" ");
        }
  lcd.setCursor(18,2);
        for(int n = 0; n < 2; n++)
        {
                lcd.print(" ");
        }
        lcd.setCursor(15,2);
        myservo.write(dataRecieve.id);
lcd.print("(");
 lcd.print(myservo.read());
lcd.print(")"); }}   }                           //<---------------COKE ON / OFF MANUAL
//<<<<<<<<<<<<<<<<<------------------------------------------------------------Manual System END------------------------------------------------------------>>>>>>>>>>>>>>>>>>               

void moveTheServo()
{
  //Serial.print("moving the servo to "); 
  lcd.setCursor(0,2);
  lcd.print("Moving Choke");
  lcd.setCursor(18,2);
        for(int n = 0; n < 2; n++)
        {
                lcd.print(" ");
        }
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
  
  //Serial.println(servoPos);
myservo.write(servoPos);
lcd.setCursor(15,2);
lcd.print("(");
lcd.print(servoPos);
lcd.print(")");

  previousMillisServo = currentMillis;
  
}//MOVE THE CHOKE SERVO
