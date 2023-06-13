//Badminton Score Board with Team Serving
#include <SPI.h>        //SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include "DMD.h"        //
#include "TimerOne.h"   //
#include "SystemFont5x7.h"
#include "fixednums7x15.h"
#include "Arial_black_16.h"

#include "RCSwitch.h"

RCSwitch mySwitch = RCSwitch();

//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 2
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

int led=13;
String currentButton;
String secondButton;
String buttonCodes[]={"3001","3002","3003","3004"};
int countPress;
long int timePress;
int setA;char setAbuff[8];
int setB;char setBbuff[8];
char scoreA;char scoreAbuff[8];
char scoreB;char scoreBbuff[8];
int scoreX=17;
int scoreX_s=25;
int scoreYA=0;
int scoreYB=16;
int setX=11;
int setYA=1;
int setYB=17;
int serve;
bool toggle;
void ScanDMD()
{ 
  dmd.scanDisplayBySPI();
}
void clearRowServe(){
  for (int j=17;j<32;j++){   //Column
    for (int i=30;i<32;i++){  //Row
      dmd.writePixel(  j,i, GRAPHICS_NORMAL,0);
    }
  }
  for (int j=17;j<32;j++){   //Column
    for (int i=14;i<16;i++){  //Row
      dmd.writePixel(  j,i, GRAPHICS_NORMAL,0);
    }
  }
}
void drawServe(int Team){
  clearRowServe();
  dmd.drawFilledBox(  17, (Team-1)*2+Team*14,31,(Team-1)*2+Team*14+1,GRAPHICS_NORMAL );
}

void drawScore(int8_t scoreAtemp, int8_t scoreBtemp){
  if (scoreAtemp<10){
    dmd.drawString(  scoreX,  scoreYA, "0", 2, GRAPHICS_NORMAL );
    dmd.drawString(  scoreX_s,  scoreYA, itoa (scoreAtemp, scoreAbuff, 10), 2, GRAPHICS_NORMAL );
  } else{
    dmd.drawString(  scoreX,  scoreYA, itoa (scoreAtemp, scoreAbuff, 10), 2, GRAPHICS_NORMAL );
  }
  
  if (scoreBtemp<10){
    dmd.drawString(  scoreX, scoreYB, "0", 2, GRAPHICS_NORMAL );
    dmd.drawString(  scoreX_s, scoreYB, itoa (scoreBtemp, scoreBbuff, 10), 2, GRAPHICS_NORMAL );
  } else{
    dmd.drawString(  scoreX, scoreYB, itoa (scoreBtemp, scoreBbuff, 10), 2, GRAPHICS_NORMAL );
  }
}

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0); 
  delay(2000); while (!Serial); //delay for Leonardo
  pinMode(led, OUTPUT);
  Serial.println(F("Ready to receive RF signals"));
  
     //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
   Timer1.initialize( 5000 );           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
   Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()
  
  // Initiating Variable
  setA=0;
  setB=0;
  scoreA=0;
  scoreB=0;
  serve=1;
  toggle=false;
  countPress=0;
  currentButton="1000";
  secondButton="1000";
  //clear/init the DMD pixels held in RAM
  dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
  //initializing
  dmd.selectFont(Arial_Black_16);   
  dmd.drawChar(  0,  scoreYA,  'A', GRAPHICS_NORMAL );
  dmd.drawChar(  0,  scoreYB,  'N', GRAPHICS_NORMAL );
  dmd.selectFont(SystemFont5x7);
   itoa (setA, setAbuff, 10);
   itoa (setB, setBbuff, 10);
   if (setA) dmd.drawString(  setX,  setYA,  setAbuff, 1, GRAPHICS_NORMAL );
   if (setB) dmd.drawString(  setX,  setYB, setBbuff, 1, GRAPHICS_NORMAL );
   dmd.selectFont(fixednums7x15);
   drawScore(scoreA, scoreB);
//   dmd.drawFilledBox(17,14,31,15,GRAPHICS_NORMAL);
   drawServe(serve);
}
 
