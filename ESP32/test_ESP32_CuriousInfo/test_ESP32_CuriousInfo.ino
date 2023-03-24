/*
Script de test ARE Moorev-Timer-Caméra hibernation ESP32 : Allume la led intégré pendant 5 secondes toutes les 60 secondes
*/

#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP  60

void setup(){
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.println("----------------------");
  Serial.println("Allumage Led pour 5 secondes");
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(5000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW


   //Configuration du timer
   esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
   Serial.println("ESP32 réveillé dans " + String(TIME_TO_SLEEP) + " secondes");

   delay(100); //Pour être sur que le texte à fini de s'afficher
   
   esp_deep_sleep_start();
}

void loop(){}
