int led = 13;
int bouton1 = 0;
int bouton2 = 1;
int val1;
int val2;

void setup() {
    pinMode(led, OUTPUT);
    pinMode(bouton1, INPUT_PULLUP);
    pinMode(bouton2, INPUT_PULLUP);
}

void loop() {
    val1 = digitalRead(bouton1);
    val2 = digitalRead(bouton2);
    if (val1 == HIGH ^ val2 == HIGH) {
        digitalWrite(led, HIGH);
    }
    else {
        digitalWrite(led, LOW);
    }
}

