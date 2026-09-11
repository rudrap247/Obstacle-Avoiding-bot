//Declaration of variables related to Ultrasonic Sensor
int trig_p = 7;
int echo_p = 8;
long duration = 0;
int distance = 0;
float sound_speed = 0.0343;//in cm / micro sec

//declaration of variables related to servo motor
#include <Servo.h>
Servo myservo;  // create Servo object to control a servo
int pos = 90;// variable to store the servo position

void distance_calculation()
{
  digitalWrite(trig_p, LOW);
  delayMicroseconds(2);

  digitalWrite(trig_p, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_p, LOW);

  duration = pulseIn(echo_p, HIGH);//inbuilt function that measures time taken to recieve sound

  distance = (sound_speed * duration)/2;//formula to calculate lenght based on speed and time

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  delay(500);//delay between serial monitor readings
}

void initial_best_distance() // to measure best initial distance from start time
{
  int pos_left = 0;     //reversed servo mounting: 0 is left
  int pos_center = 90;  //90 is straight
  int pos_right = 180;  //180 is right

  int left_initial_dist = 0;
  int center_initial_dist = 0;
  int right_initial_dist = 0;

  //scan Left
  myservo.write(pos_left);
  delay(300);
  distance_calculation();
  left_initial_dist = distance;

  //scan center
  myservo.write(pos_center);
  delay(300);
  distance_calculation();
  center_initial_dist = distance;

  //scan right
  myservo.write(pos_right);
  delay(300);
  distance_calculation();
  right_initial_dist = distance;

  //reset head to center
  myservo.write(pos_center);

  //to find best initial angle
  int best_angle = pos_center; //default to straight 

  if (left_initial_dist > center_initial_dist && left_initial_dist > right_initial_dist) 
  {
    best_angle = pos_left;   // 0 degrees
  } 
  else if (right_initial_dist > center_initial_dist && right_initial_dist > left_initial_dist) 
  {
    best_angle = pos_right;  // 180 degrees
  }

}
void servo_control()
{
  delay(500);
  myservo.write(pos);
  delay(200);
  if(distance < 12)//arbitrary 12 cm least distance parameter
  {
    pos = 0;//to scan left
    myservo.write(pos);
    distance_calculation();
    delay(200);
    if(distance < 12)
    {
      pos = 180;//to scan right
      myservo.write(pos);
      distance_calculation();
    }

  }
  else
  {
    pos = 90;
    myservo.write(pos);
  }
}
void setup() {
  Serial.begin(9600);
  pinMode(trig_p, OUTPUT);//trig pin sends out ultrasonic waves
  pinMode(echo_p, INPUT);//echo pin recieves ultrasonic waves
  myservo.attach(9);  // attaches the servo on pin 9 to the Servo object
  initial_best_distance();
}

void loop() {
  distance_calculation();
  servo_control();
}
