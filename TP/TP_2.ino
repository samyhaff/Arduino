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
volatile int pot = 0; // valeur du potentiometre
volatile int etat = 1; // marche

void motorInit()
{
     pinMode(11, OUTPUT);
     pinMode(12, OUTPUT);
}

void motorSet(int commande)
{
    analogWrite(11, commande);
    digitalWrite(12, 1);
}

void setup()
{
    motorInit();
    pinMode(A0, INPUT);
    pinMode(A1, INPUT_PULLUP); // bouton stop
    pinMode(13, OUTPUT);
    lcd.begin(16, 2); //16 col, 2 lignes
    timerInit() ; // Interruption toutes les 5 ms
}

//interruption @200Hz
ISR(TIMER1_OVF_vect)
{
    if (!digitalRead(A1)) 
        etat = (etat + 1) % 2; // l'état change lors d'un appui sur le bouton
}

void loop()
{
    if (etat) {
        pot = analogRead(A0);
        consigne = map(pot, 0, 1023, 0, 255); // on rammène la valeur dans l'intervale [0,255]
        lcd.setCursor (0, 0); // Colonne 0, ligne 0
        lcd.print(consigne);
        lcd.setCursor (0, 1);
        lcd.print(map(consigne, 0, 250, 0, 5));
    }
    else
        consigne = 0;
    lcd.clear();
    motorSet(consigne);
}
