//#include <Metro.h>
#include <VirtualWire.h>
#include<stdlib.h>


// Das ist der Sender

char messageChars[30];
String message;
char SerialReadString;
byte messageLength;
const float resistorFactorCell1 = 1023; 
const float resistorFactorCell2 = 510.95;  
const float resistorFactorCell3 = 326.77; 
const byte CellPin1 = 1;      
const byte CellPin2 = 2;   
const byte CellPin3 = 3;   
float referenceVolts = 5.000;   
float voltsCell1,voltsCell2,voltsCell3;
unsigned long previousMillisSend=0, previousMillisBlink1=0,previousMillisBlink2=0;  
const byte led1G =  8;
byte led1GState = 0;
//Metro led1GMetro = Metro(1000); 
//Metro transmitDataMetro = Metro(1000); 

//temp
unsigned long previousMillisTransmit = 0,previousMillisBlink=0;        // will store last time LED was updated
unsigned long currentMillis;

int intervalBlink = 1000;           // interval at which to blink (milliseconds)
int intervalTransmit = 1000;  


String measureLipo()
{
  voltsCell1= analogRead(CellPin1)/resistorFactorCell1 * referenceVolts ; 
  voltsCell2= analogRead(CellPin2)/resistorFactorCell2 * referenceVolts - voltsCell1;
  voltsCell3= analogRead(CellPin3)/resistorFactorCell3 * referenceVolts - voltsCell1 - voltsCell2;
  String measureLipoString = String(voltsCell1,3)+(",")+String(voltsCell2,3)+(",")+String(voltsCell3,3)+(";");
  Serial.print("ref voltage:"); 
  Serial.println(referenceVolts,4);
  Serial.print("voltages:"); 
  Serial.println(measureLipoString);
  return measureLipoString;
}

long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return result;
}

int blinkLed(){
   switch(led1GState){
      case(3):
          led1GState = 0;
          digitalWrite(led1G,0);
          intervalBlink= 1000;
          break;
      case(0):
          led1GState = 1;
          digitalWrite(led1G,1);
          intervalBlink= 80;
          break;
     case(1):
          led1GState = 2;
          digitalWrite(led1G,0);
          break;
     case(2):
          led1GState = 3;
          digitalWrite(led1G,1);
          break;
      }
  return 0;
}

void transmitData(){
    message=measureLipo();
    Serial.print("try to send: ");
    Serial.println(message);
    messageLength = message.length()+1;//because of string termination character
    message.toCharArray(messageChars, messageLength);
    vw_send((uint8_t *)&messageChars, strlen(messageChars)); //todo: länge evntl falsch, rausfinden was erg genau haben will
    vw_wait_tx();  
    Serial.println("roger.");    
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello, i am the transmitter");
  vw_set_ptt_inverted(true);
  vw_set_tx_pin(10);
  vw_setup(4000);
  pinMode(led1G, OUTPUT);
  digitalWrite(led1G, 0);
  referenceVolts = float(readVcc())/1000;
}



void loop()
{
  currentMillis = millis();
   //problem mit zwei if anweisungen hier!
  if(currentMillis - previousMillisBlink >= intervalBlink) {  // check if the metro has passed its interval .
    previousMillisBlink = currentMillis;   
    blinkLed();
  }
  if (currentMillis - previousMillisTransmit >= intervalTransmit) {
    previousMillisTransmit = currentMillis;  
    transmitData();
  } 
    //previousMillisSend = currentMillis;  
//  unsigned long currentMillis = millis();
//   if(currentMillis - previousMillisSend >= 2000) {
//   transmitData();
//   }
//  if(currentMillis - previousMillisBlink1 >= 1000) {
//    digitalWrite(led1G, 1);
//    previousMillisBlink1 = currentMillis; 
//  }
//  if(currentMillis - previousMillisBlink2 >= 1060) {
//    digitalWrite(led1G, 0);
//    previousMillisBlink2 = currentMillis; 
//  }
}
