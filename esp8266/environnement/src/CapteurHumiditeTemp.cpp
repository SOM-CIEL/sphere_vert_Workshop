#include "CapteurHumiditeTemp.h"
#include <DHT.h>
#define DHTPIN D3 //pin du esp32
#define DHTTYPE DHT11 //pin du capteur

DHT dht(DHTPIN, DHTTYPE);

void CapteurHumiditeTemp::Begin()
{
    dht.begin();
}

void CapteurHumiditeTemp::CalculHumiditeTemp()
{
    //créer variable h et t et appeler la classe dht et sa méthode read
    this->humidite = dht.readHumidity();
    this->temperature = dht.readTemperature();

}

float CapteurHumiditeTemp::GetHumidite()
{

    return this->humidite;
};

float CapteurHumiditeTemp::GetTemperature()
{
    return this->temperature;
}