#define GAUCHE 0
#define DROITE 1
#define MARCHE 1
#define ARRET 0

void timerInit() { //500ms
    TCCR1A = 0;
    TCNT1  = 0;
    OCR1A = 31250-1;
    TCCR1B = (1<<WGM12) | (1 << CS12); //CTC and clk = Clkio/256
    TIMSK1 |= (1 << TOIE1);
    interrupts();
}

volatile int motif = 0b10101;
volatile int sens = GAUCHE;
volatile int etat = MARCHE;

void setup() {
    pinMode(13, OUTPUT);
    pinMode(11, INPUT_PULLUP);
    pinMode(8, INPUT_PULLUP);
    for (int i = 3; i <= 7; i++)
        pinMode(i, OUTPUT);
    timerInit() ; // Interruption toutes les 500 ms
}


ISR(TIMER1_OVF_vect) //500ms
{
    if (etat == MARCHE) {
        if (sens == GAUCHE)
            motif = ((motif & 0b10000) >> 4) | ((motif << 1) & 0b011111);
        else
            motif = (motif >> 1) + ((motif & 1) << 4);
    }
}

void loop() {
    if (!digitalRead(11))
        sens = (sens + 1) % 2;
    if (!digitalRead(8))
        etat = (etat + 1) % 2;
    for (int i = 0; i <= 4; i++) {
       int led = (motif & (1 << i)) >> i;
       digitalWrite(3 + i, led);
    }
}
