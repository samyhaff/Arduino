//enum not supported by tinkercad?
const int OFF     = 0;
const int PUSH    = 1;
const int ON      = 2;
const int RELEASE = 3;
int state = OFF;

//retourne l'état du bp
int gestionBP0(state){
  static int etat = state;
  switch(etat) {
    case OFF:
      if (! digitalRead(0)) {
        etat = PUSH;
      } 
      break;
    case PUSH:
      etat = ON;
      break;
    case ON:
    if (digitalRead(0)) {
        etat = RELEASE;
    }
      break;
    case RELEASE:
      etat = OFF;
      break;
  }
  return etat;
}

void setup() {
  pinMode(13,OUTPUT);       //LED
}

void loop() {
    state = gestionBP0(state);
    switch(etat) {
      case OFF: 
        break; 
      case ON:
        break; 
      case PUSH: 
        digitalRead(13, HIGH);
        break; 
      case RELEASE: 
        digitalRead(13, LOW);
        break; 
    }
    delay(1);
}
