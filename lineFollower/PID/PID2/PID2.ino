#include <PID_v1.h>
#include <QTRSensors.h>
#include <Serial.h>

#define NUM_SENSORS             6  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
#define EMITTER_PIN             13  // emitter is controlled by digital pin 2 


// sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
QTRSensorsAnalog qtr((unsigned char[]) {
  1, 2, 3, 4, 5, 6}
, 
NUM_SENSORS, NUM_SAMPLES_PER_SENSOR);
unsigned int sensorValues[NUM_SENSORS];

int leftMotor = 5;
int rightMotor = 6;
int leftMotorSpeed = 0;
int rightMotorSpeed = 0;

const float Ki = 0, Kd = 0.05, Kp = 0.1;


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


int CalculatePID(int error, float Ki, float Kp, float Kd){
  int D, PrevEr, I;
  float PID;
  D = error - PrevEr;
  PrevEr = error;
  I = (I + error);
  PID = D* Kd + I * Ki + error * Kp;


   Serial.print("proportional add is ");
  Serial.print(error*Kp);
  Serial.println();
  Serial.print(D);
  Serial.println();
  Serial.print("derivative add is ");
  Serial.print(Kd*D);
  Serial.println();
  Serial.print(I);
  Serial.println();
  Serial.print(PID);
  Serial.println();
  
  return (int)PID;
}

void CalcSpeed(int PID, int *rightSpeed, int *leftSpeed){
    rightMotorSpeed += PID;
    rightMotorSpeed -= PID;
    
    if (leftMotorSpeed < 0) leftMotorSpeed = 0;
    if (rightMotorSpeed < 0) rightMotorSpeed = 0;
}

void PrintAll(int const position, const int error, const int leftMotorSpeed, const int rightMotorSpeed, const int PID, unsigned int *sensors){
  Serial.println();
  Serial.print("error is   ");
  Serial.print(error);
  Serial.println();
  
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

}


void loop()
{

  unsigned int sensors[6];

  qtr.readCalibrated(sensors);

  int position = qtr.readLine(sensors, QTR_EMITTERS_ON, 0);
  int  error = position - 2400;

  leftMotorSpeed = rightMotorSpeed = 100;  
  
  int  PID = CalculatePID(error, Ki, Kp, Kd);
  
  CalcSpeed(Pid, &leftMotorSpeed, &rightMotorSpeed);
  
  PrintAll(position, error, leftMotorSpeed, rightMotorSpeed, PID, sensors);
  
  
  analogWrite(leftMotor, leftMotorSpeed);
  analogWrite(rightMotor, rightMotorSpeed);
  //delay(1000);
} 


