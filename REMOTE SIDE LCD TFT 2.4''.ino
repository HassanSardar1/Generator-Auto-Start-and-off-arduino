#include <Adafruit_GFX.h>   
#include <Adafruit_TFTLCD.h> 
#include <TouchScreen.h> 
#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (A5, 10);
byte addresses[][6] = {"0"};
struct package {
  int id = 80;
  float temperature = 18.3;
  char automan[4] = "OK";
  char stopper[4] = "OK";
  char  light[4] = "OK";
  char  gen[4] = "OK";
  char  start[4] = "OK";
 
};

typedef struct package Package;
Package dataRecieve;
Package dataTransmit;
  int hassan = 899;
  int hassan2 = 900;
bool backsensed = false;
bool resetsensed = false;
int period = 0.0000001;
unsigned long time_now = 0;
int newpage = 0;
#define  CHERRY   0xF020
#define YP A3 
#define XM A2 
#define YM 9 
#define XP 8 
#define TS_MINX 100
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);  
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4
#define REDBAR_MINX 60
#define GREENBAR_MINX 100
#define BLUEBAR_MINX 60
#define REDBAR_MINY 10
#define GREENBAR_MINY 60
#define BLUEBAR_MINY 160
#define GREENBAR_HEIGHT 200
#define GREENBAR_WIDTH 30
#define BAR_HEIGHT 150
#define BAR_WIDTH 20
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
#define BLACK   0x0000
int BLUE = tft.color565(50, 50, 255);
#define DARKBLUE 0x0010
#define VIOLET 0x8888
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY   tft.color565(64, 64, 64);
#define GOLD 0xFEA0
#define BROWN 0xA145
#define SILVER 0xC618
#define LIME 0x07E0
int currentpcolour;
#define MINPRESSURE 10
#define MAXPRESSURE 1000
int currentpage;
int redval;
int greenval;
int blueval;
int redpos = REDBAR_MINY + 12;
int greenpos = GREENBAR_MINY + 12;
int bluepos = BLUEBAR_MINY + 12;
int oldrpos = redpos;
int oldgpos = greenpos;
int oldbpos = bluepos;
int x, y;
boolean AutoManual = 0;
boolean pushButton1 = 0; //Declared 'A' button
boolean pushButton2 = 0; //Declared 'B' button
boolean pushButton3 = 0; //Declared 'D' button
boolean pushButton4 = 0; //Declared 'C' button       
void drawHome();
void Manual();

