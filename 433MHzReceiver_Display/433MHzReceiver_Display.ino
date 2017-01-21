#include "U8glib.h"
#include <stdlib.h>
#include <Average.h>
#include <VirtualWire.h>  
#include <string.h>
#undef int
#undef abs
#undef double
#undef float
#undef round
char message[32];
float voltsCell[3];
int x;
unsigned long timeout=0;
unsigned long currentMillis=0;

U8GLIB_ST7920_128X64_4X u8g(11, 10, 9);	// Display initialization with SPI Com: SCK = en = 13, MOSI = rw = 12, CS = di = 11

float randNumber1;
float randNumber2;
float randNumber3;
int randNumberChart;
//char* randNumber1Str;
String c1String,c2String,c3String,cAllString;
char randNumber1Str[10];
char randNumber2Str[10];
char randNumber3Str[10];

char cAllCharArray[14];
int lastval;
int commaPosition;

int i;
int boxbla = 0;
int chartArray[128];//127+1
//averaging
float history_array[10];
float average;

//delay ohne wait_ms
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
const long interval = 300;  
const long interval2 = 500;  



int myRand(int low, int high) {
   return rand() % (high - low + 1) + low;
}

void draw3(void) // just blink a box
{
  if (boxbla==0){
    u8g.drawBox(10,12,20,30);
    boxbla=1;
  } else {
    boxbla=0;
  }
}

void draw2(void)
{
  lastval = lastval + myRand(0,1)-myRand(0,1); //random generator for the values: -1,0,1
  lastval=constrain(lastval, 0, 37);
  chartArray[126] = lastval;
  memmove(chartArray-1, chartArray, sizeof(chartArray)-sizeof(chartArray[0]));

  for (i = 0; i < 128; i = i + 1) {
    u8g.drawPixel(i,chartArray[i]);
  }
  
}

void draw(void) {
  empfang();
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_helvR08);

  //average = rollingAverage(history_array, 10, voltsCell[0]);
  c1String = "#1: "+ String(voltsCell[0]);
  c1String.toCharArray(randNumber1Str, 10);
  

  c2String = "#2: "+ String(voltsCell[1]);
  c2String.toCharArray(randNumber2Str, 10);


  c2String = "#3: "+ String(voltsCell[2]);
  c2String.toCharArray(randNumber3Str, 10);
  
  cAllString = "#ALL: " + String(voltsCell[0] + voltsCell[1] + voltsCell[2]);
  cAllString.toCharArray(cAllCharArray, 14);
  
  u8g.drawStr( 0, 50, randNumber1Str);
  u8g.drawStr( 0, 64, randNumber2Str);
  u8g.drawStr( 65, 49, randNumber3Str);
  u8g.drawStr( 65, 63, cAllCharArray);
  u8g.drawLine(0, 38, 128, 38);
}

void setup(void) {
 
  randomSeed(analogRead(0));
  pinMode(12, OUTPUT); // spannung von 5v für empfänger
  digitalWrite(12, HIGH); // spannung von 5v für empfänger
  Serial.begin(115200);
  Serial.println("Hello, i am the receiver");
  vw_set_ptt_inverted(true);
  vw_set_rx_pin(13);
  vw_setup(4000);
  vw_rx_start();
}

void empfang(){
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

 if (vw_get_message(buf, &buflen))
  {
    timeout = millis();
    for (int i=0; i<buflen;i++){
      //Serial.print(char(buf[i])); //print whatever is there

      if (buf[i] == ';')
      {
        x=0;
        char* chars_array = strtok(message, ",");
        while(chars_array)
        {
          voltsCell[x] = atof(chars_array);
         
          chars_array = strtok(NULL, ",");
          x=x+1;
        }
        x=0;
        Serial.println("Cell 1:      Cell 2:     Cell 3:");
        for(int x=0;x <= 2;x++){
           Serial.print(voltsCell[x],3);
           Serial.print("         ");
        } 
        Serial.println(" ");
        Serial.println(" ");
      }
      else{
        message[i] = char(buf[i]);
      }  
    }
  }
}


void loop(void) {

  // picture loop  
  currentMillis = millis();
    u8g.firstPage(); 
    if(currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis; 
      do {
        draw();
        if(currentMillis - timeout >= 5000) {
           Serial.println(currentMillis - timeout);
           u8g.drawStr( 120, 49, "!!!");  //No Data since 5 Seconds, Timeout!
         }
         draw2(); //ampermeter
        
       }while( u8g.nextPage() );
  }
  
  
  
  // rebuild the picture after some delay
   //delay(150);
}


