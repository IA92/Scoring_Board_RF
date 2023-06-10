//Badminton Scoring Board with Player Service Indicator

#include "IRLibAll.h"
#include <SPI.h>        //SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include <DMD.h>        //
#include <TimerOne.h>   //
#include "SystemFont5x7.h"
#include "Arial_black_16.h"
#include "Droid_Sans_12.h"

//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

//Create a receiver object to listen on pin 2
IRrecvPCI myReceiver(2);
 
//Create a decoder object 
IRdecode myDecoder;   

int led=13;
String currentButton;
String buttonCodes[]={"40bfd22d","40bf12ed","40bf10ef","40bfd02f","40bfe21d","40bf22dd","40bff20d","40bf32cd","40bf28d7","ffffffff"};

int setA;char setAbuff[8];
int setB;char setBbuff[8];
char scoreA;char scoreAbuff[8];
char scoreB;char scoreBbuff[8];
int scoreX=5;
int scoreX_s=11;
int scoreXm=20;
int scoreXm_s=26;
int scoreY=9;
int setAX=8;
int setBX=23;
int serve;
bool toggle;
bool toggleM;
void ScanDMD()
{ 
  dmd.scanDisplayBySPI();
}
void clearRow_12(){
  for (int j=0;j<2;j++){
    for (int i=0;i<16;i++){
      dmd.writePixel(  j,i, GRAPHICS_NORMAL,0);
    }
  }
}
void drawServe(int Player){
  clearRow_12();
  for (int i=0; i<Player;i++){
    dmd.drawFilledBox(  0, 1+4*i,1,1+4*i+1,GRAPHICS_NORMAL );
  }
}

void drawScore(int8_t scoreAtemp, int8_t scoreBtemp){
  if (scoreAtemp<10){
    dmd.drawString(  scoreX,  scoreY, "0", 2, GRAPHICS_NORMAL );
    dmd.drawString(  scoreX_s,  scoreY, itoa (scoreAtemp, scoreAbuff, 10), 2, GRAPHICS_NORMAL );
  } else{
    dmd.drawString(  scoreX,  scoreY, itoa (scoreAtemp, scoreAbuff, 10), 2, GRAPHICS_NORMAL );
  }
  
  if (scoreBtemp<10){
    dmd.drawString(  scoreXm, scoreY, "0", 2, GRAPHICS_NORMAL );
    dmd.drawString(  scoreXm_s, scoreY, itoa (scoreBtemp, scoreBbuff, 10), 2, GRAPHICS_NORMAL );
  } else{
    dmd.drawString(  scoreXm, scoreY, itoa (scoreBtemp, scoreBbuff, 10), 2, GRAPHICS_NORMAL );
  }
}

void setup() {
  Serial.begin(9600);
  delay(2000); while (!Serial); //delay for Leonardo
  myReceiver.enableIRIn(); // Start the receiver
  pinMode(led, OUTPUT);
  Serial.println(F("Ready to receive IR signals"));
  
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
  toggleM=false;
   //clear/init the DMD pixels held in RAM
   dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)
   dmd.selectFont(System5x7);
   //Initializing
   itoa (setA, setAbuff, 10);
   itoa (setB, setBbuff, 10);
   dmd.drawString(  setAX,  0,  setAbuff, 1, GRAPHICS_NORMAL );
   dmd.drawString(  setBX,  0, setBbuff, 1, GRAPHICS_NORMAL );
   drawScore(scoreA, scoreB);
   dmd.drawBox(3,0,3,15,GRAPHICS_NORMAL);
   drawServe(serve);
}
 
void loop() {

  byte b;
  //Continue looping until you get a complete signal received
  if (myReceiver.getResults()) {
    myDecoder.decode();           //Decode it
    currentButton=String(myDecoder.value,HEX);
    if (currentButton!=buttonCodes[9] && currentButton!="0"){
      Serial.println(currentButton);
      if (currentButton==buttonCodes[0]){
        scoreA=scoreA-1;
        if (toggleM){
          serve=serve-1;
          drawServe(serve);
          toggle=true;
          toggleM=false;
        }
        Serial.println("ScoreA-");
      }
      else if (currentButton==buttonCodes[1]){
        scoreA=scoreA+1;
        toggleM=false;
        if (toggle){
          serve=serve+1;
          drawServe(serve);
          toggle=false;
          toggleM=true;
        }
        Serial.println("ScoreA+");
      }
      else if (currentButton==buttonCodes[2]){
        scoreB=scoreB-1;
        if (toggleM){
          serve=serve-1;
          drawServe(serve);
          toggle=false;
          toggleM=false;
        }
        Serial.println("scoreB-");
      }
      else if (currentButton==buttonCodes[3]){
        scoreB=scoreB+1;
        toggleM=false;
        if (!toggle){
          serve=serve+1;
          drawServe(serve);
          toggle=true;
          toggleM=true;
        }
        Serial.println("scoreB+");
      }
      else if (currentButton==buttonCodes[4]){
        setB=setB-1;
        Serial.println("SetA-");
      }
      else if (currentButton==buttonCodes[5]){
        setB=setB+1;
        Serial.println("SetA+");
      }
      else if (currentButton==buttonCodes[6]){
        setA=setA-1;
        Serial.println("SetB-");
      }
      else if (currentButton==buttonCodes[7]){
        setA=setA+1;
        Serial.println("SetB+");
      }
      else if(currentButton==buttonCodes[8]){
        setA=0;
        setB=0;
        scoreA=0;
        scoreB=0;
        toggle=false;
        toggleM=false;
        serve=1;
        drawServe(serve);
      }
      if (scoreA<0) scoreA=0;
      if (scoreB<0) scoreB=0;
      if (setA<0) setA=0; 
      if (setA>2) setA=2;
      if (setB<0) setB=0; 
      if (setB>2) setB=2;
      if (serve>4) {
        serve=1;
        drawServe(serve);
      }
      itoa (setA, setAbuff, 10);itoa (setB, setBbuff, 10);
      dmd.drawString(  setAX,  0,  setAbuff, 1, GRAPHICS_NORMAL );
      dmd.drawString(  setBX,  0, setBbuff, 1, GRAPHICS_NORMAL );
      drawScore(scoreA, scoreB);
    }
    myReceiver.enableIRIn();      //Restart receiver
  }
  
}

