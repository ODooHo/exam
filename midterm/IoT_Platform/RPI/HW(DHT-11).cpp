#include<iostream>
#include<unistd.h>
#include<wiringPi.h>
#include<iomanip>
using namespace std;

#define USING_DHT11      true   // The DHT11 uses only 8 bits
#define DHT_GPIO         05      // Using GPIO 22 for this example
#define LH_THRESHOLD     26      // Low=~14, High=~38 - pick avg.
#define RGB_R            16
#define RGB_G            20
#define RGB_B		 21

int main(){
   int humid = 0, temp = 0;
   cout << "Starting the one-wire sensor program" << endl;  
TRYAGAIN: 
   wiringPiSetupGpio();
   piHiPri(99);                       // If checksum fails (come back here)
   unsigned char data[5] = {0,0,0,0,0};
   pinMode(RGB_R,OUTPUT);
   pinMode(RGB_G,OUTPUT);
   pinMode(RGB_B,OUTPUT);
   pinMode(DHT_GPIO, OUTPUT);                 // gpio starts as output
   digitalWrite(DHT_GPIO, LOW);               // pull the line low
   usleep(18000);                             // wait for 18ms
   digitalWrite(DHT_GPIO, HIGH);              // set the line high
   pinMode(DHT_GPIO, INPUT);                  // now gpio is an input

   // need to ignore the first and second high after going low
   do { delayMicroseconds(1); } while(digitalRead(DHT_GPIO)==HIGH);
   do { delayMicroseconds(1); } while(digitalRead(DHT_GPIO)==LOW);
   do { delayMicroseconds(1); } while(digitalRead(DHT_GPIO)==HIGH);
   // Remember the highs, ignore the lows -- a good philosophy!
   for(int d=0; d<5; d++) {       // for each data byte
      // read 8 bits
      for(int i=0; i<8; i++) {    // for each bit of data
         do { delayMicroseconds(1); } while(digitalRead(DHT_GPIO)==LOW);
         int width = 0;           // measure width of each high
         do {
            width++;
            delayMicroseconds(1);
            if(width>1000) break; // missed a pulse -- data invalid!
         } while(digitalRead(DHT_GPIO)==HIGH);    // time it!
         // shift in the data, msb first if width > the threshold
         data[d] = data[d] | ((width > LH_THRESHOLD) << (7-i));
      }
   }
   if (USING_DHT11){
      humid = data[0] * 10;            // one byte - no fractional part
      temp = data[2] * 10;             // multiplying to keep code concise
   }
   else {                              // for DHT22 (AM2302/AM2301)
      humid = (data[0]<<8 | data[1]);  // shift MSBs 8 bits left and OR LSBs
      temp = (data[2]<<8 | data[3]);   // same again for temperature
   }
   unsigned char chk = 0;   // the checksum will overflow automatically
   for(int i=0; i<4; i++){ chk+= data[i]; }
   if(chk==data[4]){
      cout<<(float)temp/10<<endl;
      if(((float)temp/10)>26){
	digitalWrite(RGB_R,HIGH);
	digitalWrite(RGB_B,LOW);
	digitalWrite(RGB_G,LOW);
      }
      else{
	digitalWrite(RGB_R,LOW);
	digitalWrite(RGB_B,HIGH);
	digitalWrite(RGB_G,LOW);
      }
      usleep(2000000);
      goto TRYAGAIN;
   }
   else {
      cout << "Checksum bad - data error - trying again!" << endl;
      usleep(2000000);   // have to delay for 1-2 seconds between readings
      goto TRYAGAIN;     // a GOTO!!! call yourself a C/C++ programmer!
   }
   return 0;
}
