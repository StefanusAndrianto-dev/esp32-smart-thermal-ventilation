#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <ESP32Servo.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


#define DHTPIN 15
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);


#define SERVO_PIN 5

Servo coolingServo;


int ledPins[10] = {13, 12, 14, 27, 26, 25, 33, 32, 4, 16};

void setup() {

  Serial.begin(115200);

  
  dht.begin();

 
  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {

    Serial.println("OLED FAILED");

    while (true);
  }

  coolingServo.attach(SERVO_PIN);
  coolingServo.write(0);

 
  for (int i = 0; i < 10; i++) {

    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }


  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.display();
}

void loop() {


  float temp = dht.readTemperature();

  if (isnan(temp)) {

    Serial.println("DHT ERROR");
    return;
  }

 
  int ledLevel = map((int)temp, 20, 50, 0, 10);

  ledLevel = constrain(ledLevel, 0, 10);

  for (int i = 0; i < 10; i++) {

    digitalWrite(ledPins[i], (i < ledLevel) ? HIGH : LOW);
  }


  int servoAngle = map((int)temp, 20, 50, 0, 180);

  servoAngle = constrain(servoAngle, 0, 180);

  coolingServo.write(servoAngle);

 
  display.clearDisplay();

  display.setCursor(0, 0);
  display.println("Thermal Monitor");

  display.setCursor(0, 15);
  display.print("TEMP : ");
  display.print(temp);
  display.println(" C");

  display.setCursor(0, 30);

  if (temp < 30) {

    display.println("STATUS : NORMAL");

  }
  else if (temp < 40) {

    display.println("STATUS : WARNING");

  }
  else {

    display.println("STATUS : OVERHEAT");
  }

  display.setCursor(0, 45);
  display.print("COOLING : ");
  display.print(servoAngle);
  display.println(" DEG");

  display.display();

 
  Serial.print("TEMP: ");
  Serial.print(temp);
  Serial.print(" C | SERVO: ");
  Serial.println(servoAngle);

  delay(150);
}