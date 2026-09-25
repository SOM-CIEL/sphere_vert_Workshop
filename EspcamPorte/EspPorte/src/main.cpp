#include <Arduino.h>
#include <Wire.h>
#include "pitches.h"

// =====================================================
// CONFIGURATION GPIO
// =====================================================

#define redPin1 12
#define redPin2 13

#define RX_PIN 3
#define TX_PIN 1

#define BUZZZER_PIN 2


// =====================================================
// MELODIE 1
// Never gonna give you up
// =====================================================

int melody1[] = {
  NOTE_A5,
  NOTE_E6,
  NOTE_A5,
  NOTE_E6
};

int durations1[] = {
  8,
  8,
  8,
  8
};




// =====================================================
// VARIABLES ALARME
// =====================================================

bool alarmeActive = false;

// Mélodie actuellement jouée
int currentMelody = 0;

// Note actuellement jouée
int currentNote = 0;

// Gestion du temps
unsigned long previousNoteMillis = 0;

// Durée de la note actuelle
unsigned long noteDuration = 0;


// =====================================================
// VARIABLES LED
// =====================================================

bool ledRouge = false;


// =====================================================
// GESTION DES LEDS
// =====================================================

void setColor(int redValue, int greenValue) {

  analogWrite(redPin1, redValue);
  analogWrite(redPin2, greenValue);
}


void ledsOff() {

  setColor(0, 0);
}


void changerLED() {

  ledRouge = !ledRouge;

  if (ledRouge) {

    // Rouge
    setColor(255, 0);

  } else {

    // Vert
    setColor(0, 255);
  }
}


// =====================================================
// OBTENIR LA TAILLE DE LA MELODIE
// =====================================================

int getMelodySize(int melodyNumber) {

  if (melodyNumber == 0) {

    return sizeof(melody1) / sizeof(melody1[0]);

  }
}


// =====================================================
// OBTENIR LA FREQUENCE D'UNE NOTE
// =====================================================

int getNote(int melodyNumber, int noteNumber) {

  if (melodyNumber == 0) {

    return melody1[noteNumber];

  }
}


// =====================================================
// OBTENIR LA DUREE D'UNE NOTE
// =====================================================

int getDuration(int melodyNumber, int noteNumber) {

  if (melodyNumber == 0) {

    return durations1[noteNumber];

  }
}


// =====================================================
// JOUER LA NOTE ACTUELLE
// =====================================================

void playCurrentNote() {

  // Récupération de la fréquence
  int frequency = getNote(
    currentMelody,
    currentNote
  );

  // Récupération du type de durée
  int durationType = getDuration(
    currentMelody,
    currentNote
  );

  // Calcul de la durée en millisecondes
  noteDuration = 1000 / durationType;


  // -----------------------------------------
  // Changement de LED à chaque nouvelle note
  // -----------------------------------------

  changerLED();


  // -----------------------------------------
  // Jouer la note
  // -----------------------------------------

  tone(
    BUZZZER_PIN,
    frequency,
    noteDuration
  );


  // Mémorisation du moment où la note démarre
  previousNoteMillis = millis();
}


// =====================================================
// DEMARRER L'ALARME
// =====================================================

void startAlarm() {

  // Si l'alarme est déjà active,
  // on ne fait rien
  if (alarmeActive) {

    return;
  }


  Serial.println("ALARME START");


  // Activation de l'alarme
  alarmeActive = true;


  // Commencer à la première mélodie
  currentMelody = 0;


  // Commencer à la première note
  currentNote = 0;


  // Initialisation LED
  ledRouge = false;


  // Jouer immédiatement la première note
  playCurrentNote();
}


// =====================================================
// ARRETER L'ALARME
// =====================================================

void stopAlarm() {

  Serial.println("ALARME STOP");


  // Désactivation
  alarmeActive = false;


  // Arrêt du buzzer
  noTone(BUZZZER_PIN);


  // Extinction des LEDs
  ledsOff();
}


// =====================================================
// MISE A JOUR DE L'ALARME
// =====================================================

void updateAlarm() {

  // Si l'alarme n'est pas active,
  // rien à faire
  if (!alarmeActive) {

    return;
  }


  unsigned long currentMillis = millis();


  // -----------------------------------------
  // Vérifier si la note est terminée
  // -----------------------------------------

  if (
    currentMillis - previousNoteMillis
    >= noteDuration * 1.30
  ) {


    // Arrêter la note actuelle
    noTone(BUZZZER_PIN);


    // Passer à la note suivante
    currentNote++;


    // -----------------------------------------
    // Vérifier si la mélodie est terminée
    // -----------------------------------------

    if (
      currentNote >=
      getMelodySize(currentMelody)
    ) {


      // Revenir à la première note
      currentNote = 0;


      // Passer à la mélodie suivante
      currentMelody++;


      // -----------------------------------------
      // Si les 3 mélodies sont terminées
      // on recommence à la première
      // -----------------------------------------

      if (currentMelody >= 3) {

        currentMelody = 0;
      }
    }


    // Jouer la nouvelle note
    playCurrentNote();
  }
}


// =====================================================
// RECEPTION UART
// =====================================================

void checkUART() {

  if (Serial1.available()) {


    // Lire jusqu'au retour à la ligne
    String commande =
      Serial1.readStringUntil('\n');


    // Supprimer espaces et caractères inutiles
    commande.trim();


    Serial.print("Commande UART : ");
    Serial.println(commande);


    // -----------------------------------------
    // COMMANDE ALARME
    // -----------------------------------------

    if (commande == "alarme") {

      startAlarm();
    }


    // -----------------------------------------
    // COMMANDE STOP
    // -----------------------------------------

    else if (commande == "stop") {

      stopAlarm();
    }
  }
}


// =====================================================
// SETUP
// =====================================================

void setup() {


  // -----------------------------------------
  // GPIO
  // -----------------------------------------

  pinMode(redPin1, OUTPUT);
  pinMode(redPin2, OUTPUT);

  pinMode(BUZZZER_PIN, OUTPUT);


  // LEDs éteintes au démarrage
  ledsOff();


  // -----------------------------------------
  // Port série USB
  // -----------------------------------------

  Serial.begin(115200);


  // -----------------------------------------
  // UART Raspberry Pi
  // -----------------------------------------

  Serial1.begin(
    115200,
    SERIAL_8N1,
    RX_PIN,
    TX_PIN
  );


  // -----------------------------------------
  // Message de démarrage
  // -----------------------------------------

  Serial.println();
  Serial.println("============================");
  Serial.println("ESP32-CAM");
  Serial.println("SYSTEME ALARME");
  Serial.println("============================");
  Serial.println("UART en attente...");
}


// =====================================================
// LOOP PRINCIPALE
// =====================================================

void loop() {


  // -----------------------------------------
  // Toujours écouter l'UART
  // -----------------------------------------

  checkUART();


  // -----------------------------------------
  // Faire fonctionner l'alarme
  // -----------------------------------------

  updateAlarm();
}
