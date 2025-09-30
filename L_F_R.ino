// Pin Definitions
#define LEFT_MOTOR_FORWARD 9
#define LEFT_MOTOR_BACKWARD 10
#define RIGHT_MOTOR_FORWARD 11
#define RIGHT_MOTOR_BACKWARD 12

// Sensor Pin Assignments
const int sensorPins[] = {A0, A1, A2, A3, A4, A5, A6, A7}; 
int sensorValues[8];  // Store sensor readings

// Thresholds and Background Detection
int BLACK_THRESHOLD = 500; // Default threshold; recalibrated in setup
int WHITE_THRESHOLD = 300; // Default threshold; recalibrated in setup
bool isBlackBackground = true;  // True if starting with black background

// Speed Settings
#define BASE_SPEED 180
#define HIGH_SPEED 255

void setup() {
  // Set motor pins as output
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  
  // Set up serial for debugging
  Serial.begin(9600);
  
  // Calibrate background based on initial sensor readings
  calibrateBackground();
}

void loop() {
  // Read sensor values
  for (int i = 0; i < 8; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
  }

  // Interpret sensor data based on background
  bool onLine = isBlackBackground ? 
                (sensorValues[3] < WHITE_THRESHOLD && sensorValues[4] < WHITE_THRESHOLD) : 
                (sensorValues[3] > BLACK_THRESHOLD && sensorValues[4] > BLACK_THRESHOLD);
                
  bool leftDetected = isBlackBackground ? 
                      (sensorValues[0] < WHITE_THRESHOLD || sensorValues[1] < WHITE_THRESHOLD) : 
                      (sensorValues[0] > BLACK_THRESHOLD || sensorValues[1] > BLACK_THRESHOLD);
                      
  bool rightDetected = isBlackBackground ? 
                       (sensorValues[6] < WHITE_THRESHOLD || sensorValues[7] < WHITE_THRESHOLD) : 
                       (sensorValues[6] > BLACK_THRESHOLD || sensorValues[7] > BLACK_THRESHOLD);

  bool farLeftDetected = isBlackBackground ? (sensorValues[0] < WHITE_THRESHOLD) : (sensorValues[0] > BLACK_THRESHOLD);
  bool farRightDetected = isBlackBackground ? (sensorValues[7] < WHITE_THRESHOLD) : (sensorValues[7] > BLACK_THRESHOLD);

  if (onLine) {
    // Forward on line
    driveForward(BASE_SPEED);
  } else if (leftDetected && !rightDetected) {
    // Turn slightly left
    turnLeft(BASE_SPEED / 2);
  } else if (!leftDetected && rightDetected) {
    // Turn slightly right
    turnRight(BASE_SPEED / 2);
  } else if (farLeftDetected) {
    // Acute left turn
    turnLeft(HIGH_SPEED);
  } else if (farRightDetected) {
    // Acute right turn
    turnRight(HIGH_SPEED);
  } else if (!leftDetected && !rightDetected) {
    // Potential end or misalignment
    if (sensorValues[3] > BLACK_THRESHOLD || sensorValues[4] > BLACK_THRESHOLD) {
      // Possible dead-end or wrong path; make a U-turn
      turnAround();
    }
  }
}

// Motor Control Functions
void driveForward(int speed) {
  analogWrite(LEFT_MOTOR_FORWARD, speed);
  analogWrite(RIGHT_MOTOR_FORWARD, speed);
  analogWrite(LEFT_MOTOR_BACKWARD, 0);
  analogWrite(RIGHT_MOTOR_BACKWARD, 0);
}

void turnLeft(int speed) {
  analogWrite(LEFT_MOTOR_FORWARD, 0);
  analogWrite(RIGHT_MOTOR_FORWARD, speed);
  analogWrite(LEFT_MOTOR_BACKWARD, 0);
  analogWrite(RIGHT_MOTOR_BACKWARD, 0);
}

void turnRight(int speed) {
  analogWrite(LEFT_MOTOR_FORWARD, speed);
  analogWrite(RIGHT_MOTOR_FORWARD, 0);
  analogWrite(LEFT_MOTOR_BACKWARD, 0);
  analogWrite(RIGHT_MOTOR_BACKWARD, 0);
}

void turnAround() {
  analogWrite(LEFT_MOTOR_FORWARD, HIGH_SPEED);
  analogWrite(RIGHT_MOTOR_BACKWARD, HIGH_SPEED);
  delay(500); // Adjust delay for full turn
}

void stopMotors() {
  analogWrite(LEFT_MOTOR_FORWARD, 0);
  analogWrite(RIGHT_MOTOR_FORWARD, 0);
  analogWrite(LEFT_MOTOR_BACKWARD, 0);
  analogWrite(RIGHT_MOTOR_BACKWARD, 0);
}

void calibrateBackground() {
  // Read initial sensor values for calibration
  int initialReading = analogRead(sensorPins[3]);

  // Determine background based on initial reading
  if (initialReading > 600) {
    isBlackBackground = true;
    BLACK_THRESHOLD = 500; // Set appropriate thresholds for black background
    WHITE_THRESHOLD = 300;
  } else {
    isBlackBackground = false;
    BLACK_THRESHOLD = 300; // Set appropriate thresholds for white background
    WHITE_THRESHOLD = 500;
  }
  Serial.print("Background calibrated as: ");
  Serial.println(isBlackBackground ? "Black" : "White");
}
