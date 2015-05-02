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

   


void setup()
{
  
  
  Serial.begin(9600); 
  pinMode(13, OUTPUT);
  pinMode(leftMotor, OUTPUT);
  pinMode(rightMotor, OUTPUT);
   
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
 
 
 /* ((sensors[0] > 800 || sensors[1] > 800)  && (sensors[4] > 800 || sensors[5] > 800))
 {
          leftMotorSpeed = 0;
          rightMotorSpeed = 0;
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
          Serial.print("STOP");
          Serial.println();
          Serial.print(leftMotorSpeed);
          Serial.print(' ');
          Serial.print(rightMotorSpeed);
          Serial.println();
          Serial.println(); 
          digitalWrite(leftMotor, LOW);
          digitalWrite(rightMotor, LOW);
       
          //delay(1000);
  }*/
if(sensors[0] < 20 && sensors[1] < 20 && sensors[2] < 20 && sensors[3] < 20 && sensors[4] < 20 && sensors[5] < 20){}
         /* leftMotorSpeed = 0;
          rightMotorSpeed = 0;
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
          Serial.print("STOP");
          Serial.println();
          Serial.print(leftMotorSpeed);
          Serial.print(' ');
          Serial.print(rightMotorSpeed);
          Serial.println();
          Serial.println(); 
          digitalWrite(leftMotor, LOW);
          digitalWrite(rightMotor, LOW);
 }*/
  else {
      leftMotorSpeed = 150;
      rightMotorSpeed = 150;
      
   Serial.print(position);  
   Serial.println(); 

  if (position  > 2800){  // the line is on the left{
    leftMotorSpeed = 50;  // turn right*/
    rightMotorSpeed = 0;  // turn lef
  }

  if (position < 2000){  // the line is on the right{
    rightMotorSpeed = 50;  // turn right*/
    leftMotorSpeed = 0;  // turn lef
  }
        
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
          Serial.print(position);
          Serial.println();
          Serial.println();
          analogWrite(leftMotor, leftMotorSpeed);
          analogWrite(rightMotor, rightMotorSpeed);
  }
} 
