void setup() {
  pinMode(13, OUTPUT); //internal led pin
  
  pinMode(12, OUTPUT); //connect to gopro pin 19

  digitalWrite(12, HIGH); //turn on gopro by sending 3sec signal
  digitalWrite(13, HIGH); //turn on led
  
  delay(3000); // wait 3sec to turn on gopro
  
  digitalWrite(12, LOW); //end of 3sec signal
  digitalWrite(13, LOW); //turn off led
}

void loop() {}
