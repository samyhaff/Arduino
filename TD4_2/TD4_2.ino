void setup() {
  pinMode (0, INPUT_PULLUP) ;  // Poussoir
  pinMode (13, OUTPUT) ;       // Led
}

void loop() {
  static uint8_t filtre = 0 ; // filtrage avec 8 bits.
  filtre = (filtre << 1) | digitalRead (0) ;
  if (filtre == 0) {
    digitalWrite (13, HIGH) ;
  }else if (filtre == 0xFF) {
    digitalWrite (13, LOW) ;
  }
  delay (1) ; // ms
}
