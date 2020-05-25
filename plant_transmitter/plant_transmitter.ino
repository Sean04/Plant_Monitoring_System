#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"

//Constatns
#define ONE_WIRE_BUS 3
#define PHOTO_PIN A1
#define MOIST_PIN A2
#define DHT_PIN 5
#define DHTTYPE DHT22

//Temperature sensor objects
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//DHT object
DHT dht(DHT_PIN, DHTTYPE);

//Radio objects
RF24 radio(7,8);
uint8_t address[] = {0xE0, 0xE0, 0xF1, 0xF1, 0xE0};

//Plant data struct package sent over RF24
struct package {
  float temperature;
  float light;
  float humidity;
  float moisture;
};
typedef struct package Package;
Package data;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setChannel(0x76);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();

//Temperature sensor begin()
  sensors.begin();

  dht.begin();
}

void loop() {
  //  Temperature sensors
  sensors.requestTemperatures();
  data.temperature = sensors.getTempCByIndex(0);
  //Photoresistor
  data.light = analogRead(PHOTO_PIN);
  //  Moisture sensor
  data.moisture = analogRead(MOIST_PIN);
  // Humidity sensor
  data.humidity = dht.readHumidity();

//Send the data struct and the size of the data struct
  radio.write(&data, sizeof(data));

  Serial.print("Moisture: "); Serial.println(data.moisture);
  delay(1000);  
}
