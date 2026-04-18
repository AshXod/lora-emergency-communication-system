#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

#define BUTTON 4

float lat = 0;
float lon = 0;

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON, INPUT_PULLUP);

  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);

  LoRa.setPins(5, 14, 26);   // CS, RST, DIO0

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed!");
    while (1);
  }

  Serial.println("Transmitter Ready");
}

void loop() {

  // Read GPS data
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  // Get location
  if (gps.location.isValid()) {
    lat = gps.location.lat();
    lon = gps.location.lng();

    Serial.print("Latitude: ");
    Serial.println(lat, 6);

    Serial.print("Longitude: ");
    Serial.println(lon, 6);
  }

  // Button press
  if (digitalRead(BUTTON) == LOW) {

    if (lat != 0 && lon != 0) {

      String message = String(lat, 6) + "," + String(lon, 6);

      Serial.print("Sending: ");
      Serial.println(message);

      LoRa.beginPacket();
      LoRa.print(message);
      LoRa.endPacket();

      Serial.println("Packet Sent");
    } 
    else {
      Serial.println("GPS not ready");
    }

    delay(3000);
  }
}
