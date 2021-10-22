#include <Arduino.h>
#include <string.h>
#define onboard 13


int count = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(onboard, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(onboard, LOW);
  delay(1000);
  digitalWrite(onboard, HIGH);
  delay(1000);
  Serial.println("loop completed: " + std::to_string(count));
  count = count + 1;
}