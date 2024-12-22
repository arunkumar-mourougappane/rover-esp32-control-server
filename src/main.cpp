#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_LSM6DSOX.h> // LSM6DS for 6-DOF Measurement
#include "NeoPixel.h"

/**
 * @brief  Pins
 */
#define LSM6DOX_SDA_PIN 42
#define LSM6DOX_SCL_PIN 41

/**
 * @brief NeoPixel Pins
 *
 */
#define NEOPIXEL_DATA_PIN 33
#define NEOPIXEL_POWER_PIN 34
/**
 * @brief NeoPixel Count.
 *
 */
#define NEOPIXEL_COUNT 1

/**
 * @brief Task instantiations.
 *
 */
TaskHandle_t sensor_process_task;
TaskHandle_t web_handler_task;

CNeoPixel pixels(NEOPIXEL_DATA_PIN, NEOPIXEL_DATA_PIN);

void Task0code(void *);
void Task1code(void *);

int led_bit = HIGH;

void setup()
{

   Serial.begin(115200);
   Serial.println();
   pinMode(LED_BUILTIN, OUTPUT);

   pixels.setBrightness(64);

   pixels.SetPixelColor(CNeoPixel::Color(255, 0, 0)); // Set pixel to red
   delay(5000);
   pixels.SetPixelColor(CNeoPixel::Color(0, 0, 0)); // Set pixel to red
   // create a task that executes the Task0code() function, with priority 1 and executed on core 0
   xTaskCreatePinnedToCore(Task0code, "Task0", 10000, NULL, 1, &sensor_process_task, 0);
   // create a task that executes the Task0code() function, with priority 1 and executed on core 1
   xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 1, &web_handler_task, 1);
}

void loop()
{
}

void Task0code(void *pvParameters)
{
   Serial.print("Task0 running on core ");
   Serial.println(xPortGetCoreID());
   // Setup 6DOF Data
   delay(2000);
   Adafruit_LSM6DSOX lsm6dsox;
   TwoWire i2c_wire(0);

   // Connect and initialize LSM6DSOx
   i2c_wire.setPins(LSM6DOX_SDA_PIN, LSM6DOX_SCL_PIN);
   while (!lsm6dsox.begin_I2C(LSM6DS_I2CADDR_DEFAULT, &i2c_wire, 0))
   {
      Serial.println("Failed to initialize LSM6DOX.");
      delay(1000);
   }

   Serial.println("LSM6DSOX Found!");

   sensors_event_t accel;
   sensors_event_t gyro;
   sensors_event_t temp;
   const uint8_t MAX_LED_WAIT = 5;
   uint8_t led_wait_count = 0;
   bool led_set = false;
   for (;;)
   {
      if ((led_wait_count < 5) && (led_set != true)) {
         pixels.UpdatePixelColor(CNeoPixel::Color(0, 50, 0)); // Set pixel to red
         led_set = true;
      }
      else if ((led_wait_count >= 5) && (led_set == true)) {
         pixels.UpdatePixelColor(CNeoPixel::Color(0, 0, 0)); // Set pixel to red
         led_set = false;
         led_wait_count = 0;
      }
      else {
         led_wait_count++;
      }
      lsm6dsox.getEvent(&accel, &gyro, &temp);
      Serial.printf(">AccX:%0.2f\n", accel.acceleration.x);
      Serial.printf(">AccY:%0.2f\n", accel.acceleration.y);
      Serial.printf(">AccZ:%0.2f\n", accel.acceleration.z);
      Serial.printf(">GyroX:%0.2f\n", gyro.gyro.x);
      Serial.printf(">GyroY:%0.2f\n", gyro.gyro.y);
      Serial.printf(">GyroZ:%0.2f\n", gyro.gyro.z);
      pixels.UpdatePixelColor(CNeoPixel::Color(0, 0, 0)); // Set pixel to red
      delay(100);
   }
}

void Task1code(void *pvParameters)
{
   Serial.print("Task1 running on core ");
   Serial.println(xPortGetCoreID());

   for (;;)
   {
      pixels.UpdatePixelColor(CNeoPixel::Color(0, 0, 100)); // Set pixel to red
      digitalWrite(BUILTIN_LED, HIGH);
      delay(250);
      digitalWrite(BUILTIN_LED, LOW);
      pixels.UpdatePixelColor(CNeoPixel::Color(0, 0, 0), true); // Set pixel to red
      delay(250);
   }
}
