/*
Fichier de test consommation arduino Nano Moorev-Timer-Camera
Allumage de 8secondes de la led au démarrage du programme
L'envoi du signal de déclenchement durant 5 secondes sur le pin de la gopro est simulé par l'allumage de la led interne de d'arduino Nano
*/

// Librairies utilisées
#include <avr/wdt.h>      // Librairie fournie avec l'IDE arduino (Watch Dog Timer)
#include <avr/sleep.h>    // Librairie fournie avec l'IDE arduino (Sleep)

// Variable qui mémorisera le fait qu'il y a eu une "interruption watchdog", et que le code de la routine associée a été exécuté
volatile bool routineInterruptionExecutee = false;    // Nota : on déclare cette variable en "volatile", pour pouvoir y accéder aussi bien dans le programme, que dans la partie interruption (ISR)

int cycle_left = 7; //diminution de 1 à chaque cycle pour atteindre un temps total avant chaque déclenchement de la gopro de environ 65,5 secondes 

// ========================
// Initialisation programme
// ========================
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialisation de la liaison série
  Serial.begin(9600);
  Serial.println(F("Démarrage du programme…"));
  Serial.println("");

  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(8000);
  digitalWrite(LED_BUILTIN, LOW);

}

// =================
// Boucle principale
// =================
void loop() {

  // À chaque passage dans cette boucle, on check l'état de la variable "routineInterruptionExecutee"
  //    --> si elle est égale à FALSE, alors cela veut dire qu'aucune interruption n'a encore été déclenchée par le chien de garde
  //    --> si elle est égale à TRUE, alors cela veut dire qu'une interruption watchdog a eu lieu
    
  if (!routineInterruptionExecutee) {
    if (cycle_left!=0){
      cycle_left--;
      Serial.print("cycle_left :");
      Serial.println(cycle_left);
    }
    else{
      declenchementGoPro();                   // Allumage de la LED embarquée sur l'Arduino, pendant 3 secondes
      cycle_left = 6;                         //Réinitialisation du compteur
    }
    
    activationDuWatchdog();                         // Configuration du Watch Dog Timer (WDT), pour qu'il produise une interruption au bout de 8 secondes
    miseEnSommeilDuMicrocontroleur();               // Mise du µC en sommeil (il sera réveillé ultérieurement, par l'interruption du watchdog)    
  } else {
    routineInterruptionExecutee = false;            // Si interruption exéctuée, on efface cette info
  }

}


// =========================================
// Routine d'interruption (liée au watchdog)
// =========================================
ISR(WDT_vect) {
  desactivationDuWatchdog();
  routineInterruptionExecutee = true;
}

// ================================
// Routine d'activation du Watchdog
// ================================
void activationDuWatchdog() {

  __asm__ __volatile__ ("wdr");
  
  WDTCSR = (1 << WDCE) | (1 << WDE);          // Déverrouille les bits de configuration du Watchdog (procédure spécifiée par le fabricant)
  
  WDTCSR = 0b01100001;          // Mise à 1 du bit WDIE (autorisant les "interruptions watchdog") ainsi que les bits WDP3 et WDP0, pour une interruption toutes les 8 secondes
}

// ====================================
// Routine de désactivation du Watchdog
// ====================================
void desactivationDuWatchdog() {
  
  // *******************************************************************************
  //  BITS DE CONFIGURATION DU WATCHDOG
  //                           (cf. page 47 du datasheet de l'ATmega328P)
  // *******************************************************************************
  //  Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0
  //  WDIF  | WDIE  | WDP3  | WDCE  |  WDE  | WDP2  | WDP1  | WDP0
  // *******************************************************************************

  WDTCSR = (1 << WDCE) | (1 << WDE);      // Déverrouille les bits de configuration du Watchdog (procédure spécifiée par le fabricant)
  WDTCSR = 0b00000000;                    // Mise à 0 des bits WDIE et WDE, pour désactiver le timer du chien de garde
  
}

// ==========================================
// Routine de mise en sommeil de l'ATmega328P
// ==========================================
void miseEnSommeilDuMicrocontroleur(void) {
  // Au passage, il y a 5 différents modes de mise en sommeil d'un arduino, pour économiser de l'énergie :
  //   - le mode SLEEP_MODE_IDLE (celui qui économise le moins d'énergie)
  //   - le mode SLEEP_MODE_ADC
  //   - le mode SLEEP_MODE_PWR_SAVE
  //   - le mode SLEEP_MODE_STANDBY
  //   - le mode SLEEP_MODE_PWR_DOWN (celui qui économise le plus d'énergie)

  // Ici, on active la mise en sommeil "SLEEP_MODE_PWR_DOWN"
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  // Et on met le µC en sommeil maintenant
  Serial.flush();         // On attend la fin de la transmission des données séries, avant de mettre le microcontrôleur en sommeil
  sleep_mode();

  // …
  // le programme va reprendre à partir d'ici, une fois le microcontrôleur réveillé par le Watchdog (et après avoir exécuté l'interruption correspondante)
  // …

  // Puise on désactive le mode sommeil, et on continue le programme
  sleep_disable();
  
}

// ==============================================
// Routine d'allumage de la LED embarquée arduino
// ==============================================
void declenchementGoPro(void) {
  Serial.println("-----------------------------");
  Serial.println("Allumage de la LED embarquée sur l'arduino (pendant 5 seconde)");

  digitalWrite(LED_BUILTIN, HIGH);
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("Extinction de la LED");

}
