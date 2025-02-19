#include <ArduinoBLE.h>

BLEService newService("180A"); // creating the service
BLEByteCharacteristic readChar("2A57", BLERead);
BLEByteCharacteristic writeChar("2A58", BLEWrite);

const int trigPin = 8;
const int echoPin = 10;

long previousMillis = 0;

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

  BLE.setLocalName("Brian_Cameron_Akshay");
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

  BLEDevice central = BLE.central(); // wait for a BLE central

  if (central) {  // if a central is connected to the peripheral
    Serial.print("Connected to central: ");
    
    Serial.println(central.address()); // print the central's BT address
    
    digitalWrite(LED_BUILTIN, HIGH); // turn on the LED to indicate the connection

    while (central.connected()) { // while the central is connected:
      long currentMillis = millis();
      
      if (currentMillis - previousMillis >= 200) { 
        previousMillis = currentMillis;

        if (writeChar.written()) {
          if (writeChar.value()) {

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
            delay(10);

            readChar.writeValue(distanceCm);
            Serial.println("Distance printed to peripheral");

         }
        }

      }
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());

  }    
}