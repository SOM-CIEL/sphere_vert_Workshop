#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "CapteurHumiditeTemp.h"
#include <PubSubClient.h>
#include <Wire.h>
#include <BH1750.h>

#define redPin D5
#define greenPin D6
#define bluePin D7

const char* ssid = "horizon";
const char* password = "horizon69";

const char* mqtt_server = "10.42.0.1";
const int mqtt_port = 1883;

const char* mqtt_user = "esp8266_securite";
const char* mqtt_password = "esp8266_securite";

const int co2Pin = D0;

BH1750 lightMeter;

WiFiClient espClient;
PubSubClient client(espClient);

void setColor(int redValue, int greenValue, int blueValue)
{
    analogWrite(redPin, redValue);
    analogWrite(greenPin, greenValue);
    analogWrite(bluePin, blueValue);
}


void setup() {
  Serial.begin(115200);

  CapteurHumiditeTemp::Begin();
  pinMode(co2Pin, INPUT);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  setColor(0, 0, 0);

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

  // Initialisation I2C
  // D2 = SDA
  // D1 = SCL
  Wire.begin(D2, D1);

  // Initialisation du BH1750
  if (lightMeter.begin()) {
    Serial.println("BH1750 initialise !");
  } else {
    Serial.println("Erreur : BH1750 non detecte !");
  }
}


CapteurHumiditeTemp updateData;

void loop() {
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
    } else {
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

  unsigned long duree_us = pulseIn(co2Pin, HIGH, 2000000UL);

  if(duree_us > 0) {
    float th_ms = duree_us / 1000.0;

    float ppmCO2 = 5000.0 * (th_ms - 2.0) / 1000.0;

    if(ppmCO2 < 0) ppmCO2 = 0;
    if(ppmCO2 > 5000) ppmCO2 = 5000;

    client.publish("vaisseau/environnement/CO2", String(ppmCO2).c_str());
  }
  else {
    Serial.println("pas de signal, vérifier cablage D0");
  }

  Serial.print("MQTT : ");

  Serial.println(client.connected() ? "CONNECTE" : "DECONNECTE");


  Serial.print("Humidite : ");
  Serial.print(humidite);

  Serial.print(" % | Temperature : ");
  Serial.print(temperature);

  Serial.println(" °C");


  // Lecture du BH1750
  float lux = lightMeter.readLightLevel();

  Serial.print("Luminosité : ");
  Serial.print(lux);
  Serial.println(" lux");

  if (lux <= 100.0) {setColor(255, 255, 255);} else {setColor(0, 0, 0);}

  client.publish("vaisseau/environnement/temperature", String(temperature).c_str());
  client.publish("vaisseau/environnement/humidite", String(humidite).c_str());
  client.publish("vaisseau/environnement/luminosite", String(lux).c_str());
}