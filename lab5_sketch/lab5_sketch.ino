#include <ArduinoBLE.h>

BLEService newService("180A"); // creating the service
BLEByteCharacteristic readChar("2A57", BLERead);
BLEByteCharacteristic writeChar("2A58", BLEWrite);

const int trigPin = 8;
const int echoPin = 10;
long duration;
int distanceCm, distanceInch;

void setup() {

  // peripheral pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);

  while(!Serial);
  if (!BLE.begin()){
    Serial.println("Waiting for ArduinoBLE");
    while(1);
  }

  BLE.setDeviceName("Brian_Cameron_Akshay");
  BLE.setAdvertisedService(newService);
  newService.addCharacteristic(readChar);
  newService.addCharacteristic(writeChar);
  BLE.addService(newService);

  readChar.writeValue(0);
  writeChar.writeValue(0);

  BLE.advertise();
  Serial.println("Bluetooth device active");

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
  distanceCm = duration / 58;
  distanceInch = duration / 148;
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.print(" cm/");
  Serial.print(distanceInch);
  Serial.println(" in");
  delay(1000);



}