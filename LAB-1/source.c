
#include <ThingSpeak.h>
#include <WiFi.h>
#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT22

DHT sensorDHT(DHTPIN, DHTTYPE);

// Wi-Fi configuration
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ThingSpeak configuration
const char* writeApiKey = "UU2YVZ9T49YDT7CG";
const char* universityID = "12326324";

WiFiClient client;

unsigned long channelID = 0;

void setup() {
    Serial.begin(115200);
    sensorDHT.begin();

    Serial.print("University ID: ");
    Serial.println(universityID);

    WiFi.begin(ssid, password);

    Serial.print("Connecting to Wi-Fi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("Wi-Fi Not Connected....");
    }

    Serial.println("\nWi-Fi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    ThingSpeak.begin(client);
}

void loop() {
    float temperature = sensorDHT.readTemperature();
    float humidity = sensorDHT.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        delay(2000);
        return;
    }

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    if (WiFi.status() == WL_CONNECTED) {
        ThingSpeak.setField(1, temperature);
        ThingSpeak.setField(2, humidity);

        int response = ThingSpeak.writeFields(channelID, writeApiKey);

        Serial.print("ThingSpeak response: ");
        Serial.println(response);

        if (response == 200) {
            Serial.println("Data uploaded successfully!");
        } else {
            Serial.println("Upload failed.");
        }
    } else {
        Serial.println("Wi-Fi disconnected!");
    }

    delay(20000);
}
