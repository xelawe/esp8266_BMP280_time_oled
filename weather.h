#include <ESP8266WiFi.h>

const char* host = "wernhart.net";

void send_val(int iv_sens_nbr, float iv_value) {

  if (iv_sens_nbr == 0) {
    return;
  }

  DebugPrint("connecting to ");
  DebugPrintln(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    DebugPrintln("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/ip/weather_upd?station=1&sensor=";
  url += iv_sens_nbr;
  url += "&value=";
  url += iv_value;

  DebugPrint("Requesting URL: ");
  DebugPrintln(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Authorization: Basic aXA6ZGltcHdmaXUwNw==\r\n" +
               "Connection: close\r\n\r\n");
  delay(10);

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    DebugPrint(line);
  }

  DebugPrintln();
  DebugPrintln("closing connection");
}

