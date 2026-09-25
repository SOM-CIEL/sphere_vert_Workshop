#include <Arduino.h>
#include "LectureCarte.h"
#include <Servo.h>
#include <Wire.h>


// ===============================
// SERVO
// ===============================

Servo myservo;

#define servoPin D0

// ===============================
// RC522
// ===============================

// NOUVELLES BROCHES
#define SS_PIN  D3
#define RST_PIN D4

// SPI du RC522 :
// SCK  -> D5
// MISO -> D6
// MOSI -> D7

// ===============================
// LED RGB
// ===============================

// Attention : RX/TX sont utilisés par Serial
#define LED_R D8
#define LED_G D1 //3
#define LED_B D2 //1

const int redPin = LED_R;
const int greenPin = LED_G;
const int bluePin = LED_B;

// ===============================
// RFID
// ===============================

String uidAutorise = "4b:c4:83:75";

LectureCarte lecteur(SS_PIN, RST_PIN, uidAutorise);

// ===============================
// COULEUR LED
// ===============================

void setColor(int redValue, int greenValue, int blueValue)
{
    analogWrite(redPin, redValue);
    analogWrite(greenPin, greenValue);
    analogWrite(bluePin, blueValue);
}


// ===============================
// SETUP
// ===============================

void setup()
{
    Serial.begin(9600);

    // ---------------------------
    // RFID
    // ---------------------------

    lecteur.begin();
    Serial.println("coucouuu");

    // ---------------------------
    // SERVO
    // ---------------------------

    myservo.attach(servoPin, 500, 2400);
    myservo.write(45);

    // ---------------------------
    // LED RGB
    // ---------------------------

    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);

    // LED rouge au démarrage
    setColor(255, 0, 0);

    // ---------------------------
    // LCD
    // ---------------------------

    // D2 = SDA
    // D1 = SCL
    Wire.begin(D2, D1);

}





// ===============================
// LOOP
// ===============================

void loop()
{
    // ---------------------------
    // LED rouge = attente
    // ---------------------------

    setColor(255, 0, 0);

    // ---------------------------
    // Lecture RFID
    // ---------------------------

    bool tcheck = lecteur.lireCarte();

    if (tcheck)
    {
        // Carte autorisée

        myservo.write(199);

        setColor(0, 255, 0);


        delay(3000);

        myservo.write(45);

        setColor(255, 0, 0);

    }

    
}
