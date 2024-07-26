//practica cuatro...

/*
   Fundacion Kinal
   Centro educativo tecnico laboral Kinal
   Quinto perito
   Quinto electronica
   Codigo Tecnico: EB5BV
   Curso: Taller de electronica digital y reparacion de computadoras I
   Proyecto: practica 4
   Dev:Damasco Oxcal Vasquez
   Fecha: 26 de julio.
*/

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define GREEN_LED 2
#define RED_LED 3
const int pinBuzzer = 4; // Pin para el buzzer

MFRC522 mfrc522(SS_PIN, RST_PIN); 

byte registeredTag[] = {0x9B, 0x99, 0x84, 0x13}; // Tag RFID registrado 

void setup() {
  Serial.begin(9600); // Iniciar comunicación serial
  SPI.begin();        // Iniciar bus SPI
  mfrc522.PCD_Init(); // Iniciar MFRC522

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);

  Serial.println("Sistema de acceso RFID listo.");
}

void loop() {
  // Revisa si hay nuevas tarjetas presentes
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Intenta leer la tarjeta
  if (!mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Error al leer la tarjeta.");
    return;
  }

  // Mostrar el UID leído
  Serial.print("UID leído: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Comprueba si el UID leído es el registrado
  if (compareTags(mfrc522.uid.uidByte, registeredTag)) {
    Serial.println("Tag registrado detectado.");
    openDoor();
  } else {
    Serial.println("Tag no registrado detectado.");
    accessDenied();
  }

  mfrc522.PICC_HaltA(); // Pone la tarjeta en estado Halt
}

bool compareTags(byte *tag1, byte *tag2) {
  for (byte i = 0; i < 4; i++) {
    if (tag1[i] != tag2[i]) {
      return false;
    }
  }
  return true;
}

void openDoor() {
  digitalWrite(GREEN_LED, HIGH);
  tone(pinBuzzer, 200); // Generar tono de 200Hz
  delay(4000);          // Esperar 4 segundos
  digitalWrite(GREEN_LED, LOW);
  noTone(pinBuzzer);    // Detener tono
}

void accessDenied() {
  digitalWrite(RED_LED, HIGH);
  tone(pinBuzzer, 400); // Generar tono de 400Hz
  delay(4000);          // Esperar 4 segundos
  digitalWrite(RED_LED, LOW);
  noTone(pinBuzzer);    // Detener tono
}
