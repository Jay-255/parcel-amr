//Defining pins:
int pwma = 4;
int AIN1 = 15;
int AIN2 = 26;
int motor_speed = 0;

//Declaring functions
void motor_velocity();
void motor_direction();

void setup() {
  // put your setup code here, to run once:
  pinMode(pwma, OUTPUT);
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  Serial.begin(115200);
  delay(3000);
  Serial.println("Test Initiated");
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2,LOW);
   
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(motor_speed);
  if(motor_speed ==0)
  {
  Serial.println("if");
  // int motor_speed = Serial.parseInt();
  int motor_speed = 60;
  int speed = motor_speed;
  // analogWrite(pwma,speed*2.55);
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  motor_velocity(speed,pwma);
  // motor_direction(1,AIN1,AIN2);
  }
  else{
  Serial.println("else");
  int speed = motor_speed;
  // analogWrite(pwma,speed*2.55);
  digitalWrite(AIN1,HIGH);
  digitalWrite(AIN2,LOW);
  motor_velocity(speed,pwma);
  // motor_direction(1,AIN1,AIN2);
  }
  Serial.println(motor_speed);
  Serial.println("done");
  delay(1000);


  //attain motor speed from serial
  // while(Serial.available() == 0){
  // }
  // int motor_speed = Serial.parseInt();
  
}
 


  void motor_velocity(int velocity_percentage,int pwm){
    //Accelerate to specified Velocity
    int velocity_value = 2.55*velocity_percentage;
    int i = velocity_value;

    analogWrite(pwm,i);
  // delay(500);
}

void motor_direction(int direction,int in1,int in2){
  if(direction == 1){
  //Forward
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  Serial.println("Forward");
  }
  if(direction == 2){
  //Backward  
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  Serial.println("Reverse");
  }
  else{
  //Break
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  Serial.println("Break");
  }
  
}
