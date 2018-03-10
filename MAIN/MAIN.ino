#include <MPU9250_RegisterMap.h>
#include <SparkFunMPU9250-DMP.h>
//--------------------------------------------------------------------------------------------------------------------------------------
#include "DS3231.h" // подключаем часы реального времени
#include "LoRa.h" // LoRa
#include "Ublox.h" // Ublox
//--------------------------------------------------------------------------------------------------------------------------------------
MPU9250_DMP imu; // наш MPU9250
//--------------------------------------------------------------------------------------------------------------------------------------
#define GPS_BAUD 9600 // скорость работы с портом GPS
#define GPS_SERIAL Serial1  // какой Serial используем для работы с GPS
Ublox M6_Gps; // наш GPS-трекер
// LoRa pin
#define csPin 8
#define resetPin 9 
#define irqPin 7

const int ledPin =  LED_BUILTIN;// the number of the LED pin
//const int ledPin =  13;         // the number of the LED pin
int ledState = LOW;             // ledState used to set the LED


// Altitude - Latitude - Longitude - N Satellites
float gpsArray[] = {0, 0, 0, 0};
//--------------------------------------------------------------------------------------------------------------------------------------
void onLoraReceive(int packetSize)
{
  if(packetSize < 1)
    return;

  // тут разбор пришедшего пакета
/*  
  // read packet
  for (int i = 0; i < packetSize; i++) 
  {
    Serial.print((char)LoRa.read());
  }
 */
}
//--------------------------------------------------------------------------------------------------------------------------------------
void setup() 
{

  Serial.begin(115200);
  GPS_SERIAL.begin(GPS_BAUD);
  
  RTCClock.begin(1); // часы на первом интерфейсе I2C (поменять на 1, если надо на второй интерфейс I2C
  pinMode(ledPin, OUTPUT);
  // Set sketch compiling time
 //RTCClock.setTime(10,5,16,5,9,2,18);
  
 DS3231Time tm = RTCClock.getTime();
 
 
  /*
  tm.dayOfMonth
  tm.month
  tm.year

  tm.hour
  tm.minute
  tm.second
*/
  String timeString = RTCClock.getTimeStr(tm);
  String dateString = RTCClock.getDateStr(tm);

  Serial.println(dateString);
  Serial.println(timeString);

  
 // LoRa.onReceive(onLoraReceive);
  
  LoRa.setPins(csPin, resetPin, irqPin);// set CS, reset, IRQ pin
  Serial.println("LoRa Receiver");

  if (!LoRa.begin(868E6)) // initialize ratio at 868 MHz
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  else
  {
      Serial.println("Starting LoRa successfully!");
  }
  
  //LoRa.receive(); // переключаемся на приём

  // Call imu.begin() to verify communication with and
  // initialize the MPU-9250 to it's default values.
  // Most functions return an error code - INV_SUCCESS (0)
  // indicates the IMU was present and successfully set up
  if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
      Serial.println("Unable to communicate with MPU-9250");
      Serial.println("Check connections, and try again.");
      Serial.println();
      delay(5000);
    }
  }

  // Use setSensors to turn on or off MPU-9250 sensors.
  // Any of the following defines can be combined:
  // INV_XYZ_GYRO, INV_XYZ_ACCEL, INV_XYZ_COMPASS,
  // INV_X_GYRO, INV_Y_GYRO, or INV_Z_GYRO
  // Enable all sensors:
  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);

  // Use setGyroFSR() and setAccelFSR() to configure the
  // gyroscope and accelerometer full scale ranges.
  // Gyro options are +/- 250, 500, 1000, or 2000 dps
  imu.setGyroFSR(2000); // Set gyro to 2000 dps
  // Accel options are +/- 2, 4, 8, or 16 g
  imu.setAccelFSR(2); // Set accel to +/-2g
  // Note: the MPU-9250's magnetometer FSR is set at 
  // +/- 4912 uT (micro-tesla's)

  // setLPF() can be used to set the digital low-pass filter
  // of the accelerometer and gyroscope.
  // Can be any of the following: 188, 98, 42, 20, 10, 5
  // (values are in Hz).
  imu.setLPF(5); // Set LPF corner frequency to 5Hz

  // The sample rate of the accel/gyro can be set using
  // setSampleRate. Acceptable values range from 4Hz to 1kHz
  imu.setSampleRate(10); // Set sample rate to 10Hz

  // Likewise, the compass (magnetometer) sample rate can be
  // set using the setCompassSampleRate() function.
  // This value can range between: 1-100Hz
  imu.setCompassSampleRate(10); // Set mag rate to 10Hz


}
//--------------------------------------------------------------------------------------------------------------------------------------
void printIMUData(void)
{  
  // After calling update() the ax, ay, az, gx, gy, gz, mx,
  // my, mz, time, and/or temerature class variables are all
  // updated. Access them by placing the object. in front:

  // Use the calcAccel, calcGyro, and calcMag functions to
  // convert the raw sensor readings (signed 16-bit values)
  // to their respective units.
  float accelX = imu.calcAccel(imu.ax);
  float accelY = imu.calcAccel(imu.ay);
  float accelZ = imu.calcAccel(imu.az);
  float gyroX = imu.calcGyro(imu.gx);
  float gyroY = imu.calcGyro(imu.gy);
  float gyroZ = imu.calcGyro(imu.gz);
  float magX = imu.calcMag(imu.mx);
  float magY = imu.calcMag(imu.my);
  float magZ = imu.calcMag(imu.mz);
  
  Serial.println("Accel: " + String(accelX) + ", " +
              String(accelY) + ", " + String(accelZ) + " g");
  Serial.println("Gyro: " + String(gyroX) + ", " +
              String(gyroY) + ", " + String(gyroZ) + " dps");
  Serial.println("Mag: " + String(magX) + ", " +
              String(magY) + ", " + String(magZ) + " uT");
  Serial.println("Time: " + String(imu.time) + " ms");
  Serial.println();
}
//--------------------------------------------------------------------------------------------------------------------------------------
void handleGPS()
{
   // работаем с GPS 

  if(!GPS_SERIAL.available())
    return;

  while(GPS_SERIAL.available())
  {
        char c = GPS_SERIAL.read();
         if (M6_Gps.encode(c)) 
         {
            gpsArray[0] = M6_Gps.altitude;
            gpsArray[1] = M6_Gps.latitude;
            gpsArray[2] = M6_Gps.longitude; 
            gpsArray[3] = M6_Gps.sats_in_use;

           for(byte i = 0; i < sizeof(gpsArray)/sizeof(gpsArray[0]); i++) 
            {
              Serial.print(gpsArray[i], 6);
              Serial.print(" ");
            }
            
            Serial.println("");         
        }
  }
 
   
}
//--------------------------------------------------------------------------------------------------------------------------------------
void handleIMU()
{
  // работаем с акселерометром

 // dataReady() checks to see if new accel/gyro data
  // is available. It will return a boolean true or false
  // (New magnetometer data cannot be checked, as the library
  //  runs that sensor in single-conversion mode.)
  if ( imu.dataReady() )
  {
    // Call update() to update the imu objects sensor data.
    // You can specify which sensors to update by combining
    // UPDATE_ACCEL, UPDATE_GYRO, UPDATE_COMPASS, and/or
    // UPDATE_TEMPERATURE.
    // (The update function defaults to accel, gyro, compass,
    //  so you don't have to specify these values.)
    imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
    printIMUData();
  }
  
}
//--------------------------------------------------------------------------------------------------------------------------------------

void testLoraReceive()
{
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");
     // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
      if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }

  
}




void loop() 
{

  
 // DS3231Time tm = RTCClock.getTime();
  /*
  tm.dayOfMonth
  tm.month
  tm.year

  tm.hour
  tm.minute
  tm.second
*/
 // String timeString = RTCClock.getTimeStr(tm);
 // String dateString = RTCClock.getDateStr(tm);
  

  /*
   // отсыл пакета в LoRa
   LoRa.beginPacket();
   LoRa.write(buffer, length);
   LoRa.endPacket();
   LoRa.receive();
   */


  handleIMU();
  handleGPS();
  testLoraReceive();

  
}
//--------------------------------------------------------------------------------------------------------------------------------------

