#include <DHT.h>

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
#define DHTPIN 5      // what digital pin we're connected to
#define DHTPOWERPIN 14     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
float dhttemp;
float dhthum;

void init_dht22() {
  pinMode(DHTPOWERPIN, OUTPUT);
  digitalWrite(DHTPOWERPIN, LOW);
}

void get_dht22() {

  digitalWrite(DHTPOWERPIN, HIGH);
  delay(500);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();

  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  digitalWrite(DHTPOWERPIN, LOW);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    DebugPrintln("Failed to read from DHT sensor!");
    return;
  }

  dhthum = h;
  dhttemp = t;

  DebugPrint("Humidity: ");
  DebugPrint(h);
  DebugPrint(" %\t");
  DebugPrint("Temperature: ");
  DebugPrint(t);
  DebugPrint(" *C ");
  DebugPrintln("");

  //  delay(1000);
  send_val(gv_sens_nbr_t, t);
  send_val(gv_sens_nbr_h, h);


}

