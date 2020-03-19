/*
 * created by Myat Su Phyo
 * moving car to carry mosquito coils 
 */

// ultrasonic1 
int trigPin = 8;    // Trigger
int echoPin = 9;    // Echo
int LED = 10;

long duration, inches, old_record_inches;
int status = 1;

// LM35
float lm_value, temp;

// flame
int flameSensor = 7;
int flameSensorValue = HIGH;
 
void setup() {
  //Serial Port begin
  Serial.begin (9600);
  // ultrasonic 1st pair -> trigPin, echoPin, LED
  pinMode(11, OUTPUT);
  pinMode(12, INPUT);
  pinMode(13, OUTPUT);
  // ultrasonic 2nd pair -> trigPin, echoPin, LED
  pinMode(8, OUTPUT);
  pinMode(9, INPUT);
  pinMode(10, OUTPUT);
  // flame sensor
  pinMode(flameSensor, INPUT);
}

void loop() {
  checkUltrasonicSensor(trigPin, echoPin, LED);
  
  // to check temperature from LM35
  checkTemperatureSenor(A0);
  
  switch (status) {
    case 0:
    // there is an obstacle, switch direction
      Serial.println("Switch sensor");
      setUltrasonicSensor();
    break;
    case 1: 
    // // there is no obstacle in front of the sensor
      Serial.print("Keep going with Ultrasonic Sensor Trig at ");
      Serial.println(trigPin);
    break;
    case 2: 
    // the burning mosquit coil has reached its end, stop moving around
      Serial.println("Finished burning");
      digitalWrite(LED, LOW);
    break;
  } 
  
  delay(1000);
}

// start of temperature and flame 
void checkTemperatureSenor(int LM35) {
  lm_value = analogRead(LM35);
  temp = (lm_value * 500) / 1023;
  Serial.print("Temp in C = ");
  Serial.println(temp);//Temperature in Celcius
  if (temp >35) {
    status = 2;
    checkFlameSensor();
  } 
}
void checkFlameSensor() {
  flameSensorValue = digitalRead(flameSensor);
  if (flameSensorValue == 0) {
    Serial.println("FIRE FIRE FIRE");
    Serial.println("Fire buzzer here");
  }
}
// end of temperature and flame

// start of ultrasonic
void checkUltrasonicSensor(int trigPin, int echoPin, int LED){
  // to know when the car is stucked
  old_record_inches = inches;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  
  // Convert the time into a distance
  inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135
  
  Serial.print("The closest obstacle is at ");
  Serial.print(inches);
  Serial.println(" in.");
  Serial.println();
  
  if (inches <=5 || inches == old_record_inches ) {
    digitalWrite(LED, LOW);
    status = 0;
  } else {
    digitalWrite(LED, HIGH);
    status = 1;
  }
}
void setUltrasonicSensor() {
  // LED 10 is for first sensor
  if (LED == 10 ) {
    trigPin = 11;
    echoPin = 12;
    LED = 13;
  } else {
    trigPin = 8;
    echoPin = 9;
    LED = 10;
  }
}
// end of ultrasonic