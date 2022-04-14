
#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <iomanip>
#include <wiringPi.h>
#include <unistd.h>
using namespace std;

#define HEX(x) setw(2) << setfill('0') << hex << (int)(x)

#define DEVID 0x00
#define POWER_CTL 0x2D
#define DATA_FORMAT 0x31
#define DATAX0 0x32
#define DATAX1 0x33
#define DATAY0 0x34
#define DATAY1 0x35
#define DATAZ0 0x36
#define DATAZ1 0x37
#define BUFFER_SIZE 0x40
#define LED_R 27
#define LED_G 20
#define LED_B 21
unsigned char dataBuffer[BUFFER_SIZE];

int writeRegister(int file, unsigned char address, char value)
{
   unsigned char buffer[2];
   buffer[0] = address;
   buffer[1] = value;
   if (write(file, buffer, 2) != 2)
   {
      cout << "Failed write to the device" << endl;
      return 1;
   }
   return 0;
}

int readRegisters(int file)
{
   writeRegister(file, 0x00, 0x00);
   if (read(file, dataBuffer, BUFFER_SIZE) != BUFFER_SIZE)
   {
      cout << "Failed to read in the full buffer." << endl;
      return 1;
   }
   if (dataBuffer[DEVID] != 0xE5)
   {
      cout << "Problem detected! Device ID is wrong" << endl;
      return 1;
   }
   return 0;
}

short combineValues(unsigned char msb, unsigned char lsb)
{
   return ((short)msb << 8) | (short)lsb;
}

int main()
{
   wiringPiSetupGpio();
   pinMode(LED_R, OUTPUT);
   pinMode(LED_G, OUTPUT);
   pinMode(LED_B, OUTPUT);
   int file;
   cout << "Starting the ADXL345 sensor application" << endl;
   if ((file = open("/dev/i2c-1", O_RDWR)) < 0)
   {
      cout << "failed to open the bus" << endl;
      return 1;
   }
   if (ioctl(file, I2C_SLAVE, 0x53) < 0)
   {
      cout << "Failed to connect to the sensor" << endl;
      return 1;
   }
   writeRegister(file, POWER_CTL, 0x08);
   // Setting mode to 00000000=0x00 for +/-2g 10-bit
   // Setting mode to 00001011=0x0B for +/-16g 13-bit
   writeRegister(file, DATA_FORMAT, 0x00);
   readRegisters(file);
   cout << "The Device ID is: " << HEX(dataBuffer[DEVID]) << endl;
   cout << "The POWER_CTL mode is: " << HEX(dataBuffer[POWER_CTL]) << endl;
   cout << "The DATA_FORMAT is: " << HEX(dataBuffer[DATA_FORMAT]) << endl;
   cout << dec << endl; // reset back to decimal

   // Now loop a display the x, y, z accelerometer for 60 seconds
   int count = 0;
   while (count < 60)
   {
      short x = combineValues(dataBuffer[DATAX1], dataBuffer[DATAX0]);
      short y = combineValues(dataBuffer[DATAY1], dataBuffer[DATAY0]);
      short z = combineValues(dataBuffer[DATAZ1], dataBuffer[DATAZ0]);
      // Use \r and flush to write the output on the same line
      cout << "X=" << x << " Y=" << y << " Z=" << z << " sample=" << count << "     \r" << flush;
      if (x > 240 || x < -240)
      {
         digitalWrite(LED_R, HIGH);
         digitalWrite(LED_G, LOW);
         digitalWrite(LED_B, LOW);
      }
      else if (y > 250 || y < -250)
      {
         digitalWrite(LED_G, HIGH);
         digitalWrite(LED_R, LOW);
         digitalWrite(LED_B, LOW);
      }
      else if (z > 250 || z < -250)
      {
         digitalWrite(LED_B, HIGH);
         digitalWrite(LED_R, LOW);
         digitalWrite(LED_G, LOW);
      }
      else
      {
         digitalWrite(LED_R, LOW);
         digitalWrite(LED_G, LOW);
         digitalWrite(LED_B, LOW);
      }
      usleep(1000000);
      readRegisters(file); // read the sensor again
      count++;
   }
   close(file);
   return 0;
}