void setup()
{
  Serial.begin(9600);
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS );
  
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
  tft.reset();
  tft.begin(tft.readID());
  Serial.println(); 
  Serial.print("reading id...");
  delay(500);
  Serial.println(tft.readID(), HEX);
  tft.fillScreen(BLACK);
  tft.setRotation(1);

  currentpage = 0;

  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.setCursor(50, 140);
  tft.print("Loading...");

  tft.setTextColor(tft.color565(255, 255, 0));
  tft.setCursor(30, 70);
  tft.print("Welcome");

  tft.setCursor(30, 100);
  tft.print("By: Hassan");
  for (int i = 0; i < 250; i++)
  {
    tft.fillRect(30 - 10, 180, i, 10, RED);
    delay(1);
  }
  tft.fillScreen(BLACK);
 drawHome();
}
bool Gass = false ;
int i = 0 ;
void loop()
{
 
 if ( myRadio.available()) {
    while (myRadio.available()){
     myRadio.read( &dataRecieve, sizeof(dataRecieve));
    }  
      
  }
    delay(50);
  myRadio.stopListening();
  Serial.println(dataTransmit.gen);
  myRadio.openWritingPipe(addresses[0]);
  myRadio.write(&dataTransmit, sizeof(dataTransmit));

  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
TSPoint p = ts.getPoint();   


  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
if (currentpage == 0)
  {  
    Gass = false;
  
    
    if (p.x > 430 && p.x < 590 && p.y > 123 && p.y < 850)
      { 
        strcpy( dataTransmit.automan, "on" );
        if (p.z > ts.pressureThreshhold) {
        Serial.println("Auto Mode IN");
        Serial.println(AutoManual);
         tft.drawRoundRect(17, 160, 280, 50, 8, WHITE);    

        
 
       tft.fillRoundRect(17, 90, 280, 50, 8, GREEN);
  tft.drawRoundRect(17, 90, 280, 50, 8, WHITE); 
  tft.setCursor(40, 110); 

        tft.print("AUTO MODE");

      
        x = 0;
        y = 0;
        p.z = 0;
        autopage();
        currentpage=101;
        
      } }

 if (p.x > 670 && p.x < 760 && p.y > 125 && p.y < 830)
      {if (p.z > ts.pressureThreshhold) {
strcpy( dataTransmit.automan, "off" );
Serial.println("Manual Mode IN");
tft.drawRoundRect(17, 90, 280, 50, 8, WHITE);  
        delay(70);
       tft.fillRoundRect(17, 160, 280, 50, 8, GREEN);  
  tft.drawRoundRect(17, 160, 280, 50, 8, WHITE);
        tft.setCursor(30, 180);
        tft.print("MANUAL MODE");
       currentpage = 1;
       Manual();
           delay(70);
        x = 0;
        y = 0;
        p.z = 0;
        redpos = REDBAR_MINY + 12;
        greenpos = GREENBAR_MINY + 12;
        bluepos = BLUEBAR_MINY + 12;
        oldrpos = redpos;
        oldgpos = greenpos;
        oldbpos = bluepos;  }}//end
    }
    if (currentpage == 1 )
  {
      x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
      y = map(p.y, TS_MINY, TS_MAXY,  tft.height(), 0) + 30;
 if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {    //If sensed a touch
     Serial.print("x = ");
        Serial.println(p.x);
        Serial.print("y = ");
        Serial.println(p.y);
        Serial.print("pressure = ");
        Serial.println(p.z);
  }
    
     
       if (x > 217 && x < 244 && y > 22 && y < 140)
        { 
          if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {
          redval = map(y, 42, 250, 0, 255);
          oldrpos = redpos;
          tft.fillRect(oldrpos, REDBAR_MINX + 3, BAR_WIDTH - 10, BAR_WIDTH - 5, BLACK);
          redpos = y;
          tft.fillRect(redpos, REDBAR_MINX + 3, BAR_WIDTH - 10, BAR_WIDTH - 5, RED);
       
  }        }else{
          redval = map(y, 42, 250, 0, 255);
          oldrpos = redpos;
          tft.fillRect(oldrpos, REDBAR_MINX + 3, BAR_WIDTH - 10, BAR_WIDTH - 5, BLACK);
          tft.fillRect(22, REDBAR_MINX + 3, BAR_WIDTH - 10, BAR_WIDTH - 5, RED); 
          redpos=22; }
  if (p.y > 90 && p.x <200  && p.x > 100 && p.y < 230)
      {
  tft.fillRoundRect(253, 5, 60, 30, 8, RED);
  tft.drawRoundRect(253, 5, 60, 30, 8, WHITE);
  tft.setCursor(260, 13);
   tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.print("GASS");
  if (Gass == true) {
      Gass = false;
     tft.fillRoundRect(253, 5, 60, 30, 8, RED);
  tft.drawRoundRect(253, 5, 60, 30, 8, WHITE);
  tft.setCursor(260, 13);
   tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.print("GASS");
    }else {
      Gass = true;
      tft.fillRoundRect(253, 5, 60, 30, 8, GREEN);
  tft.drawRoundRect(253, 5, 60, 30, 8, WHITE);
  tft.setCursor(260, 13);
   tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.print("GASS");}
        }
        if (Gass == true) {
   strcpy( dataTransmit.gen, "on" );
  }
  if (Gass == false) {
    strcpy( dataTransmit.gen, "off" );
  }

        

 if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  { 
     if (redpos > 70){
      strcpy( dataTransmit.stopper, "on" ); 
      tft.fillRoundRect(0, 150, 319, 90, 8, WHITE);
      tft.setCursor(10, 160);
      tft.setTextSize(2);
      tft.print("Generator Stopped");
     i=0;}else{
        strcpy( dataTransmit.stopper, "off" );
         tft.fillRoundRect(0, 150, 319, 90, 8, WHITE); 
      }
          }
         
 if (x > 146 && x < 180 && y > 72 && y < 236){
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {
           greenval = map(y, 42, 250, 0, 255);
          oldgpos = greenpos;
          tft.fillRect(oldgpos, GREENBAR_MINX + 3, GREENBAR_WIDTH - 10, GREENBAR_WIDTH - 5, BLACK);
          greenpos = y;
          tft.fillRect(greenpos, GREENBAR_MINX + 3, GREENBAR_WIDTH - 10, GREENBAR_WIDTH - 5, GREEN);
  }          }else{
            greenval = map(y, 42, 250, 0, 255);
          oldgpos = greenpos;
          tft.fillRect(oldgpos, GREENBAR_MINX + 3, GREENBAR_WIDTH - 10, GREENBAR_WIDTH - 5, BLACK);
          tft.fillRect(73, GREENBAR_MINX + 3, GREENBAR_WIDTH - 10, GREENBAR_WIDTH - 5, GREEN);
          greenpos = 73;
            }
                    if (greenpos>200){currentpage = 2;}
         
        if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {
        if (x > 215 && x < 240 && y > 172 && y < 320)
        { 
          blueval = map(y, 42, 250, 0, 255);
          oldbpos = bluepos;
          tft.fillRect(oldbpos, BLUEBAR_MINX + 3, BAR_WIDTH - 10, BAR_WIDTH - 5, BLACK);
          if(y <  272){
          bluepos = y;
          }else{bluepos = 272;}
          tft.fillRect(bluepos, BLUEBAR_MINX + 3, BAR_WIDTH - 10, BAR_WIDTH - 5, BLUE);
           int khan = bluepos - 92;
          dataTransmit.id=khan;
          strcpy( dataTransmit.light, "on" ); 
          }else{strcpy( dataTransmit.light, "off" ); }
           }

      if (p.y > 743 && p.x < 247 && p.x > 150 && p.y < 875)
      {
        tft.fillRoundRect(5, 5, 50, 30, 8, WHITE);
        delay(70);
        tft.fillRoundRect(5, 5, 50, 30, 8, BLUE);
        tft.drawRoundRect(5, 5, 50, 30, 8, WHITE);
        tft.setCursor(15, 15);
        tft.print("<-");
        delay(70);
        tft.fillRoundRect(5, 5, 50, 30, 8, BLACK);
        currentpage = 0;
        drawHome();
        p.x = 160;
        p.y = 760;
        redval = 0;
        blueval = 0;
        greenval = 0;
        redpos = REDBAR_MINY + 12;
        greenpos = GREENBAR_MINY + 12;
        bluepos = BLUEBAR_MINY + 12;
        oldrpos = redpos;
        oldgpos = greenpos;
        oldbpos = bluepos;
        i=0;

      }}
      if(currentpage == 2){
      x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
      y = map(p.y, TS_MINY, TS_MAXY,  tft.height(), 0) + 30;
       if (p.y > 90 && p.x <200  && p.x > 100 && p.y < 230)
      {
  tft.fillRoundRect(253, 5, 60, 30, 8, RED);
  tft.drawRoundRect(253, 5, 60, 30, 8, WHITE);
  tft.setCursor(260, 13);
   tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.print("GASS");
  if (Gass == true) {
      Gass = false;
     tft.fillRoundRect(253, 5, 60, 30, 8, RED);
  tft.drawRoundRect(253, 5, 60, 30, 8, WHITE);
  tft.setCursor(260, 13);
   tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.print("GASS");
    }else {
      Gass = true;
      tft.fillRoundRect(253, 5, 60, 30, 8, GREEN);
  tft.drawRoundRect(253, 5, 60, 30, 8, WHITE);
  tft.setCursor(260, 13);
   tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.print("GASS");}
        }
        if (Gass == true) {
   strcpy( dataTransmit.gen, "on" );
  }
  if (Gass == false) {
    strcpy( dataTransmit.gen, "off" );
  }
      

      
        if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {
        if (x > 215 && x < 240 && y > 172 && y < 320)
        { 
          blueval = map(y, 42, 250, 0, 255);
          oldbpos = bluepos;
          tft.fillRect(oldbpos, BLUEBAR_MINX + 3, BAR_WIDTH - 10, BAR_WIDTH - 5, BLACK);
          if(y <  272){
          bluepos = y;
          }else{bluepos = 272;}
          tft.fillRect(bluepos, BLUEBAR_MINX + 3, BAR_WIDTH - 10, BAR_WIDTH - 5, BLUE);
           int khan = bluepos - 92;
           dataTransmit.id=khan;
            strcpy( dataTransmit.light, "on" );
               
          }else{strcpy( dataTransmit.light, "off" ); }
     }
 if (x > 146 && x < 180 && y > 72 && y < 236)
        {
          if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {
          
          greenval = map(y, 42, 250, 0, 255);
          oldgpos = greenpos;
          tft.fillRect(oldgpos, GREENBAR_MINX + 3, GREENBAR_WIDTH - 10, GREENBAR_WIDTH - 5, BLACK);
          greenpos = y;
          tft.fillRect(greenpos, GREENBAR_MINX + 3, GREENBAR_WIDTH - 10, GREENBAR_WIDTH - 5, GREEN);
     }            }else{
            greenval = map(y, 42, 250, 0, 255);
          oldgpos = greenpos;
          tft.fillRect(oldgpos, GREENBAR_MINX + 3, GREENBAR_WIDTH - 10, GREENBAR_WIDTH - 5, BLACK);
          tft.fillRect(73, GREENBAR_MINX + 3, GREENBAR_WIDTH - 10, GREENBAR_WIDTH - 5, GREEN);
          greenpos = 73;
            }
                    if (greenpos>200){currentpage = 2;}
        if (x > 217 && x < 244 && y > 22 && y < 140)
        {
           if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {

          redval = map(y, 42, 250, 0, 255);
          oldrpos = redpos;
          tft.fillRect(oldrpos, REDBAR_MINX + 3, BAR_WIDTH - 10, BAR_WIDTH - 5, BLACK);
          redpos = y;
          tft.fillRect(redpos, REDBAR_MINX + 3, BAR_WIDTH - 10, BAR_WIDTH - 5, RED);  
            
    }      }else{
    
       redval = map(y, 42, 250, 0, 255);
          oldrpos = redpos;
          tft.fillRect(oldrpos, REDBAR_MINX + 3, BAR_WIDTH - 10, BAR_WIDTH - 5, BLACK);
          tft.fillRect(22, REDBAR_MINX + 3, BAR_WIDTH - 10, BAR_WIDTH - 5, RED);  
          redpos = 22;  
      }
            if (redpos > 70){
   strcpy( dataTransmit.stopper, "on");
      tft.fillRoundRect(0, 150, 319, 90, 8, WHITE);  
      tft.setCursor(10, 160);
      tft.setTextSize(2);
      tft.print("Generator Stopped");
      currentpage = 1 ;
      i=0;}else{  
        strcpy( dataTransmit.stopper, "off");
        tft.fillRoundRect(0, 150, 319, 90, 8, WHITE);  }
      
 if(greenpos > 200){ strcpy( dataTransmit.start, "on");
  Serial.println("Starting Generator");
  tft.setCursor(80, 60);
  tft.fillRoundRect(0, 150, 319, 90, 8, WHITE);
   if(i>250){
    tft.setCursor(80, 60);
    tft.fillRoundRect(0, 150, 319, 90, 8, WHITE);  
    i=0;}else{
    tft.setCursor(10, 160);
    tft.setTextSize(2);
    tft.print("Starting Generator....");
     if(millis() > time_now + period){
      time_now = millis();
      i++;
      p.x=0;
      p.y=0;
      tft.fillRect(20, 190, i, 10, RED);
        }
         }
          }else{
              strcpy( dataTransmit.start, "off");
           tft.setCursor(80, 60);
          tft.fillRoundRect(0, 150, 319, 90, 8, WHITE);
          i=0;
          }
      

  if (p.y > 743 && p.x < 247 && p.x > 150 && p.y < 875)
        {
          tft.fillRoundRect(5, 5, 50, 30, 8, WHITE);
          delay(70);
          tft.fillRoundRect(5, 5, 50, 30, 8, BLUE);
          tft.drawRoundRect(5, 5, 50, 30, 8, WHITE);
          tft.setCursor(15, 15);
          tft.print("<-");
          delay(70);
          tft.fillRoundRect(5, 5, 50, 30, 8, BLACK);
         
          p.x = 160;
          p.y = 760;
           currentpage = 0;
          drawHome();
          i=0;
          return;
          
       }

  p.x=0;
  p.y=0;
       }
if(currentpage == 101)
{
String msg = dataRecieve.light;
String msg2 = dataRecieve.gen;
String msg3 = dataRecieve.start;
String msg4 = dataRecieve.stopper;

  
    if(msg=="off")
    {    //LIGHT
         tft.setTextColor(BLACK);
      tft.setCursor(235, 175);          
      tft.setTextSize(2);
    tft.fillRoundRect(225, 165, 90, 30, 8, WHITE); //LIGHT
     tft.print("OFF");
    }
     if (msg=="on")
    {
      //LIGHT
       tft.setTextColor(BLACK);
      tft.setCursor(235, 175);
      tft.setTextSize(2);
    tft.fillRoundRect(225, 165, 90, 30, 8, WHITE); //LIGHT
     tft.print("ON");
    }
     if (msg2=="off")
    {
        //GENERATOR
       tft.setTextColor(BLACK);
       tft.setCursor(235, 135);
       tft.setTextSize(2);
       tft.fillRoundRect(225, 125, 90, 30, 8, WHITE); // GEN
       tft.print("OFF");
    }
     if (msg2=="on")
    {
        //GENERATOR
      tft.setTextColor(BLACK);
        tft.setCursor(235, 135);
         tft.setTextSize(2);
        tft.fillRoundRect(225, 125, 90, 30, 8, WHITE); // GEN
     tft.print("ON");
    }
    if (msg4=="on" )
    {
      //STOP GENERATOR
      
  tft.fillRoundRect(225, 85, 90, 30, 8, WHITE);   // stop
    tft.setTextColor(BLACK);
        tft.setCursor(235,  95);
        tft.setTextSize(1);
          tft.print("Stopping");
    }
    if(msg4 == "off")
    {
       //STOP GENERATOR
       
  tft.fillRoundRect(225, 85, 90, 30, 8, WHITE);   // stop
    tft.setTextColor(BLACK);
    tft.setTextColor(BLACK);
        tft.setCursor(235,  95);
        tft.setTextSize(2);
          tft.print("Free");
    }
     if (msg3 == "on")
    {
       
      //Choke
      tft.setTextColor(BLACK);
        tft.setCursor(235, 215);
  tft.fillRoundRect(225, 205, 90, 30, 8, WHITE); //Choke
         tft.setTextSize(2);
     tft.print(dataRecieve.id);
   

tft.fillRoundRect(225, 45, 90, 30, 8, WHITE);   //STart    tft.setTextColor(BLACK);
        tft.setCursor(235, 55);
        tft.setTextSize(1);
     tft.print("Starting");
    }
    else if (msg3 == "off")
    {
      

tft.fillRoundRect(225, 45, 90, 30, 8, WHITE);   //STart
    tft.setTextColor(BLACK);
     tft.setCursor(235, 55);
        tft.setTextSize(1);
     tft.print("NotStarting");
    }
   
     if (dataRecieve.temperature == 19.3 && hassan == 899)
    {
     hassan2=900;
      hassan++;
      Serial.print(hassan);
     tft.fillRoundRect(4, 45, 220, 190, 8, WHITE); //ERROR
     tft.setTextSize(2);
     tft.setCursor(10, 60);
     tft.setTextColor(RED);
     tft.print("Error");
     tft.setCursor(10, 90);
     tft.setTextColor(BLUE);
     tft.print("System Failed To");
     tft.setCursor(10, 110);
     tft.print("Start Generator");
     tft.setCursor(10, 130);
     tft.print("Automatically :-(");
     tft.setCursor(10, 150);
     tft.print("Please Reset To");
     tft.setCursor(10, 170);
     tft.print("Try Again......");
  } else if (dataRecieve.temperature == 18.3 && hassan2 != 901){
      hassan=899;
      hassan2 ++;
   autopage();
  }
     


    
  if (p.y > 743 && p.x < 247 && p.x > 150 && p.y < 875)
        {
          tft.fillRoundRect(5, 5, 50, 30, 8, WHITE);
          delay(70);
          tft.fillRoundRect(5, 5, 50, 30, 8, BLUE);
          tft.drawRoundRect(5, 5, 50, 30, 8, WHITE);
          tft.setCursor(15, 15);
          tft.print("<-");
          delay(70);
          tft.fillRoundRect(5, 5, 50, 30, 8, BLACK);
         
          p.x = 160;
          p.y = 760;
           currentpage = 0;
          drawHome();
          i=0;
          return;
          
      }
         
} 
}

