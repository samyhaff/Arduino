void timerInit() { //500ms
    TCCR1A = 0;
    TCNT1  = 0;
    OCR1A = 31250-1;
    TCCR1B = (1<<WGM12) | (1 << CS12); //CTC and clk = Clkio/256
    TIMSK1 |= (1 << TOIE1);
    interrupts();
}

volatile int led = 0;

void setup() {
    pinMode(13, OUTPUT);
    for (int i = 3; i <= 7; i++)
        pinMode(i, OUTPUT);
    timerInit() ; // Interruption toutes les 500 ms
}


ISR(TIMER1_OVF_vect) //500ms
{
   led = (led + 1) % 5; 
}

void loop() {
    for (int i = 0; i <= 4; i++) {
        if (i == led)
            digitalWrite(i + 3, HIGH); 
        else
            digitalWrite(i + 3, LOW); 
    }
}
