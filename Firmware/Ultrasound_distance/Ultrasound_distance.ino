//defining pins
  int trig1 = 13;
  int echo1 = 25;
  int trig2 = 14;
  int echo2 = 27;
  int trig3 = 33;
  int echo3 = 23;
  long readSensor();
void setup() {
  // put your setup code here, to run once:
  //Defining pins

  //pin mappings
  pinMode(trig1,OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2,OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trig3,OUTPUT);
  pinMode(echo3, INPUT);
  Serial.begin(115200);
  Serial.println("Distance Test Loading...");
  delay(3000);

}

void loop() {
  // put your main code here, to run repeatedly:
  int distance1 = readSensor(trig1,echo1);
  int distance2 = readSensor(trig2,echo2);
  int distance3 = readSensor(trig3,echo3);
  Serial.print("D1: ");
  Serial.print(distance1);
  Serial.print(" | ");
  Serial.print("D2: ");
  Serial.print(distance2);
  Serial.print(" | ");
  Serial.print("D3: ");
  Serial.println(distance3);
  delay(1000);

}

long readSensor(int trig,int echo){

  //initial trigger for Trig to start
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);

  //stop initial trigger
  digitalWrite(trig,LOW);

  //Read echo duration
  long duration = pulseIn(echo,HIGH);

  //Calculate distance
  int distance = 0.343/2 * duration;

  //Return Distance
  return distance;
  // Serial.println("mm ");

  //Delay for 1 second to avoid spamming
  
}

