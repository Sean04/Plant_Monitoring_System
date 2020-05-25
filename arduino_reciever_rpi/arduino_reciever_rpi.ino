#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(9, 10, 3, 4, 5, 6);
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
  lcd.begin(16,2);
  lcd.print("Ready");
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0,address);
  radio.setChannel(0x76);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
}

void loop() {
  if (radio.available()){
    radio.read(&data, sizeof(data));
    data.moisture = map(data.moisture, 1023, 261, 0, 100);

//  LCD Displays
    lcd.clear();
    lcd.print("Temp: ");
    lcd.print(data.temperature);
    lcd.setCursor(0,1);
    lcd.print("Light: ");
    lcd.print(data.light);

//  Serial data out
    Serial.print("Temperature:"); 
    Serial.print(data.temperature);
    Serial.print(";");
    Serial.print("Light:");
    Serial.print(data.light);
    Serial.print(";");
    Serial.print("Soil_Moisture:");
    Serial.print(data.moisture);
    Serial.print(";");
    Serial.print("Humidity:");
    Serial.print(data.humidity);
    Serial.println(";");
    delay(5000);
  }
}