//ultrasonic sensor pins
int trigPinFront = 11;
int echoPinFront = 10;
int trigPinBack = 9;
int echoPinBack = 8;

//motor controller pins
int motorSpeedPinFront = 23; //PWM
int motorDirPinFront = 25;
int motorSpeedPinBack = 27; //PWM
int motorDirPinBack = 29;

//how close should the end of the track be before the pod reverses
//accuracy is between ~6cm and 4m
float safeDistance = 10; //cm

//when dir is postive, we go "forward," negative "backward"
//usually 1 or -1
int dir = 1;

int motorSpeed = 10; //value between 0 and 255 representing 0-35v I think

void setup() {
  Serial.begin(9600);
}

float finddistance(int trigPin, int echoPin) {
  float duration, cm;

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  cm = duration / 29 / 2;
  return cm;
}

void updatedirection() {
  if (finddistance(trigPinFront, echoPinFront) < safeDistance) {
    dir = -1;
    digitalWrite(motorDirPinFront, LOW);
    digitalWrite(motorDirPinBack, LOW);
  }
  if (finddistance(trigPinBack, echoPinBack) < safeDistance) {
    dir = 1;
    digitalWrite(motorDirPinFront, HIGH);
    digitalWrite(motorDirPinBack, HIGH);
  }
}

void setspeed(int optionalNewSpeed = motorSpeed) {
  motorSpeed = optionalNewSpeed;
  analogWrite(motorSpeedPinFront, motorSpeed);
  analogWrite(motorSpeedPinBack, motorSpeed);
}

String hrdir(int dir) {
  //human readable direction
  if (dir > 0) {
    return "Forward";
  } else {
    return "Backward";
  }
}

void loop() {
  updatedirection();
  Serial.print("Motors turning: ");
  Serial.println(hrdir(dir));
  
  setspeed();
  //Serial.print("Current speed: ");
  //Serial.print(((float)motorSpeed / 255) * 30);
  //Serial.println("volts");
  
  //Serial.print("Front post: ");
  Serial.print(finddistance(trigPinFront, echoPinFront));
  //Serial.println(" cm");
  //Serial.print("Back post: ");
  Serial.print(finddistance(trigPinBack, echoPinBack));
  //Serial.println(" cm");
}
