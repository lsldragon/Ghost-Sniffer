#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN 5
#define PIXEL_NUM 1
#define ADC_PIN A0

#define DELAYVAL 500

Adafruit_NeoPixel pixels(PIXEL_NUM, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void init_sys();
void read_adc();
void off_ws2812b();
void goto_deep_sleep();

void init_sys()
{

  Serial.println("Initing WS2812B...");

  pixels.clear(); // Set all pixel colors to 'off'

  pixels.setPixelColor(0, pixels.Color(255, 0, 0)); // R
  pixels.show();
  delay(DELAYVAL);

  pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // G
  pixels.show();
  delay(DELAYVAL);

  pixels.setPixelColor(0, pixels.Color(0, 0, 255)); // B
  pixels.show();
  delay(DELAYVAL);

  off_ws2812b();

  delay(1500); // off led

  Serial.println("Initing WS2812B Done ...");
}

void off_ws2812b()
{
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // off
  pixels.show();
}

void read_adc()
{
  Serial.println("Init ADC....");

  int adc_value = analogRead(ADC_PIN);
  Serial.printf("ADC Value = %d \n", adc_value);

  if (adc_value <= 242) // ghost -> R
  {
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  }

  if ((adc_value > 242) && (adc_value <= 243)) // normal -> G
  {
    pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  }

  if (adc_value > 243) // angel -> B
  {
    pixels.setPixelColor(0, pixels.Color(0, 0, 255));
  }

  pixels.show();
  delay(5000);

  Serial.println("Read ADC Done....");
}

void goto_deep_sleep()
{
  Serial.println("Go to sleep");
  off_ws2812b();
  ESP.deepSleep(0);
}

void setup()
{
  Serial.begin(9600);
  Serial.println();

  pixels.begin();
  pixels.setBrightness(128);

  init_sys();

  read_adc();

  goto_deep_sleep();
}

void loop()
{
}
