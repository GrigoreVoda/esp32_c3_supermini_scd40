#include <Wire.h>
#include <SensirionI2CScd4x.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// -------- Pins --------
#define SDA_PIN 8
#define SCL_PIN 9

#define LED_RED    2
#define LED_YELLOW 3
#define LED_GREEN  4

// -------- Display --------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
SensirionI2cScd4x scd4x;

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  // OLED
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // SCD41
  scd4x.begin(Wire, 0x62);   // <<< REQUIRED
  scd4x.stopPeriodicMeasurement();
  delay(500);
  scd4x.startPeriodicMeasurement();
}

void loop() {
  uint16_t co2 = 0;
  float temperature = 0.0;
  float humidity = 0.0;
  bool dataReady = false;

  scd4x.getDataReadyStatus(dataReady);

  if (dataReady) {
    scd4x.readMeasurement(co2, temperature, humidity);

    // LEDs
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, LOW);

    if (co2 > 1500) {
      digitalWrite(LED_RED, HIGH);
    } else if (co2 > 1200) {
      digitalWrite(LED_YELLOW, HIGH);
    } else {
      digitalWrite(LED_GREEN, HIGH);
    }

    // Display
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print("CO2");

    display.setCursor(0, 20);
    display.print(co2);
    display.print("ppm");

    display.setTextSize(1);
    display.setCursor(0, 45);
    display.print("T:");
    display.print(temperature, 1);
    display.print("C ");

    display.print("H:");
    display.print(humidity, 0);
    display.print("%");

    display.display();
  }

  delay(1000);
}

