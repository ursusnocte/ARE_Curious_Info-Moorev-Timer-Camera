void setup() {
pinMode(14, OUTPUT); // Broche A0

for(int i=0; i<10; i++){ // Déclenchement du script de la gopro 10 fois de suite à 30 secondes d'intervalle
    digitalWrite(14, LOW);
    delay(3000);
    digitalWrite(14, HIGH);
    delay(3000);
    
    delay(30000);
  }
}

void loop() {}
