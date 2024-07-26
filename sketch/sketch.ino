#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <WebServer.h>

const char* ssid = "Redmi Note 12";        // Ganti dengan SSID WiFi Anda
const char* password = "jenengkuu";   // Ganti dengan password WiFi Anda

const String serverName = "http://192.168.105.133:5000"; // Endpoint API (jika diperlukan)

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
WebServer httpServer(80);

const int buzzerPin = 32;  // Contoh pin buzzer (sesuaikan dengan pin yang Anda gunakan)
bool alarmOn = false;

void setup() {
  Wire.begin(26, 27);
  Serial.begin(9600);
  Serial.println("Adafruit MLX90614 test");
  mlx.begin();

  pinMode(buzzerPin, OUTPUT);  // Set pin buzzer sebagai output

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

}

void loop() {
  httpServer.handleClient();

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Specify the URL and method
    http.begin(serverName + "/sensor");
    http.addHeader("Content-Type", "application/json"); // Specify content-type

    // Read temperatures
    float ambientTempC = mlx.readAmbientTempC();
    float objectTempC = mlx.readObjectTempC();
    float ambientTempF = mlx.readAmbientTempF();
    float objectTempF = mlx.readObjectTempF();

    // Create a JSON object with temperature data
    String jsonData = String("{\"ambient_temp_c\":") + ambientTempC +
                      ",\"ambient_temp_f\":" + ambientTempF +
                      ",\"object_temp_c\":" + objectTempC +
                      ",\"object_temp_f\":" + objectTempF + "}";

    // Send the POST request
    http.POST(jsonData);

    // Handle the response
    // if (httpResponseCode > 0) {
    String response = http.getString();
    if (response == "1") {
      if (!alarmOn) startAlarm();
      alarmOn = true;
    }else alarmOn = false;
    // Serial.print("Response: ");
    Serial.println(response);
    // } else {
    //   Serial.print("Error on sending POST: ");
    //   Serial.println(httpResponseCode);
    // }

    // End the connection
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(1000);
}

void startAlarm(){
  digitalWrite(buzzerPin, HIGH);
  delay(2000);
  digitalWrite(buzzerPin, LOW);
  Serial.println("Alarm");
  alarmOn = false;
}