#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

float gv_temp_bme280;
float gv_humi_bme280;

void init_bme280() {
  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  Wire.begin(D3, D4);
  status = bme.begin(0x76, &Wire);
  if (!status) {
//    display.println("Could not find a valid BME280 sensor, check wiring!");
//    display.update();
    while (1) {
      delay(100);
    }
  }
}

void get_bme280() {
  gv_temp_bme280 = bme.readTemperature();
  gv_humi_bme280 = bme.readHumidity();
}

