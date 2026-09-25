#ifndef LECTURE_CARTE_H
#define LECTURE_CARTE_H

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

class LectureCarte
{
private:
    MFRC522 rfid;
    String uidAutorise;

public:
    LectureCarte(byte ssPin, byte rstPin, String uidAutorise);

    void begin();
    bool lireCarte();
};

#endif
