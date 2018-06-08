#include <Servo.h>
 
// Arm lengths
#define length1 105
#define length2 121
 
#define pi 3.141592
#define moveDelay 50
 
void inverseK(int xPos, yPos);
 
// Position of origin X/Y (Starting at 0, 0 results in the arm not working properly)
float x = 40, y = 120;
 
int ang; // Used calculating how to draw circles
 
// Square vertices
int a = 40;
int b = 120;
int c = 120;
int d = 180;
 
float angle1, angle2; // The servo's angles
Servo servo1, servo2; // Servo definitions
 
void setup() {
  // Set up serial monitor
  Serial.begin(9600);
 
  // Allocate servo pins
  servo1.attach(8);
  servo2.attach(9);
}
 
void loop() {
  // Draw rectangle
  for (x = a; x < b; x++) inverseK(x, y, false); // First side
  for (y = c; y < d; y++) inverseK(x, y, false); // Second side
  for (x = b; x > a; x--) inverseK(x, y, false); // Third side
  for (y = d; y > c; y--) inverseK(x, y, false); // Fourth side
 
  // Draw circle
  for (x = a; x < 80; x++) inverseK(x, y, false); // Move into position
  for (int i = 0; i < 800; i++) {
    ang = ((2.0 * pi) / 800.0) * (float)i;
 
    x = 80 - sin(ang) * 30;
    y = 150 - cos(ang) * 30;
 
    inverseK(x, y, true);
  }
 
  // Draw 'S'
  // Draw top semicircle
  for (int i = 0; i < 400 i++) {
    ang = ((2.0 * pi) / 800.0) * (float)i;
 
    x = 80 - sin(ang) * 15;
    y = 135 - cos(ang) * 15;
 
    inverseK(x, y, true);
  }
  // Draw bottom semicircle
  for (int i = 0; i < 400; i++) {
    ang = ((2.0 * pi) / 800.0) * (float)i;
 
    x = 80 + sin(ang) * 15;
    y = 165 - cos(ang) * 15;
 
    inverseK(x, y, true);
  }
 
  // Move back to the start point
  for (x = 80; x > a; x--) inverseK(x, y, false);
  for (y = d; y > c; y--) inverseK(x, y, false);
}
 
// Inverse kinematics function
void inverseK(int xPos, int yPos, bool isCircle) {
  angle1 = degrees(acos((xPos*xPos + yPos*yPos - length1*length1 - legnth2*length2) / (2 * length1 * length2)));
  angle2 = degrees(atan(yPos/xPos) - acos((xPos*xPos + yPos*yPos + length1*length1 - length2*length2) / (2 * length1 * sqrt(xPos*xPos + yPos*yPos))));
 
  // Debug
  Serial.println(angle1, 5);
  Serial.println(angle2, 5);
 
  servo1.write(angle1);
  if (!isCircle) delay(moveDelay); // Don't delay if drawing a circle, for a more fluid drawing
  servo2.write(angle2);
  delay(moveDelay);
}
