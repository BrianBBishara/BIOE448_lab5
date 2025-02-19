const int trigPin = 11;
const int echoPin = 12;
long duration;
int distanceCm, distanceInch;

void setup() {

  // peripheral pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // LED pins
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

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
@@ -30,6 +37,17 @@
  Serial.print(" cm/");
  Serial.print(distanceInch);
  Serial.println(" in");

  // LED feedback for distance
    if (distanceInch > 10) {
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
  } 
  else {
    digitalWrite(4, HIGH);
    digitalWrite(3, LOW);   
  }

  delay(1000);

}