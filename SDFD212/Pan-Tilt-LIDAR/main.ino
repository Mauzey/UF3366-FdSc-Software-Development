#include <LIDARLite.h>
#include <Servo.h>
#include <Wire.h>

void    debug(bool e);
void    getCoords(int pA, int tA, int d);
float   degToRad(int d);

LIDARLite lidar;
Servo panServo, tiltServo;

float xPos, yPos, zPos;

float lidarDist;
float panAng    = 90;
float tiltAng   = 65;
float moveDelay = 40;

void setup(){
  Serial.begin(115200);
  
  // Assign servo pins
  panServo.attach(9);
  tiltServo.attach(8);
  
  // Set servos to default positions
  panServo.write(panAng);
  tiltServo.write(tiltAng);
  delay(moveDelay);
  
  // Config lidar for I2C/400kHz
  lidar.begin(0, true);
  lidar.configure(0);
}

void loop(){
  // Sweep tilt servo from bottom to top
  for (tiltAng = 45; tiltAng <= 85; tiltAng += 1){
    tiltServo.write(tiltAng);
    
    // Sweep pan servo from left to right
    for (panAng = 70; panAng <= 110; panAng += 1){
      lidarDist = lidar.distance();
      
      getCoords(panAng, tiltAng, lidarDist);
      
      // Move servo
      panServo.write(panAng);
      delay(moveDelay);
      debug(false);
    }
    
    // Return to left position
    panAng = 70;
    panServo.write(panAng);
    delay(moveDelay + 40);
  }
}

// Get coords, given a set of angles and a distance (args = panAngle, tiltAngle, distance)
void getCoords(float pA, float tA, float d){
  float C; // Temporary var for calculations
  
  // Calculate Z value
  zPos = sin(degToRad(tA - 65)) * d;
  
  // Calculate X value
  C = sqrt((d * d) - (zPos * zPos));
  xPos = cos(degToRad(pA - 90)) * C;
  
  // Calculate Y value
  yPos = sqrt((C * C) - (xPos * xPos));
}

// Convert degrees to radians (args = degrees)
float degToRad(float d){
  float r;
  r = d / 360 * (2 * PI);
  
  return r;
}

// Print debug info (args = additional information)
void debug(bool e){
  if (e){
    Serial.print("Pan Angle: ");
    Serial.print(panAng);
    Serial.print("\tTilt Angle: ");
    Serial.print(tiltAng);
    Serial.print("\tDistance: ");
    Serial.print(lidarDist);
    Serial.print("\txPos: ");
    Serial.print(xPos);
    Serial.print("\tyPos: ");
    Serial.print(yPos);
    Serial.print("\tzPos: ");
    Serial.println(zPos);
  } else {
    Serial.print(xPos);
    Serial.print("\t");
    Serial.print(yPos);
    Serial.print("\t");
    Serial.print(zPos);
    Serial.println("\t");
  }
}
