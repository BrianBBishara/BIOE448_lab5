#include <ArduinoBLE.h>

BLEService newService("180A"); // create the service
BLEByteCharacteristic readChar("2A57", BLERead); // create the read characteristic
BLEByteCharacteristic writeChar("2A58", BLEWrite); // create the write characteristic

// peripheral pins
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

  while(!Serial); // must open serial monitor
  if (!BLE.begin()){ // pause function if ArduinoBLE hasn't finished initializing
    Serial.println("Waiting for ArduinoBLE");
    while(1);
  }

  BLE.setLocalName("Brian_Cameron_Akshay");
  BLE.setAdvertisedService(newService); // add service as discoverable
  newService.addCharacteristic(readChar); // add read characteristic
  newService.addCharacteristic(writeChar); // add write characteristic
  BLE.addService(newService); // add service to device

  // initialize characteristic values
  readChar.writeValue(0);
  writeChar.writeValue(0);

  // confirm that device is properly set up and advertised
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

        // trigger sensor only when central writes to peripheral
        if (writeChar.written()) {
          if (writeChar.value()) {

            // send trigger pulse to peripheral
            digitalWrite(trigPin, LOW);
            delayMicroseconds(2); // good practice to send low state to peripheral first
            digitalWrite(trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);

            // calculate output pulse duration
            duration = pulseIn(echoPin, HIGH);

            // calibrate sensor
            distanceCm = duration / 58;
            distanceInch = duration / 148;
            Serial.print("Distance: ");
            Serial.print(distanceCm);
            Serial.print(" cm/");
            Serial.print(distanceInch);
            Serial.println(" in");
            delay(10);

            // read distance value from peripheral
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