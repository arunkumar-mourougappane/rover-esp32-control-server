#include "RoverServer.h"
#include <EmbeddedWebServer.h>
#include <atomic>

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

CNeoPixel pixels(NEOPIXEL_DATA_PIN, NEOPIXEL_DATA_PIN);

std::atomic<bool> imu_data_ready_flag;

void setup()
{

   Serial.begin(115200);
   Serial.setDebugOutput(true);
   Serial.println();
   pinMode(LED_BUILTIN, OUTPUT);

   pixels.setBrightness(64);

   pixels.SetPixelColor(CNeoPixel::Color(255, 0, 0)); // Set pixel to red
   delay(1000);
   pixels.UpdatePixelColor(CNeoPixel::Color(128, 0, 128), true); // Set pixel to red
   imuSensorQueue = xQueueCreate(10, sizeof(imu_data_t));
   delay(1000);
   pixels.SetPixelColor(CNeoPixel::Color(0, 0, 0)); // Set pixel to red

   // create a task that executes the SensorDataTask() function, with priority 1 and executed on core 0
   xTaskCreatePinnedToCore(SensorDataTask, "Task0", 10000, NULL, 1, &sensor_process_task, 0);
   // create a task that executes the WebServerTask() function, with priority 1 and executed on core 1
   xTaskCreatePinnedToCore(WebServerTask, "Task1", 10000, NULL, 1, &web_handler_task, 1);
}

void loop()
{
}

void SensorDataTask(void *pvParameters)
{
   log_i("Task0 running on core %d\n", xPortGetCoreID());
   // Setup 6DOF Data
   delay(2000);
   imu_data_ready_flag = false;

   Adafruit_LSM6DSOX lsm6dsox;
   TwoWire i2c_wire(0);
   
   // Connect and initialize LSM6DSOx
   i2c_wire.setPins(LSM6DOX_SDA_PIN, LSM6DOX_SCL_PIN);
   while (!lsm6dsox.begin_I2C(LSM6DS_I2CADDR_DEFAULT, &i2c_wire, 0))
   {
      log_e("Failed to initialize LSM6DOX.");
      delay(1000);
   }

   log_i("LSM6DSOX Found!");

   sensors_event_t accel;
   sensors_event_t gyro;
   sensors_event_t temp;

   const uint8_t MAX_LED_WAIT = 5;
   uint8_t led_wait_count = 0;
   bool led_set = false;
   imu_data_t imu_data;
   for (;;)
   {
      // Neo Pixel Blink to say that we are sampling data.
      if ((led_wait_count < 80) && (led_set != true))
      {
         pixels.UpdatePixelColor(CNeoPixel::Color(0, 50, 0)); // Set pixel to red
         led_set = true;
      }
      else if ((led_wait_count >= 5) && (led_set == true))
      {
         pixels.UpdatePixelColor(CNeoPixel::Color(0, 0, 0)); // Set pixel to red
         led_set = false;
         led_wait_count = 0;
      }
      else
      {
         led_wait_count++;
      }

      // read data from IMU Sensor
      lsm6dsox.getEvent(&accel, &gyro, &temp);
#ifdef TELEPLOT_ENABLE
      Serial.printf(">AccX:%0.2f\n", accel.acceleration.x);
      Serial.printf(">AccY:%0.2f\n", accel.acceleration.y);
      Serial.printf(">AccZ:%0.2f\n", accel.acceleration.z);
      Serial.printf(">GyroX:%0.2f\n", gyro.gyro.x);
      Serial.printf(">GyroY:%0.2f\n", gyro.gyro.y);
      Serial.printf(">GyroZ:%0.2f\n", gyro.gyro.z);
#endif
      // Update only if data is not ready / stale.
      if (imu_data_ready_flag == false) {
         imu_data.accX = accel.acceleration.x;
         imu_data.accY = accel.acceleration.y;
         imu_data.accZ = accel.acceleration.z;
         imu_data.gyroX = gyro.gyro.x;
         imu_data.gyroY = gyro.gyro.y;
         imu_data.gyroZ = gyro.gyro.z;
         imu_data.temperature = temp.temperature;
         xQueueSend(imuSensorQueue, &imu_data, pdMS_TO_TICKS(100));
         imu_data_ready_flag = true;
      }
      pixels.UpdatePixelColor(CNeoPixel::Color(0, 0, 0)); // Set pixel to red
      delay(20);
   }
}



void WebServerTask(void *pvParameters)
{
   log_i("Task1 running on core %d", xPortGetCoreID());
   log_i("Setting up Web server handlers");
   CEmbeddedWebServer embeddedServer(80, ROVER_AP_SSID, ROVER_AP_PASS_PHRASE);
   embeddedServer.SetupNetwork();
   embeddedServer.SetUpWebHandlers();
   imu_data_t imu_data;
   log_i("Starting Web Server");
   for (;;)
   {
      // Read only if data is ready.
      if (imu_data_ready_flag == true) {
         if (xQueueReceive(imuSensorQueue, &imu_data, pdMS_TO_TICKS(100)) == pdTRUE) {
            embeddedServer.updateImuData(imu_data);
         }
         imu_data_ready_flag = false;
      }
      embeddedServer.handleClient();
      delay(5);
   }
}