void drawHome()
{
 tft.fillScreen(BLACK);
  tft.drawRoundRect(0, 0, 319, 240, 8, WHITE);    
  tft.fillRoundRect(17, 160, 280, 50, 8, RED);  
  tft.drawRoundRect(17, 160, 280, 50, 8, WHITE);
  tft.fillRoundRect(17, 90, 280, 50, 8, RED);
  tft.drawRoundRect(17, 90, 280, 50, 8, WHITE); 
  /*x0：x coordinate of the starting point
 y0：y coordinate of the starting point
 w：the length of the square 
 h：the width of the square
 radius：the radius of the round corners
color：the color of the square */
  tft.setCursor(10, 20);
  tft.setTextSize(2);
  tft.setFont();
  tft.setTextColor(WHITE);
  tft.print("Welcome To Main Menu");
  tft.setCursor(30, 50);
  tft.setTextSize(2);
  tft.setTextColor(LIME);
  tft.print("Select Control Options");
  tft.setTextColor(BLACK);
  tft.setCursor(40, 110);
  tft.print("Auto Start Generator");
  tft.setCursor(30, 180);
  tft.print("Manual Start Generator");
}
void Manual()
  { 
  tft.fillScreen(BLACK);
  tft.drawRoundRect(0, 0, 319, 240, 8, WHITE);      
  tft.setCursor(100, 10);
  tft.setTextSize(2);
  tft.setFont();
  tft.setTextColor(WHITE);
  tft.print("Manual Mode");
    tft.setTextSize(1);
  tft.setTextColor(WHITE);
  tft.setCursor(70, 50);
  tft.print("Stop");
   tft.setCursor(230, 50);
  tft.print("Choke");
  tft.setCursor(140, 90);
  tft.print("Start");
    tft.fillRoundRect(253, 5, 60, 30, 8, RED);
  tft.drawRoundRect(253, 5, 60, 30, 8, WHITE);
  tft.setCursor(260, 13);
   tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.print("GASS");
  
  tft.drawRect(BLUEBAR_MINY, BLUEBAR_MINX, BAR_HEIGHT, BAR_WIDTH, WHITE); 

  tft.drawRect(GREENBAR_MINY, GREENBAR_MINX, GREENBAR_HEIGHT, GREENBAR_WIDTH, WHITE);  

  tft.drawRect(REDBAR_MINY, REDBAR_MINX, BAR_HEIGHT, BAR_WIDTH, WHITE);

  tft.fillRect(REDBAR_MINY + 12, REDBAR_MINX + 3, BAR_WIDTH - 10, BAR_WIDTH - 5, RED);

  tft.fillRect(GREENBAR_MINY + 12, GREENBAR_MINX + 3, GREENBAR_WIDTH - 10, GREENBAR_WIDTH - 5, GREEN);

  tft.fillRect(BLUEBAR_MINY + 12, BLUEBAR_MINX + 3, BAR_WIDTH - 10, BAR_WIDTH - 5, BLUE);


  tft.fillRoundRect(5, 5, 50, 30, 8, BLUE);
  tft.drawRoundRect(5, 5, 50, 30, 8, WHITE);
  tft.setCursor(15, 15);
   tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.print("<-");
  delay(300);
}
void autopage(){
  tft.fillScreen(BLACK);
  tft.drawRoundRect(0, 0, 319, 240, 8, WHITE);    
  tft.fillRoundRect(225, 45, 90, 30, 8, WHITE);  
  tft.drawRoundRect(225, 45, 90, 30, 8, RED);
  tft.fillRoundRect(225, 85, 90, 30, 8, WHITE);  
  tft.drawRoundRect(225, 85, 90, 30, 8, RED);
  tft.fillRoundRect(225, 125, 90, 30, 8, WHITE);
  tft.drawRoundRect(225, 125, 90, 30, 8, RED); 
  tft.fillRoundRect(225, 165, 90, 30, 8, WHITE);
  tft.drawRoundRect(225, 165, 90, 30, 8, RED); 
  tft.fillRoundRect(225, 205, 90, 30, 8, WHITE);
  tft.drawRoundRect(225, 205, 90, 30, 8, RED); 
  tft.setCursor(65, 20);
  tft.setTextSize(2);
  tft.setFont();
  tft.setTextColor(WHITE);
  tft.print("Auto Information Page");
  tft.setCursor(10, 60);
  tft.setTextColor(GREEN);
  tft.print("Starting Generator");
  tft.setCursor(10, 100);
  tft.print("Stopping Generator"); 
  tft.setCursor(10, 140);  
  tft.print("Generator"); 
  tft.setCursor(10, 180);
  tft.print("Light");
  tft.setCursor(10, 220);
  tft.print("Choke");
  
    tft.fillRoundRect(5, 5, 50, 30, 8, BLUE);
  tft.drawRoundRect(5, 5, 50, 30, 8, WHITE);
  tft.setCursor(15, 15);
   tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.print("<-");
  delay(300);
  }
