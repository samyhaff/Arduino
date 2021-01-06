// include the library code:
#include <LiquidCrystal.h>

volatile int compteur = 999;
volatile bool condition = false;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void timerInit() {
  TCCR1A = 0;
  TCNT1  = 0;
  OCR1A = 16000-1;  //1ms
  TCCR1B = (1<<WGM12) | (1 << CS10); //CTC and clk = Clkio
  TIMSK1 |= (1 << TOIE1);
 interrupts();
}

void setup() {
  pinMode(13,OUTPUT);
  lcd.begin(16, 2); //16 col, 2 lignes
  lcd.print (millis ()) ;
  timerInit() ; // Interruption toutes les 1ms
}

//interruption toutes les 1ms
ISR(TIMER1_OVF_vect)
{
    if (compteur > 0)
        compteur--;
    else {
        compteur = 999;
        condition = true;
    }
}

void loop() {
    if (condition) {
        lcd.setCursor(0, 0);
        lcd.print(millis());
        condition = false
    }
}
