// Connections 

// GPS                : gps(tx)=>ard(rx 4) && gps(rx)=>ard(tx pin3) && vcc 5v & gnd        
// Ecomapass and MPU  : SDA => ard(A4) && SCL => ard(A5) && vcc 5v & gnd
// UltraSonic HCSR04  : trigPin=>9  echoPin=>10


// GPS                => serial port 
// Ecompass and MPU   => I2C
//Ultrasonic          => serial port


#include <Arduino.h>
#include <Wire.h>
#include <HMC5883L_Simple.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

char delimeter = '|';

//GPS
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// Create a compass
HMC5883L_Simple Compass;
const int MPU_ADDR = 0x68; // MPU-6500 I2C address

//ultrasonic
const int trigPin = 9;
const int echoPin = 10;
long duration;
int distance;

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);  //gps baud
  Wire.begin(); // initialize I2C bus
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to 0 to wake up MPU-6500

  //ultrasonic
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  //Ecompass
  Compass.SetDeclination(-0, 13, 'W');
  Compass.SetSamplingMode(COMPASS_SINGLE);
  Compass.SetScale(COMPASS_SCALE_088);
  Compass.SetOrientation(COMPASS_HORIZONTAL_X_NORTH);

  Wire.endTransmission(true);
}

void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true); // request 14 bytes from MPU-6500
  int16_t ax = Wire.read() << 8 | Wire.read(); // accelerometer X-axis
  int16_t ay = Wire.read() << 8 | Wire.read(); // accelerometer Y-axis
  int16_t az = Wire.read() << 8 | Wire.read(); // accelerometer Z-axis
  int16_t gx = Wire.read() << 8 | Wire.read(); // gyroscope X-axis
  int16_t gy = Wire.read() << 8 | Wire.read(); // gyroscope Y-axis
  int16_t gz = Wire.read() << 8 | Wire.read(); // gyroscope Z-axis

  //Ecompass
  float heading = Compass.GetHeadingDegrees();

  //GPS
  // while (ss.available() > 0)
  //   if (gps.encode(ss.read()))
  displayInfo();

  // if (millis() > 5000 && gps.charsProcessed() < 10)
  // {
  //   Serial.print(-11111);
  //   Serial.print(delimeter);
  //   // while(true);
  // }

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor

  Serial.print(ax);
  Serial.print(delimeter);
  Serial.print(ay);
  Serial.print(delimeter);
  Serial.print(az);
  Serial.print(delimeter);
  Serial.print(gx);
  Serial.print(delimeter);
  Serial.print(gy);
  Serial.print(delimeter);
  Serial.print(gz);
  Serial.print(delimeter);
  Serial.print(heading);
  Serial.print(delimeter);
  Serial.print(distance);
  Serial.print(delimeter);
  delay(3000); // wait 1 second before reading again
}


//GPS

void displayInfo()
{
  // Serial.print(F("  Location:  ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    Serial.print(delimeter);
  }
  else
  {
    Serial.print(-11111);
    Serial.print(delimeter);
  }

  // Serial.print(F("  Date/Time: "));
  // if (gps.date.isValid())
  // {
  //   Serial.print(gps.date.month());
  //   Serial.print(F("/"));
  //   Serial.print(gps.date.day());
  //   Serial.print(F("/"));
  //   Serial.print(gps.date.year());
  //   Serial.print(delimeter);
  // }
  // else
  // {
  //   Serial.print(F("  INVALID  "));
  // }

  // Serial.print(F(" "));
  // Serial.print(delimeter);
  // if (gps.time.isValid())
  // {
  //   if (gps.time.hour() < 10) Serial.print(F("0"));
  //   Serial.print(gps.time.hour());
  //   Serial.print(F(":"));
  //   if (gps.time.minute() < 10) Serial.print(F("0"));
  //   Serial.print(gps.time.minute());
  //   Serial.print(F(":"));
  //   if (gps.time.second() < 10) Serial.print(F("0"));
  //   Serial.print(gps.time.second());
  //   Serial.print(F("."));
  //   if (gps.time.centisecond() < 10) Serial.print(F("0"));
  //   Serial.print(gps.time.centisecond());
  //   Serial.print(delimeter);
  // }
  // else
  // {
  //   Serial.print(F("  INVALID  "));
  // }

  // Serial.println();
}
