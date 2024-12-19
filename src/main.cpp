#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_LSM6DSOX.h> // LSM6DS for 6-DOF Measurement

TaskHandle_t sensor_process_task;
TaskHandle_t web_handler_task;

void Task0code(void*);
void Task1code(void*);

int led_bit = HIGH;

/**
 * I2C Pins
 */
#define LSM6DOX_SDA_PIN 42
#define LSM6DOX_SCL_PIN 41

void setup()
{
   Serial.begin(115200);
   Serial.println();
   pinMode(LED_BUILTIN, OUTPUT);

   // create a task that executes the Task0code() function, with priority 1 and executed on core 0
   xTaskCreatePinnedToCore(Task0code, "Task0", 10000, NULL, 1, &sensor_process_task, 0);
   // create a task that executes the Task0code() function, with priority 1 and executed on core 1
   xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 1, &web_handler_task, 1);
}

void loop()
{
   digitalWrite(LED_BUILTIN, led_bit);
   led_bit = !led_bit;
   delay(500);
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
   sensors_event_t mag_event; 

   for (;;) {
      lsm6dsox.getEvent(&accel, &gyro, &temp);
      Serial.printf(">AccX:%0.2f\n", accel.acceleration.x);
      Serial.printf(">AccY:%0.2f\n", accel.acceleration.y);
      Serial.printf(">AccZ:%0.2f\n", accel.acceleration.z);
      Serial.printf(">GyroX:%0.2f\n", gyro.gyro.x);
      Serial.printf(">GyroY:%0.2f\n", gyro.gyro.y);
      Serial.printf(">GyroZ:%0.2f\n", gyro.gyro.z);
      delay(100);
   }

}

void Task1code(void *pvParameters)
{
   Serial.print("Task1 running on core ");
   Serial.println(xPortGetCoreID());

   for (;;)
   {
      digitalWrite(BUILTIN_LED, HIGH);
      delay(250);
      digitalWrite(BUILTIN_LED, LOW);
      delay(250);
   }
}
