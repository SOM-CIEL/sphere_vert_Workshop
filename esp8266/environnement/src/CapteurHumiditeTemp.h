#pragma once

class CapteurHumiditeTemp{

    private :

    float humidite;
    float temperature;

    public : 
    static void Begin();
    void CalculHumiditeTemp();
    float GetHumidite();
    float GetTemperature();
};