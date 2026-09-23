#include <Arduino.h>
#include<ESP8266WiFi.h>
#include<ESP8266HTTPClient.h>
#include<WiFiClient.h>
#include "CapteurHumiditeTemp.h"
#include <PubSubClient.h>

const char* ssid = "Pixel_3808";
const char* password = "pixeldejojo";

const char* mqtt_server = "10.230.174.77";
const int mqtt_port = 1883;

const char* mqtt_user = "esp8266_securite";
const char* mqtt_password = "esp8266_securite";

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
 Serial.begin(115200);
 CapteurHumiditeTemp::Begin();
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.print("Connecté !");

  client.setServer(mqtt_server, mqtt_port);

  while (!client.connected()) {
    Serial.println("Connexion MQTT...");

    if(client.connect("esp8266_securite", mqtt_user, mqtt_password)) {
      Serial.println("MQTT connecte");
    } else {
    Serial.println("Erreur MQTT : ");
    Serial.println(client.state());
    delay(2000);
    }
  }
}


CapteurHumiditeTemp updateData;
void loop() {
  // Wait a few seconds between measurements.
  delay(1000);

  // verif wifi
  if(!client.connected()) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Wi-Fi déconnecté, reconnexion...");

        WiFi.disconnect();
        WiFi.begin(ssid, password);

        unsigned long debut = millis();

        while (WiFi.status() != WL_CONNECTED && millis() - debut < 10000) {
            delay(500);
            Serial.print(".");
        }

        Serial.println();

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("Wi-Fi reconnecté");
        } else {
            Serial.println("Échec reconnexion Wi-Fi");
            return;
        }
    }
  }

  // verif mqtt
  if (!client.connected()) {
    Serial.println("MQTT déconnecté, reconnexion...");

    if (client.connect("esp8266_securite", mqtt_user, mqtt_password)) {
      Serial.println("MQTT reconnecté");
    }
    else {
      Serial.print("Échec MQTT : ");
      Serial.println(client.state());
      delay(2000);
      return;
    }
  }


  client.loop();

  updateData.CalculHumiditeTemp();
  float humidite = updateData.GetHumidite();
  float temperature = updateData.GetTemperature();

  Serial.print("MQTT : ");
  Serial.println(client.connected() ? "CONNECTE" : "DECONNECTE");


  Serial.print(humidite);
  Serial.println(temperature);

  client.publish("vaisseau/environnement/temperature", String(temperature).c_str());
  client.publish("vaisseau/environnement/humidite", String(humidite).c_str());
}