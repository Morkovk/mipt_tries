#include <QTRSensors.h>

#define NUM_SENSORS             6  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
#define EMITTER_PIN             13  // emitter is controlled by digital pin 2

// sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
QTRSensorsAnalog qtr((unsigned char[]) {0, 1, 2, 3, 4, 5}, 
  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR);
unsigned int sensorValues[NUM_SENSORS];

int leftMotor = 5;
int rightMotor = 6;
int leftMotorSpeed = 0;
int rightMotorSpeed = 0;


void setup()
{
  pinMode(13, OUTPUT);
   pinMode(leftMotor, OUTPUT);
   pinMode(rightMotor, OUTPUT);
   
   digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtr.calibrate();       // reads all sensors 10 times at 2.5 ms per six sensors (i.e. ~25 ms per call)
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

 
  Serial.begin(9600);
  for (int i = 0; i < NUM_SENSORS; i++)
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
 
  int position = qtr.readLine(sensors, QTR_EMITTERS_ON, 1);
 

 
 
 if (sensors[0] < 50 && sensors[1] < 50 && sensors[2] < 50 && sensors[3] < 50 && sensors[4] < 50 && sensors[5] < 50)
  {
     leftMotorSpeed = 0;
   rightMotorSpeed = 0;
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
     Serial.print("STOP");
  Serial.println();
   Serial.print(leftMotorSpeed);
   Serial.print(' ');
 Serial.print(rightMotorSpeed);
 Serial.println();
 Serial.println(); 
 digitalWrite(leftMotor, LOW);
  digitalWrite(rightMotor, LOW);
 
    delay(1000);
  }
  else {
     leftMotorSpeed = (sensors[2] + sensors[3] + sensors[4] + sensors[5]) / 4 % 255;
   rightMotorSpeed = (sensors[2] + sensors[3] + sensors[4] + sensors[5]) / 4 % 255;

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
   Serial.print("Ok");
  Serial.println();
 Serial.print(leftMotorSpeed);
 Serial.print(' ');
 Serial.print(rightMotorSpeed);
  Serial.println();
  Serial.println();
  analogWrite(leftMotor, leftMotorSpeed);
  analogWrite(rightMotor, rightMotorSpeed);
  delay(300);
  }
  /*
 
 
leftMotorSpeed = 100;
rightMotorSpeed = 100;

  int error = position - 1000;
 
  if (error < -500)  // the line is on the left
    leftMotorSpeed = 0;  // turn left
  if (error > 500)  // the line is on the right
    rightMotorSpeed = 0;  // turn right*/
 
  
  
  delay(1000);
} 
