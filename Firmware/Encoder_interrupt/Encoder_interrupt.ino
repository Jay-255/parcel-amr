

//Define pins
int DT = 39;
int CLK = 36;

//Define functions

int32_t readencodercount();


volatile int32_t encodercount = 0; //Full decoded position
volatile uint8_t encoderstate = 0; // last 2-bit AB state

// encoder sequence table
static const int8_t QUADTABLE[16] = {
  0,-1,+1,0,
  +1,0,0,-1,
  -1,0,0,+1,
  0,+1,-1,0
};

void IRAM_ATTR handleEncoderISR(){
  uint8_t a = digitalRead(DT);
  uint8_t b = digitalRead(CLK);
  
  uint8_t currentstate = (a << 1) | b;
  uint8_t index = (encoderstate << 2) | currentstate;

  encodercount += QUADTABLE[index];
  encoderstate = currentstate;
}

int32_t readEncodercount(){
  noInterrupts();
  int32_t value = encodercount;
  interrupts();
  return value;
}

int32_t readEncoderdetents(){
  return readEncodercount()/4;
}





void setup() {
  // put your setup code here, to run once:
  pinMode(DT, INPUT_PULLUP);
  pinMode(CLK, INPUT_PULLUP);

  Serial.begin(115200);

  uint8_t a = digitalRead(DT);
  uint8_t b = digitalRead(CLK);
  uint8_t encoderstate = (a << 1) | b;


  attachInterrupt(digitalPinToInterrupt(DT), handleEncoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CLK), handleEncoderISR, CHANGE);

}

void loop() {
  // put your main code here, to run repeatedly:
  static uint32_t lastReportedDetents = 0;
  int32_t detents = readEncoderdetents();


  if (detents != lastReportedDetents){
    if ((detents - lastReportedDetents) < 0){
      Serial.print("CW | Distance: ");
      Serial.println(detents);
    }
    else{
      Serial.print("CCW | Distance: ");
      Serial.println(detents);
    }
    lastReportedDetents = detents;
  }

}
