#ifndef MAIN_CPP
#define MAIN_CPP
#include <Arduino.h>
#include <string.h>
#include "sensors/load_cell.cpp"
#include "sd/sd_card.cpp"
#include "data/data.cpp"
#include <math.h>
#include <Wire.h>
// #include "config/definitions.cpp"


using namespace std;

LoadCell loadCell_1;
SD_card sd;
Data data;

void setupSensors();
void setupSDCard();
void setupData();



void setup()
{
  
  // TIMER 1 for interrupt frequency 120.00480019200768 Hz:
  // cli(); // stop interrupts
  // TCCR1A = 0; // set entire TCCR1A register to 0
  // TCCR1B = 0; // same for TCCR1B
  // TCNT1  = 0; // initialize counter value to 0
  // // set compare match register for 120.00480019200768 Hz increments
  // OCR1A = 16665; // = 16000000 / (8 * 120.00480019200768) - 1 (must be <65536)
  // // turn on CTC mode
  // TCCR1B |= (1 << WGM12);
  // // Set CS12, CS11 and CS10 bits for 8 prescaler
  // TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
  // // enable timer compare interrupt
  // TIMSK1 |= (1 << OCIE1A);
  // sei(); // allow interrupts

  
  pinMode(PC13, OUTPUT);
  Serial.begin(115200);


  // setupSensors();
  setupSDCard();  
  // setupData();
}

void loop()
{
  Serial.println("Hello World");
  delay(1000);
  digitalWrite(PC13, HIGH);
  delay(1000);
  digitalWrite(PC13, LOW);
  // Do nothing
}


// setup functions
void setupSensors(){
  loadCell_1 = LoadCell();
  loadCell_1.setupLoadCell(LOADCELL_DOUT_PIN, 
                              LOADCELL_SCK_PIN, 
                              TIMEOUT, 
                              TIMER);   
  loadCell_1.loadCellBegin();
}

void setupSDCard(){
  sd = SD_card();
  sd.setupSD();
}

void setupData(){
  data = Data();
}

// ISR(TIMER1_COMPA_vect){
//   // String data1 = loadCell_1.readLoadString(data, TIMER);
//   // sd.writeSD(TESTFILE, data1);
//   Serial.println(millis());
// }


// void unitTestFile(){
//   delay(10)
//   String fakeData = random(10).toString();
//   sd.writeSD(TESTFILE, fakeData);
// }


#endif