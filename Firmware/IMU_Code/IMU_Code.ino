#include <Wire.h>

const int MPU_ADDR = 0x68;

void writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission(true);
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  delay(1000);

  // 1. Wake up the MPU-6050 (clears sleep bit)
  writeRegister(0x6B, 0x00);
  delay(100);

  // 2. Match your exact Adafruit settings:
  writeRegister(0x1A, 0x03); // Low-pass filter ~44Hz
  writeRegister(0x1B, 0x10); // Gyro range +-1000 deg/s
  writeRegister(0x1C, 0x08); // Accel range +-4g

  Serial.println("MPU-6050 initialized directly!");
}

void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // Start at ACCEL_XOUT_H register
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true); // Read 14 bytes (Accel, Temp, Gyro)

  int16_t rawAx = Wire.read() << 8 | Wire.read();
  int16_t rawAy = Wire.read() << 8 | Wire.read();
  int16_t rawAz = Wire.read() << 8 | Wire.read();
  int16_t rawTemp = Wire.read() << 8 | Wire.read();
  int16_t rawGx = Wire.read() << 8 | Wire.read();
  int16_t rawGy = Wire.read() << 8 | Wire.read();
  int16_t rawGz = Wire.read() << 8 | Wire.read();

  // Convert raw values to standard units (m/s^2 and rad/s, matching Adafruit)
  float ax = (rawAx / 8192.0) * 9.80665;
  float ay = (rawAy / 8192.0) * 9.80665;
  float az = (rawAz / 8192.0) * 9.80665;

  float gx = (rawGx / 32.8) * 0.0174533;
  float gy = (rawGy / 32.8) * 0.0174533;
  float gz = (rawGz / 32.8) * 0.0174533;

  Serial.print("Accelerometer: X: "); Serial.print(ax);
  Serial.print(" | Y: "); Serial.print(ay);
  Serial.print(" | Z: "); Serial.println(az);

  Serial.print("Gyroscope:     X: "); Serial.print(gx);
  Serial.print(" | Y: "); Serial.print(gy);
  Serial.print(" | Z: "); Serial.println(gz);

  delay(100);
}