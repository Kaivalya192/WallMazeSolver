#include <L298N.h>

// Define L298N instances for each motor
L298N motor1(9, 8, 10);  // Adjust pin numbers as per your wiring
L298N motor2(11, 12, 13);  // Adjust pin numbers as per your wiring

int error = 0;
int lerror = 0;
int speed1, speed2, out;
const int kp = 15;
const int kd = 7;

int trigPin1 = 6;
int echoPin1 = A1;
int trigPin2 = 5;
int echoPin2 = A0;
long duration1, cm1;
long duration2, cm2;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  // Adjust motor direction if necessary
  motor1.setSpeed(255);  // Adjust speed as needed
  motor2.setSpeed(255);  // Adjust speed as needed
  
  delay(1000);
}

void loop() {
  // Readings for sensor 1
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  pinMode(echoPin1, INPUT);
  duration1 = pulseIn(echoPin1, HIGH);
  cm1 = (duration1 / 2) / 29.1;

  // Readings for sensor 2
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  pinMode(echoPin2, INPUT);
  duration2 = pulseIn(echoPin2, HIGH);
  cm2 = (duration2 / 2) / 29.1;

  // Debugging: print sensor readings
  Serial.print("Sensor 1: ");
  Serial.print(cm1);
  Serial.print(" cm, Sensor 2: ");
  Serial.print(cm2);
  Serial.println(" cm");

  // Adjusting motor speed based on error
  if (cm2 < 10 && cm2 > 6) {
    error = 0;
  }
  if (cm2 > 10 && cm2 < 20) {
    error = 1;
  }
  if (cm2 < 6) {
    error = -1;
  }
  out = kp * error + kd * (lerror - error);
  Serial.print("Output: ");
  Serial.println(out);

  speed1 = 255 + out;  // Adjust speed base value as needed
  speed2 = 255 - out;  // Adjust speed base value as needed

  motor1.setSpeed(speed1);
  motor2.setSpeed(speed2);

  // Debugging: print motor speeds
  Serial.print("Motor Speeds: ");
  Serial.print(speed1);
  Serial.print(", ");
  Serial.println(speed2);

  // Adjust motor directions based on conditions
  if (cm1 < 5 && cm2 < 10) {
    motor1.run(L298N::BACKWARD);  // Adjust direction as needed
    motor2.run(L298N::BACKWARD);  // Adjust direction as needed
    delay(500);
    motor1.stop();
    motor2.stop();
  }
  if (cm2 > 17) {
    delay(1100);
    motor1.run(L298N::FORWARD);  // Adjust direction as needed
    motor2.run(L298N::FORWARD);  // Adjust direction as needed
    delay(500);
    motor1.stop();
    motor2.stop();
  }
}
