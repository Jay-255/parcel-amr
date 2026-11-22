
//Declaring pins
int Sensor_A = 39;
int Sensor_B = 36;
int current_state;
int last_state;
String direction;
int counter = 0;
String get_distance();


void setup() {
  //Setup
  pinMode(Sensor_A,INPUT_PULLUP);
  pinMode(Sensor_B,INPUT_PULLUP);
  // digitalWrite(Sensor_A,HIGH);
  // digitalWrite(Sensor_B,HIGH);
  Serial.begin(115200);

  last_state = digitalRead(Sensor_A);  
}

void loop() {

  current_state = digitalRead(Sensor_A);

  if(current_state != last_state && current_state == HIGH){
  //Direction Determination
  if(digitalRead(Sensor_B) != current_state){
    counter--;
    direction = "CCW";
  }
  else{
    counter++;
    direction = "CW";
  }
  int angle = 4*counter/11;
  double dst = 3.142*65/990;
  int Dist = dst * counter;
  //counter
  Serial.print("Direction: ");
  Serial.print(direction);
  Serial.print(" | Count: ");
  Serial.print(counter);
  Serial.print(" | Distance: ");
  Serial.print(Dist);
  Serial.print(" | angle: ");
  Serial.println(angle);
  }

  last_state = current_state;
  delay(1);
}

String get_distance(int pin_A, int pin_B){
  int current_state;
  int last_state;
  String direction;
  int counter;

  current_state = digitalRead(pin_A);

  if(current_state != last_state && current_state == HIGH){
  //Direction Determination
  if(digitalRead(pin_B) != current_state){
    counter--;
    direction = "CCW";
  }
  else{
    counter++;
    direction = "CW";
  }
  int angle = 4*counter/11;
  double dst = 3.142*65/990;
  int Dist = dst * counter;
  //counter
  Serial.print("Direction: ");
  Serial.print(direction);
  Serial.print(" | Count: ");
  Serial.print(counter);
  Serial.print(" | Distance: ");
  Serial.print(Dist);
  Serial.print(" | angle: ");
  Serial.println(angle);
  }

  last_state = current_state;
  delay(1);
}





