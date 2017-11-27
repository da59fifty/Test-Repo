#include <Servo.h>
//accelerometer stuff
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>  // not used in this demo but required!

#define LSM9DS1_SCK A5
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5

const int servoPin1 = 2;
const int servoPin2 = 8;
const int buttonpin = 5;
const int modepin = 7;
const int onoffpin = 11;
const int ledpin = 13;

int buttonstate = 0;
int modestate = 0;
int onoffstate = 0;

int pos = 0; //0 = closed, 1 = open;
int rotclosed = 6;
int rotopen = 66;
int testrot = 10;
int posright = rotclosed;
int posleft = 180 - rotclosed;

float latg;
float longg;
int vspeed; // vehicle speed

float latglimit = .4;   // g's
float longglimit = .1; //g's, 
int vspeedlimit = 20; // max speed where vehicle is traction limited...in units?

Servo Servoright;
Servo Servoleft;

// i2c
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

void setupSensor()
{
  // 1.) Set the accelerometer range
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_8G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);
  
  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_8GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_12GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);

  // 3.) Setup the gyroscope
 // lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_500DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);
}

// end accelerometer stuff
void setup() 
{
  Serial.begin(9600);

  Servoright.attach(servoPin1);
  Servoleft.attach(servoPin2);

  pinMode(buttonpin, INPUT);
  pinMode(modepin, INPUT);
  pinMode(ledpin, OUTPUT);

  //left side has opposite rotation
  Servoright.write(posright);
  Servoleft.write(posleft);

  delay(1000);
  posright = rotclosed + testrot;   //inital test begin
  posleft = 180 - (rotclosed + testrot);

  Servoright.write(posright);
  Servoleft.write(posleft);

  delay(500);

  posright = rotclosed;
  posleft = 180 - rotclosed;

  Servoright.write(posright);
  Servoleft.write(posleft);    //initial test end

  while (!Serial) 
  {
    delay(1); // will pause Zero, Leonardo, etc until serial console opens
  }
  
  Serial.println("LSM9DS1 data read demo");
  
  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }
  Serial.println("Found LSM9DS1 9DOF");

  // helper to just set the default scaling we want, see above!
  setupSensor();
}

void loop() 
{
  buttonstate = digitalRead(buttonpin);
  modestate = digitalRead(modepin);
  onoffstate = digitalRead(onoffpin);

  //get accelerometer data
  lsm.read();  /* ask it to read in the data */ 

  /* Get a new sensor event */ 
  sensors_event_t a, m, g, temp;

  lsm.getEvent(&a, &m, &g, &temp); 

  //Serial.print("Accel X: "); Serial.print(a.acceleration.x); Serial.print(" m/s^2");
  //Serial.print("\tY: "); Serial.print(a.acceleration.y);     Serial.print(" m/s^2 ");
  // Serial.print("\tZ: "); Serial.print(a.acceleration.z);     Serial.println(" m/s^2 ");

  // Serial.print("Mag X: "); Serial.print(m.magnetic.x);   Serial.print(" gauss");
  //Serial.print("\tY: "); Serial.print(m.magnetic.y);     Serial.print(" gauss");
  // Serial.print("\tZ: "); Serial.print(m.magnetic.z);     Serial.println(" gauss");

  //Serial.print("Gyro X: "); Serial.print(g.gyro.x);   Serial.print(" dps");
  //Serial.print("\tY: "); Serial.print(g.gyro.y);      Serial.print(" dps");
  // Serial.print("\tZ: "); Serial.print(g.gyro.z);      Serial.println(" dps");

  // Serial.println();
  // delay(200);

  // end get accelerometer data
  latg = a.acceleration.y / 9.8;
  longg = a.acceleration.x / 9.8;

  posright = rotclosed;
  posleft = 180 - rotclosed;

  vspeed = 30; //just for testing

  if (modestate == HIGH) //push button mode
  { 
    Serial.println("HIGH");
    if (buttonstate == HIGH) //high = on
    {
      posright = rotopen;
      posleft = 180 - rotopen;
  }

}// end push button mode
else if (modestate == LOW) // automatic mode
{ 
  latg = a.acceleration.y / 9.8;
  longg = a.acceleration.x / 9.8;

  Serial.println("LOW");

  if (abs(latg) < latglimit and vspeed > vspeedlimit and longg > longglimit)
  {
    posright = rotopen;
    posleft = 180 - rotopen;
    Serial.println("OPEN");
  }
}// end automatic mode

if (onoffstate == LOW) //drs is off
{
  posright = rotclosed;
  posleft = 180 - rotclosed;
}

if (posright == rotopen)
{
  ledpin == HIGH;
}

Servoright.write(posright);
Servoleft.write(posleft);

Serial.println(latg);
Serial.println(longg);

delay(10);

}//end void loop
