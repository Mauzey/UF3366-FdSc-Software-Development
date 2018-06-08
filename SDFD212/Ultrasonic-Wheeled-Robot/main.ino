// Pin definitions
#define F_TRIG 5
#define F_ECHO 4
#define L_TRIG 8
#define L_ECHO 2
#define R_TRIG 7
#define R_ECHO 6

// Turn direction definitions
#define CW  0
#define CCW 1

// Motor definitions
#define MOTOR_A 0
#define MOTOR_B 1

void checkSensor(int trigPin, int echoPin, float dist);
void driveMotor(byte motor, byte dir, byte spd);
void rotate90(bool rotateLeft);
void initialise();
void debug();

// Motor pin assignments
const byte PWMA = 3;  // PWM control (speed) for motor A
const byte PWMB = 11; // PWM control (speed) for motor B
const byte DIRA = 12; // Direction control for motor A
const byte DIRB = 13; // Direction control for motor B

int frontDist, leftDist, rightDist;
int maxDist = 20; // The closest an object can get to the robot before triggering a turn (measured in cm)

void setup() {
  // Start serial monitor
  Serial.begin(9600);
  
  initialise();
}

void loop() {
  // Move forward
  driveMotor(MOTOR_A, CW, 255);
  driveMotor(MOTOR_B, CW, 255);
  
  // Check front-facing sensor
  checkSensor(F_TRIG, F_ECHO, frontDist);

  // If an obstruction is detected
  if (frontDist <= maxDist) {
    // Stop
    stopMotor(MOTOR_A);
    stopMotor(MOTOR_B);
    
    // Check left and right facing sensors
    checkSensor(L_TRIG, L_ECHO, leftDist);
    checkSensor(R_TRIG, R_ECHO, rightDist);

    // Move either left or right: whichever has the most free space
    if (leftDist > rightDist) {
      rotate90(true);
    } else {
      rotate90(false);
    }
  }

  debug();
}

void checkSensor(int trigPin, int echoPin, float dist) {
  // Check signal
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  // Read signal
  pinMode(echoPin, INPUT);
  dist = (pulseIn(echoPin, HIGH) / 2) / 29.1;
}

void driveMotor(byte motor, byte dir, byte spd) {
  if (motor == MOTOR_A) {
    digitalWrite(DIRA, dir);
    analogWrite(PWMA, spd);
  } else if (motor == MOTOR_B) {
    digitalWrite(DIRB, dir);
    analogWrite(PWMB, spd);
  }
}

void rotate90(bool rotateLeft) {
  if (rotateLeft) {
    driveMotor(MOTOR_A, CW, 255);
    driveMotor(MOTOR_B, CCW, 255);
  } else {
    driveMotor(MOTOR_A, CCW, 255);
    driveMotor(MOTOR_B, CW, 255);
  }

  delay(500);
  stopMotor(MOTOR_A);
  stopMotor(MOTOR_B);
}

// Initialise motors and sensors
void initialise() {
  // Set up ultrasonic pins
  pinMode(F_TRIG, OUTPUT);
  pinMode(F_ECHO, INPUT);
  pinMode(L_TRIG, OUTPUT);
  pinMode(L_ECHO, INPUT);
  pinMode(R_TRIG, OUTPUT);
  pinMode(R_ECHO, INPUT);

  // Set up motor pins
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);

  // Init motor pins as low
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);

  Serial.print("Initialised\n\n");
}

void debug() {
  Serial.print("front dist: ");
  Serial.print(frontDist);
  Serial.print(" left dist: ");
  Serial.print(leftDist);
  Serial.print(" right dist: ");
  Serial.println(rightDist);
}
