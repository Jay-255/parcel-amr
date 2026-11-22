#include <Arduino.h>

// --- Pin Definitions ---
const int MOTOR_A_CH_A = 34;
const int MOTOR_A_CH_B = 35;
const int MOTOR_B_CH_A = 36;
const int MOTOR_B_CH_B = 39;

// --- Mechanical & Encoder Constants ---
const float PPR = 11.0f;                  // JGB37-545 encoder pulses per motor shaft rev
const float GEAR_RATIO = 45.0f;           // <-- UPDATE THIS to your exact motor (e.g., 45.0 or 56.0)
const float QUAD_MULTIPLIER = 4.0f;       // 4x decoding via QUADTABLE (counts all 4 edges per pulse)
const float WHEEL_DIAMETER_MM = 65.0f;

const float COUNTS_PER_WHEEL_REV = PPR * GEAR_RATIO * QUAD_MULTIPLIER;
const float WHEEL_CIRCUMFERENCE_MM = PI * WHEEL_DIAMETER_MM;
const float MM_PER_COUNT = WHEEL_CIRCUMFERENCE_MM / COUNTS_PER_WHEEL_REV;

// Flip to -1 if a wheel counts negative when pushed forward
const int8_t MOTOR_A_SIGN = 1;
const int8_t MOTOR_B_SIGN = -1;           // Opposing wheel on a diff-drive robot usually spins opposite

// --- Shared ISR Variables ---
volatile int32_t encoderCountA = 0;
volatile int32_t encoderCountB = 0;
volatile uint8_t encoderStateA = 0;
volatile uint8_t encoderStateB = 0;

// Kept in DRAM so ISRs never crash when accessing it during Flash/Wi-Fi ops
static const DRAM_ATTR int8_t QUADTABLE[16] = {
   0, -1, +1,  0,
  +1,  0,  0, -1,
  -1,  0,  0, +1,
   0, +1, -1,  0
};

// --- ISRs ---
void IRAM_ATTR handleEncoderA() {
  uint8_t a = digitalRead(MOTOR_A_CH_A);
  uint8_t b = digitalRead(MOTOR_A_CH_B);
  uint8_t currentState = (a << 1) | b;
  uint8_t index = (encoderStateA << 2) | currentState;

  encoderCountA += MOTOR_A_SIGN * QUADTABLE[index];
  encoderStateA = currentState;
}

void IRAM_ATTR handleEncoderB() {
  uint8_t a = digitalRead(MOTOR_B_CH_A);
  uint8_t b = digitalRead(MOTOR_B_CH_B);
  uint8_t currentState = (a << 1) | b;
  uint8_t index = (encoderStateB << 2) | currentState;

  encoderCountB += MOTOR_B_SIGN * QUADTABLE[index];
  encoderStateB = currentState;
}

// --- Helper Functions ---
int32_t readEncoderA() {
  noInterrupts();
  int32_t val = encoderCountA;
  interrupts();
  return val;
}

int32_t readEncoderB() {
  noInterrupts();
  int32_t val = encoderCountB;
  interrupts();
  return val;
}

void resetEncoders() {
  noInterrupts();
  encoderCountA = 0;
  encoderCountB = 0;
  interrupts();
}

const char* getDirection(int32_t delta) {
  if (delta > 0) return "FWD";
  if (delta < 0) return "REV";
  return "STOP";
}

void setup() {
  Serial.begin(115200);

  // GPIOs 34-39 are input-only and lack internal pull-ups on ESP32.
  // Ensure your JGB37-545 encoder board has pull-ups or add external 10k resistors to 3.3V.
  pinMode(MOTOR_A_CH_A, INPUT);
  pinMode(MOTOR_A_CH_B, INPUT);
  pinMode(MOTOR_B_CH_A, INPUT);
  pinMode(MOTOR_B_CH_B, INPUT);

  // Initialize global states (no shadowing)
  encoderStateA = (digitalRead(MOTOR_A_CH_A) << 1) | digitalRead(MOTOR_A_CH_B);
  encoderStateB = (digitalRead(MOTOR_B_CH_A) << 1) | digitalRead(MOTOR_B_CH_B);

  attachInterrupt(digitalPinToInterrupt(MOTOR_A_CH_A), handleEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(MOTOR_A_CH_B), handleEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(MOTOR_B_CH_A), handleEncoderB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(MOTOR_B_CH_B), handleEncoderB, CHANGE);

  Serial.println("Encoders ready. Send 'r' in Serial Monitor to reset counts.");
}

void loop() {
  static unsigned long lastPrintTime = 0;
  static int32_t lastCountA = 0;   // Signed int32_t so negative deltas work
  static int32_t lastCountB = 0;

  // Allow easy testing of resetEncoders() via Serial Monitor
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'r' || cmd == 'R') {
      resetEncoders();
      lastCountA = 0;
      lastCountB = 0;
      Serial.println(">>> Encoders reset to 0 <<<");
    }
  }

  // Print every 500ms without blocking
  if (millis() - lastPrintTime >= 500) {
    lastPrintTime = millis();

    int32_t countA = readEncoderA();
    int32_t countB = readEncoderB();

    int32_t deltaA = countA - lastCountA;
    int32_t deltaB = countB - lastCountB;

    float distA_mm = countA * MM_PER_COUNT;
    float distB_mm = countB * MM_PER_COUNT;

    Serial.print("Motor A | Dir: ");
    Serial.print(getDirection(deltaA));
    Serial.print(" | Counts: ");
    Serial.print(countA);
    Serial.print(" | Dist: ");
    Serial.print(distA_mm, 1);
    Serial.print(" mm   ||   ");

    Serial.print("Motor B | Dir: ");
    Serial.print(getDirection(deltaB));
    Serial.print(" | Counts: ");
    Serial.print(countB);
    Serial.print(" | Dist: ");
    Serial.print(distB_mm, 1);
    Serial.println(" mm");

    lastCountA = countA;
    lastCountB = countB;
  }
}