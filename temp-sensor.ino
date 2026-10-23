#include <WiFi.h>
#include <DHT.h>
#include <ESP32MQTTClient.h>
#include <math.h>

// -------------------- Sensor --------------------
namespace Sensor {
  constexpr uint8_t DHT_PIN = D2;
  constexpr uint8_t DHT_TYPE = DHT11;

  DHT dht(DHT_PIN, DHT_TYPE);

  void initSensor() {
    dht.begin();
    delay(1000);
  }

  float getTemperature() {
    return dht.readTemperature();
  }
}

// -------------------- Wi-Fi --------------------
namespace Network {
  constexpr char WIFI_SSID[] = "";
  constexpr char WIFI_PASSWORD[] = "";

  void initWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Ansluter till Wi-Fi");

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    Serial.println();
    Serial.println("Wi-Fi anslutet!");
    Serial.print("ESP32 IP-adress: ");
    Serial.println(WiFi.localIP());
  }
}

// -------------------- MQTT --------------------
namespace Mqtt {
  constexpr unsigned long PUBLISH_INTERVAL = 5000;
  unsigned long lastPublishTime = 0;

  constexpr char MQTT_HOST[] = "";
  constexpr uint16_t MQTT_PORT = ;
  constexpr char MQTT_USERNAME[] = "";
  constexpr char MQTT_PASSWORD[] = "";
  constexpr char MQTT_PUB_TOPIC[] = "";

  ESP32MQTTClient mqttClient;

  void initMqtt() {
    mqttClient.setURL(
      MQTT_HOST,
      MQTT_PORT,
      MQTT_USERNAME,
      MQTT_PASSWORD
    );

    mqttClient.setMqttClientName("");
    mqttClient.loopStart();
  }

  void publishTemperature() {
    if (!mqttClient.isConnected()) {
      return;
    }

    unsigned long currentTime = millis();

    if (currentTime - lastPublishTime < PUBLISH_INTERVAL) {
      return;
    }

    lastPublishTime = currentTime;

    float temperature = Sensor::getTemperature();

    if (isnan(temperature)) {
      Serial.println("Kunde inte lasa temperaturen.");
      return;
    }

    char payload[128];

  
    snprintf(
      payload,
      sizeof(payload),
      "{\"deviceId\":\"sensor-01\",\"value\":%.1f,\"unit\":\"°C\"}",
      temperature
    );


    mqttClient.publish(MQTT_PUB_TOPIC, payload, 0, false);

    Serial.print("Publicerad temperatur: ");
    Serial.println(payload);
  }
}

void onMqttConnect(esp_mqtt_client_handle_t client) {}

#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
esp_err_t handleMQTT(esp_mqtt_event_handle_t event) {
  Mqtt::mqttClient.onEventCallback(event);
  return ESP_OK;
}
#else
void handleMQTT(
  void *handlerArgs,
  esp_event_base_t base,
  int32_t eventId,
  void *eventData
) {
  auto event = static_cast<esp_mqtt_event_handle_t>(eventData);
  Mqtt::mqttClient.onEventCallback(event);
}
#endif

// -------------------- Programstart --------------------
void setup() {
  Serial.begin(115200);
  delay(2000);

  Sensor::initSensor();
  Network::initWiFi();
  Mqtt::initMqtt();
}

void loop() {
  Mqtt::publishTemperature();
  delay(10);
}