void loop() {

  byte b;
  //Continue looping until you get a complete signal received
  if (millis()-timePress>100){
      countPress=0;
    }
  if (mySwitch.available()) {
    secondButton=String(mySwitch.getReceivedValue());
    if (secondButton==currentButton && ((millis()-timePress)<100||countPress==1)){
      countPress=countPress+1;
      mySwitch.resetAvailable();
      Serial.println(currentButton);
      Serial.println(secondButton);
      if (countPress==10){
        if (currentButton==buttonCodes[3]){
          setB=setB-1;
          Serial.println("SetB-");
        }
        else if (currentButton==buttonCodes[1]){
          if (setA+setB<3) setB=setB+1;
          Serial.println("SetB+");
          serve=2;
          scoreA=0;
          scoreB=0;
          toggle=true;
        }
        else if (currentButton==buttonCodes[2]){
          setA=setA-1;
          Serial.println("SetA-");
        }
        else if (currentButton==buttonCodes[0]){
          if (setA+setB<3) setA=setA+1;
          Serial.println("SetA+");
          serve=1;
          scoreA=0;
          scoreB=0;
          toggle=false;
        }
        if (scoreA<0) scoreA=0;
        else if (scoreA>31) scoreA=31;
        if (scoreB<0) scoreB=0;
        else if (scoreB>31) scoreB=31;      
        if (setA<0) setA=0; 
        if (setA>2) setA=2;
        if (setB<0) setB=0; 
        if (setB>2) setB=2;
        drawScore(scoreA, scoreB);
        drawServe(serve);
        if (serve>2) {
          serve=1;
          drawServe(serve);
        } else if (serve<1){
          serve=2;
          drawServe(serve);
        }
        mySwitch.resetAvailable();
        dmd.selectFont(SystemFont5x7);      
        itoa (setA, setAbuff, 10);itoa (setB, setBbuff, 10);
        if (setA) dmd.drawString(  setX,  setYA,  setAbuff, 1, GRAPHICS_NORMAL );
        if (setB) dmd.drawString(  setX,  setYB, setBbuff, 1, GRAPHICS_NORMAL );
        dmd.selectFont(fixednums7x15);
      }
      else if (countPress==20){
      Serial.println("in countpress>20");
//      if ((currentButton==buttonCodes[0] || currentButton==buttonCodes[1]) && (secondButton==buttonCodes[2] || secondButton==buttonCodes[3]) ){
        setA=0;
        setB=0;
        scoreA=0;
        scoreB=0;
        toggle=false;
        serve=1;
//      }
        if (scoreA<0) scoreA=0;
        else if (scoreA>31) scoreA=31;
        if (scoreB<0) scoreB=0;
        else if (scoreB>31) scoreB=31;      
        if (setA<0) setA=0; 
        if (setA>2) setA=2;
        if (setB<0) setB=0; 
        if (setB>2) setB=2;
        drawScore(scoreA, scoreB);
        drawServe(serve);
        if (serve>2) {
          serve=1;
          drawServe(serve);
        } else if (serve<1){
          serve=2;
          drawServe(serve);
        }
        countPress=0;
        mySwitch.resetAvailable();
        dmd.selectFont(SystemFont5x7);
        itoa (setA, setAbuff, 10);itoa (setB, setBbuff, 10);
        if (setA) dmd.drawString(  setX,  setYA,  setAbuff, 1, GRAPHICS_NORMAL );
        if (setB) dmd.drawString(  setX,  setYB, setBbuff, 1, GRAPHICS_NORMAL );
        dmd.selectFont(fixednums7x15);
    }
    }
    
    
    else if (countPress<10){
      currentButton=secondButton;
      countPress=1;
      if (currentButton!=buttonCodes[9] && currentButton!="0"){
        
        if (currentButton==buttonCodes[0]){
          scoreA=scoreA+1;
          if (toggle){
            serve=serve+1;
//            drawServe(serve);
            toggle=false;
          }
          Serial.println("ScoreA+");
        }
        else if (currentButton==buttonCodes[2]){
          scoreA=scoreA-1;
          Serial.println("ScoreA-");
        }
        else if (currentButton==buttonCodes[1]){
          scoreB=scoreB+1;
          if (!toggle){
            serve=serve+1;
//            drawServe(serve);
            toggle=true;
          }
          Serial.println("scoreB+");
        }
        else if (currentButton==buttonCodes[3]){
          scoreB=scoreB-1;
          Serial.println("scoreB-");
        }
        if (scoreA<0) scoreA=0;
        else if (scoreA>31) scoreA=31;
        if (scoreB<0) scoreB=0;
        else if (scoreB>31) scoreB=31;      
        if (setA<0) setA=0; 
        if (setA>2) setA=2;
        if (setB<0) setB=0; 
        if (setB>2) setB=2;
        drawScore(scoreA, scoreB);
        drawServe(serve);
        if (serve>2) {
          serve=1;
          drawServe(serve);
        } else if (serve<1){
          serve=2;
          drawServe(serve);
        }
        mySwitch.resetAvailable();
        dmd.selectFont(SystemFont5x7);
        itoa (setA, setAbuff, 10);itoa (setB, setBbuff, 10);
        if (setA) dmd.drawString(  setX,  setYA,  setAbuff, 1, GRAPHICS_NORMAL );
        if (setB)dmd.drawString(  setX,  setYB, setBbuff, 1, GRAPHICS_NORMAL );
        dmd.selectFont(fixednums7x15);
      }
    }
    timePress=millis();
    }
  
}
