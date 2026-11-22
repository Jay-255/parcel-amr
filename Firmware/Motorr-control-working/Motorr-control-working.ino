
  int AIN1 = 15;
  int AIN2 = 26;
  int PWMA = 4;


void setup() {
  // put your setup code here, to run once:
  // defining controller pins
  
  
  //Setting pins as outputs for motor driving
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(PWMA,OUTPUT);
  

  // setting pins low for the start
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(PWMA, LOW);
  Serial.begin(115200);
  
}





void loop(){

 

  }

void motor_dir(int Direction){
  if(Direction == 1){
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    Serial.println("Forward");
  }
  else{
    if(Direction == 0){
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, LOW);
      Serial.println("Stop");
    }
    else{
      if(Direction == 2){
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);
      Serial.println("Back"); 
      }
      else{}
    }
    


  }
}


