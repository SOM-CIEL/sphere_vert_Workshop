#include "LectureCarte.h"

LectureCarte::LectureCarte(byte ssPin, byte rstPin, String uidAutorise)
    : rfid(ssPin, rstPin)
{
    this->uidAutorise = uidAutorise;
}

void LectureCarte::begin()
{
    // Initialisation SPI
    SPI.begin();

    // Initialisation du RC522
    rfid.PCD_Init();

    Serial.println();
    Serial.println("Lecteur RFID RC522 pret !");
    Serial.println("Approchez une carte...");
}

bool LectureCarte::lireCarte()
{
    // Vérifie si une nouvelle carte est présente
    if (!rfid.PICC_IsNewCardPresent())
    {
        return false;
    }

    // Lit la carte
    if (!rfid.PICC_ReadCardSerial())
    {
        return false;
    }

    Serial.print("UID : ");

    String trame;

    for (byte i = 0; i < rfid.uid.size; i++)
    {
        if (rfid.uid.uidByte[i] < 0x10)
        {
            Serial.print("0");
            trame += "0";
        }

        Serial.print(rfid.uid.uidByte[i], HEX);

        String tmp = String(rfid.uid.uidByte[i], HEX);
        trame += tmp;

        if (i < rfid.uid.size - 1)
        {
            Serial.print(":");
            trame += ":";
        }
    }

    Serial.println();

    // Affiche le type de carte
    MFRC522::PICC_Type type =
        rfid.PICC_GetType(rfid.uid.sak);

    Serial.print("Type : ");
    Serial.println(rfid.PICC_GetTypeName(type));

    Serial.print(trame);

    bool autorise = false;

    if (trame == uidAutorise)
    {
        Serial.println();
        Serial.println("Acces autorise !");
        autorise = true;
    }
    else
    {
        Serial.println();
        Serial.println("Acces refuse !");
    }

    // Arrête la communication avec la carte
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    delay(1000);

    return autorise;
}
