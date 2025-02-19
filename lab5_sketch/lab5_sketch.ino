const int trigPin = 8;
const int echoPin = 10;
long duration;
int distanceCm, distanceInch;

void setup() {

  // peripheral pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);

}

void loop() {

  // Sending trigger pulse
  // Sending trigger pulse to peripheral
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); // good practice to send low state to peripheral first
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Calculation output pulse duration
  duration = pulseIn(echoPin, HIGH);

  // Calibrating sensor
  Serial.print(" cm/");
  Serial.print(distanceInch);
  Serial.println(" in");


  delay(1000);

}