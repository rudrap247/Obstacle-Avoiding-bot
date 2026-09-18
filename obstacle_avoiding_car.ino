//Declaration of variables related to Ultrasonic Sensor
int trig_p = 7;//trigger pin
int echo_p = 8;//echo pin
long duration = 0;//time taken to recieve signal by ultrasonic sensor
int distance = 0;//distance measured from ultrasonic sensor
int least_distance = 12;//arbitrary 12 cm least distance parameter
int scan_time = 300;//safe time for which ultrasonic sensor can obtain reading
float sound_speed = 0.0343;//in cm / micro sec

//declaration of variables related to servo motor
#include <Servo.h>
Servo myservo;  // create Servo object to control a servo
int pos_left = 0;// variable to store the left servo position
int pos_center = 90;// variable to store the center servo position
int pos_right = 180;// variable to store the right servo position

//declaring motor pins

//left motor pins
int ENA =5;
int IN1 = 2;
int IN2 = 4;

//right motor pins
int ENB = 6;
int IN3 = 12;
int IN4 = 13;

//analog values for speed
int forward_speed = 85;
int turn_speed = 120;//higher than forward as turning requires more torque


int distance_calculation()
{
  digitalWrite(trig_p, LOW);
  delayMicroseconds(2);

  digitalWrite(trig_p, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_p, LOW);

  duration = pulseIn(echo_p, HIGH);//inbuilt function that measures time taken to recieve sound

  distance = (sound_speed * duration)/2;//formula to calculate distance based on speed and time

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  return distance;
}

void move_forward()
{
  //both motor move in same forward direction
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, forward_speed); 
  analogWrite(ENB, forward_speed);
}

void turn_left()
{
  //to turn left right wheel moves forward and left wheel moves backwards
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, turn_speed); 
  analogWrite(ENB, turn_speed);
  delay(500);
}

void turn_right()
{
  //to turn right left wheel moves forward and right wheel moves backwards
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, turn_speed); 
  analogWrite(ENB, turn_speed);
  delay(500);
}

void stop_car()
{
  //no current goes to motor hence car stops
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); 
  analogWrite(ENB, 0);
  delay(1000);
}

void initial_best_distance() // to measure best initial distance from start time
{
  int left_initial_dist = 0;
  int center_initial_dist = 0;
  int right_initial_dist = 0;

  //scan Left
  myservo.write(pos_left);
  delay(scan_time);
  left_initial_dist = distance_calculation();

  //scan center
  myservo.write(pos_center);
  delay(scan_time);
  center_initial_dist = distance_calculation();

  //scan right
  myservo.write(pos_right);
  delay(scan_time);
  right_initial_dist = distance_calculation();

  myservo.write(pos_center);//reset to center

  if (left_initial_dist > center_initial_dist && left_initial_dist > right_initial_dist) 
  {
    turn_left();//car turns left
    myservo.write(pos_center);
    move_forward();//car moves forward at servo being at center position
  } 
  else if (right_initial_dist > center_initial_dist && right_initial_dist > left_initial_dist) 
  {
    turn_right();//car turns right
    myservo.write(pos_center);
    move_forward();//car moves forward at servo being at center position
  }
}

void servo_control()
{
  int left_dist = 0;
  int center_dist = 0;
  int right_dist = 0;

  myservo.write(pos_center);
  center_dist = distance_calculation();

  if (center_dist <= least_distance)
  {
    stop_car();//stopping to get stable reading

    //scan left
    myservo.write(pos_left);
    delay(scan_time);
    left_dist = distance_calculation();

    //scan right
    myservo.write(pos_right);
    delay(scan_time);
    right_dist = distance_calculation();

    myservo.write(pos_center);
    delay(scan_time);

    if(left_dist > right_dist)
    {
      turn_left();
      move_forward();
    }
    else
    {
      turn_right();
      move_forward();
    }
  }
  else
  {
    move_forward();
  }
}

void setup() {
  pinMode(trig_p, OUTPUT);//trig pin sends out ultrasonic waves
  pinMode(echo_p, INPUT);//echo pin recieves ultrasonic waves

  // all motor pins are output including pwm control
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  myservo.attach(9);  // attaches the servo on pin 9 to the Servo object
  delay(500);//gives servo time to settle at center position
  initial_best_distance();
}

void loop() {
  servo_control();
}
