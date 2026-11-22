void setup() {
  // put your setup code here, to run once:
  // defining controller pins
  int AIN1 = 15;
  int AIN2 = 26;
  int PWMA = 4;
  
  //Setting pins as outputs for motor driving
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(PWMA,OUTPUT);
  

  // setting pins low for the start
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(PWMA, LOW);

  // calling the function in startup
  void motordirection(int Direction, int AIN1,int AIN2,int PWMA);

  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Direction = 1;
  motordirection(1,AIN1,AIN2,PWMA)
}



  // Defining the function
void motordirection(int Direction, int AIN1,int AIN2,int PWMA)
  {
  //   if(Direction == 1 ){
  //   digitalWrite(AIN1, HIGH);
  //   digitalWrite(AIN2, LOW);
  // }
  // else(){
  //   digitalWrite(AIN1, LOW);
  //   digitalWrite(AIN2, HIGH);
  // }

  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  delay(1000);

  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  delay(1000);

  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);

  }






