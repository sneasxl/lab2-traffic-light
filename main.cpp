#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_CAP1188.h>

// Define LED Pins
#define RED_LED 25
#define YELLOW_LED 26
#define GREEN_LED 27
#define BUZZER 33

// Initialize CAP1188 Sensor
Adafruit_CAP1188 cap = Adafruit_CAP1188();

// Timer variables
unsigned long greenStartTime = 0;
bool isGreen = false;

void setup() {
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    
    Serial.begin(115200);
    
    if (!cap.begin()) {
        Serial.println("CAP1188 not found. Check wiring!");
        while (1);
    }
    
    // Start in Red, but transition immediately to Green
    digitalWrite(RED_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    delay(10000);  // Red remains for 10 seconds

    digitalWrite(YELLOW_LED, HIGH);
    delay(2000);  // Red-Yellow transition

    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    greenStartTime = millis();
    isGreen = true;
}

void buzz(int frequency, int duration, int pause) {
    tone(BUZZER, frequency);
    delay(duration);
    noTone(BUZZER);
    delay(pause);
}

void loop() {
    // Green Light Phase (stays green until button is pressed)
    while ((millis() - greenStartTime) < 5000 || !cap.touched()) {
        buzz(1000, 500, 1500);  // Green buzzer pattern
    }

    // Transition to Yellow
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    delay(2000);

    // Transition to Red
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    for (int i = 0; i < 20; i++) { // 10 sec buzzer pattern for red
        buzz(1000, 250, 250);
    }
    delay(10000);

    // Red-Yellow Transition
    digitalWrite(YELLOW_LED, HIGH);
    delay(2000);

    // Switch back to Green
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    greenStartTime = millis();
}
