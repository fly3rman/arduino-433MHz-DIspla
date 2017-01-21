#include <VirtualWire.h>  
#include <string.h>
#undef int
#undef abs
#undef double
#undef float
#undef round
// Das ist der Empfänger
char message[32];
float voltsCell[3];
int x;

void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.begin(9600);
  Serial.println("Hello, i am the receiver");
  vw_set_ptt_inverted(true);
  vw_set_rx_pin(12);
  vw_setup(4000);
  vw_rx_start();

}

int     commaPosition;
void loop()
{
  //  char *record1 = "one,bla,3.2;";
  //  char* Buffer[20];
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen))
  {
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


