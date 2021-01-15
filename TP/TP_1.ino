#include <LiquidCrystal.h>

void timerInit() { //5ms
  TCCR1A = 0;
  TCNT1  = 0;	
  OCR1A = 10000-1;
  TCCR1B = (1<<WGM12) | (1 << CS11); //CTC and clk = Clkio/8
  TIMSK1 |= (1 << TOIE1);
 interrupts();
}

// bibliothèque de gestion du LCD
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

volatile int consigne = 100;

int bouton_moins = A0;
int bouton_plus = A2;
int bouton_arret = A1;

// définition des états des automates
const int PUSH = 1; // état correspondant à l'appui sur le bouton +
const int NEUTRE = 2; // état neutre (lorsqu'il ne se passe rien)
const int MAX = 3; // état signifiant que consigne est proche de la llimite de 255
const int PUSH2 = 4; // état correspondant à l'appui sur le bouton -
const int ARRET = 5; // état correspond à l'arrêt 
volatile int state = NEUTRE; // on commence à l'état neutre

int gestionBouton1(int s) {
    int etat = s;
    switch(etat) {
        case ARRET:
            // sortie de l'état d'arrêt lors de l'appui
            if (!digitalRead(bouton_arret)) {
                etat = (consigne <= (int) 255 / 1.1) ? NEUTRE : MAX; // retour vers le bon état en fonction de consigne
            }
            // sortie de l'état lorsque la consigne vaut 0, on choisit de la repasser à 100
            if (consigne == 0) {
                consigne = 100;
                etat = NEUTRE;
            }
            break;
        default:
            if (!digitalRead(bouton_arret)) {
                etat = ARRET;
            }
            break;
    }
    return etat;
}

int gestionBouton2(int s) {
    int etat = s;
    switch(etat) {
        case NEUTRE:
            // passage à l'état PUSH lors d'un appui sur le bouton + et si consigne est pas trop proche de la limite
            if (!digitalRead(bouton_plus) && consigne < (int) 255 / 1.1) {
                etat = PUSH;
            }
            if (consigne >= (int) 255. / 1.1) {
                etat = MAX;
            }
            break;
        case PUSH:
            etat = NEUTRE;
            break;
        case MAX:
            break;
    }
    return etat;
}

int gestionBouton0(int s) {
    int etat = s;
    switch (etat) {
        case NEUTRE:
            if (!digitalRead(bouton_moins)) {
                etat = PUSH2;
            }
            break;
        case PUSH2:
            etat = NEUTRE;
      		break;
        case MAX:
            if (!digitalRead(bouton_moins)) {
                etat = PUSH2;
            }
    }
    return etat;
}

// définition des pin pour controler le moteur
void motorInit()
{
   pinMode(11, OUTPUT);
   pinMode(12, OUTPUT);
}

// controle du moteur 
void motorSet(int commande)
{
   analogWrite(11, commande);
   digitalWrite(12, 1);
}

// définition des pin et setup du LCD
void setup()
{
  motorInit();
  pinMode(bouton_plus, INPUT_PULLUP);
  pinMode(bouton_moins, INPUT_PULLUP);
  pinMode(bouton_arret, INPUT_PULLUP);
  pinMode(13, OUTPUT); // on utilise cette led pour voir l'arrêt 
  lcd.begin(16, 2); //16 col, 2 lignes
  timerInit() ; // Interruption toutes les 5 ms
}

//interruption @200Hz
ISR(TIMER1_OVF_vect)
{
    state = gestionBouton1(state);
    if (state == ARRET) {
        digitalWrite(13, HIGH);
        consigne = consigne * 0.95; // baisse de 5% toutes les 5ms
    }
    else {
        digitalWrite(13, LOW);
    }
    state = gestionBouton2(state);
    switch (state) {
        case PUSH:
            consigne = consigne * 1.1; // hausse de 10%
            break;
        case MAX:
            consigne = 255; // saturation à 255
            break;
        default:
            break;
    }
    state = gestionBouton0(state);
    switch (state) {
        case PUSH2:
            consigne = consigne * 0.9; // baisse de 10%
            break;
        default:
            break;
    }
}

void loop()
{
  lcd.setCursor (0, 0); // Colonne 0, ligne 0
  lcd.clear();
  lcd.print(consigne);
  motorSet(consigne);
}
