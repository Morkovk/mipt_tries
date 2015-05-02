#include <QTRSensors.h>
#include <Serial.h>

#define NUM_SENSORS             6  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
#define EMITTER_PIN             13  // emitter is controlled by digital pin 2 


// sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
QTRSensorsAnalog qtr((unsigned char[]) {1, 2, 3, 4, 5, 6}, 
  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR);
unsigned int sensorValues[NUM_SENSORS];

int leftMotor = 5;
int rightMotor = 6;
int leftMotorSpeed = 0;
int rightMotorSpeed = 0;

float Kd = 0, Ki = 0, Kp = 0.0500;
float PrevEr, error, I, D, d;
float PID;
float BorderValue = 50;


void setup()
{
  
  
  Serial.begin(9600); 
  pinMode(13, OUTPUT);
  pinMode(leftMotor, OUTPUT);
  pinMode(rightMotor, OUTPUT);
  
  Serial.print("hello");
   
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtr.calibrate();       // reads all sensors 10 times at 2.5 ms per six sensors (i.e. ~25 ms per call)
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  int i = 0;
  for (i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtr.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  

  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtr.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
}


void loop()
{
  
  unsigned int sensors[6];
  
 qtr.readCalibrated(sensors);
 
 int position = qtr.readLine(sensors, QTR_EMITTERS_ON, 0);
 int l = (sensors[0] + sensors[1] + sensors[2]);
 int r = (sensors[3] + sensors[4] + sensors[5]);
 error = 2400 - position;
 Serial.print(error);
 Serial.println();
 

 d = error - PrevEr;
 D = Kd * d;
 PrevEr = error;
 I = (I + error) * Ki;
 PID = D + I + error * Kp;

 Serial.print(d);
 Serial.println();
 Serial.print(I);
 Serial.println();
 Serial.print(PID);
 Serial.println();
 
 
 if (PID > BorderValue){
   rightMotorSpeed = (int)PID;
   leftMotorSpeed = 0;
 }
 else if (PID < -BorderValue){
   leftMotorSpeed = (int)(-PID);
   rightMotorSpeed = 0;
 }
 else if (PID < BorderValue && PID > -BorderValue)
   leftMotorSpeed = rightMotorSpeed = 100;
   
      Serial.print(sensors[0]);
      Serial.print(' ');
      Serial.print(sensors[1]);
      Serial.print(' ');
      Serial.print(sensors[2]);
      Serial.print(' ');
      Serial.print(sensors[3]);
      Serial.print(' ');
      Serial.print(sensors[4]);
      Serial.print(' ');
      Serial.print(sensors[5]);
      Serial.print(' ');
      Serial.println();
      Serial.print(leftMotorSpeed);
      Serial.print(' ');
      Serial.print(rightMotorSpeed);
     Serial.println(); 
        analogWrite(leftMotor, leftMotorSpeed);
        analogWrite(rightMotor, rightMotorSpeed);
      delay(1000);
} 
