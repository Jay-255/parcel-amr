// ============================================================================
// 1. PIN & PWM CONFIGURATION
// ============================================================================
// Motor A Pins (TB6612FNG Channel A)
const int AIN1 = 15;
const int AIN2 = 26;
const int PWMA = 4;

// Motor B Pins (TB6612FNG Channel B)
const int BIN1 = 19;
const int BIN2 = 18;
const int PWMB = 17;

// Note: Connect TB6612FNG STBY pin directly to 3.3V so the driver stays enabled.

// ESP32 LEDC PWM Settings
const int PWM_FREQ = 5000;      // 5000 Hz PWM frequency
const int PWM_RES  = 8;         // 8-bit resolution (speed values from 0 to 255)
const int PWM_CH_A = 0;         // LEDC channel 0 for Motor A
const int PWM_CH_B = 1;         // LEDC channel 1 for Motor B


// ============================================================================
// 2. NON-BLOCKING TEST TIMER VARIABLES (Replaces delay())
// ============================================================================
// These track the test sequence in loop() without pausing the ESP32.
unsigned long previousMillis = 0;
int testState = 0;              // 0 = Forward, 1 = Stop, 2 = Reverse, 3 = Stop


// ============================================================================
// 3. FUNCTION DECLARATIONS
// ============================================================================
void driveMotorA(int direction, int speed);
void driveMotorB(int direction, int speed);
void stopAll();


// ============================================================================
// 4. SETUP (Runs once at boot)
// ============================================================================
void setup() {
  Serial.begin(115200);

  // Configure all direction control pins as outputs
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // Configure ESP32 LEDC hardware PWM channels and attach them to PWM pins
  ledcSetup(PWM_CH_A, PWM_FREQ, PWM_RES);
  ledcAttachPin(PWMA, PWM_CH_A);

  ledcSetup(PWM_CH_B, PWM_FREQ, PWM_RES);
  ledcAttachPin(PWMB, PWM_CH_B);

  // Start with both motors safely braked
  stopAll();
  Serial.println("Motor Control Initialized.");
}


// ============================================================================
// 5. MAIN LOOP (Non-blocking test sequence using millis())
// ============================================================================
// Sequence: Full Forward (2s) -> Stop (1s) -> Full Reverse (2s) -> Stop (2s)
// Because this uses millis(), you can drop encoder or MQTT code right into
// loop() and it will run simultaneously without freezing.
void loop() {
  unsigned long currentMillis = millis();

  switch (testState) {
    case 0: // Full Forward for 2000ms
      driveMotorA(1, 255);
      driveMotorB(1, 255);
      Serial.println("State 0: Full Forward");
      previousMillis = currentMillis;
      testState = 1;
      break;

    case 1: // Wait 2 seconds, then Stop
      if (currentMillis - previousMillis >= 2000) {
        stopAll();
        Serial.println("State 1: Stop (Brake)");
        previousMillis = currentMillis;
        testState = 2;
      }
      break;

    case 2: // Wait 1 second, then Full Reverse
      if (currentMillis - previousMillis >= 1000) {
        driveMotorA(-1, 255);
        driveMotorB(-1, 255);
        Serial.println("State 2: Full Reverse");
        previousMillis = currentMillis;
        testState = 3;
      }
      break;

    case 3: // Wait 2 seconds, then Stop
      if (currentMillis - previousMillis >= 2000) {
        stopAll();
        Serial.println("State 3: Stop (Brake)");
        previousMillis = currentMillis;
        testState = 4;
      }
      break;

    case 4: // Pause 2 seconds before repeating the test
      if (currentMillis - previousMillis >= 2000) {
        testState = 0;
      }
      break;
  }
}


// ============================================================================
// 6. REUSABLE MOTOR CONTROL FUNCTIONS
// ============================================================================

// Drives Motor A at a given direction (1 = Forward, -1 = Reverse) and speed (0-255)
void driveMotorA(int direction, int speed) {
  // Keep speed safely within 8-bit PWM bounds
  speed = constrain(speed, 0, 255);

  if (direction == 1) {
    // Forward
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  } 
  else if (direction == -1) {
    // Reverse
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  } 
  else {
    // Active Brake if direction is 0 (or invalid)
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, HIGH);
    speed = 0;
  }

  // Send PWM duty cycle to Motor A's LEDC channel
  ledcWrite(PWM_CH_A, speed);
}

// Drives Motor B at a given direction (1 = Forward, -1 = Reverse) and speed (0-255)
void driveMotorB(int direction, int speed) {
  // Keep speed safely within 8-bit PWM bounds
  speed = constrain(speed, 0, 255);

  if (direction == 1) {
    // Forward
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  } 
  else if (direction == -1) {
    // Reverse
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  } 
  else {
    // Active Brake if direction is 0 (or invalid)
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, HIGH);
    speed = 0;
  }

  // Send PWM duty cycle to Motor B's LEDC channel
  ledcWrite(PWM_CH_B, speed);
}

// Immediately brakes both motors (TB6612FNG Short Brake: both IN pins HIGH)
void stopAll() {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, HIGH);

  ledcWrite(PWM_CH_A, 0);
  ledcWrite(PWM_CH_B, 0);
}