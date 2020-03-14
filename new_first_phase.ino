#include <dht.h>
#include <Adafruit_Sensor.h>
//#define BLYNK_PRINT Serial1
#include <BlynkSimpleStream.h>
#define DHT11_PIN 2
#include "pitches.h"
char auth[] = "FkGM0szdB65cVUH6FZXCpjbVb8OcxpYX";
BlynkTimer timer;
//Creating DHT object
dht DHT;
// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin 1
int temperature_sensorData = 0;
float humidity_sensorData = 0;
int gas_sensorData = 0;
int flame_sensorData = 0;
int button = 0; //to turn the alarm off
void Buzzer_tone();
int buzzerPin = 4;//the pin of the active buzzer attach to pin31
int gas_threshold = 450;
int flame_threshold = 750;
// notes in the melody:
int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4 };

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4};
BLYNK_WRITE(V6)//to turn the alarm off manually
{
  button = param.asInt(); // assigning incoming value from pin V6 to a variable

  // process received value
}
void setup()
{
   pinMode(A1, INPUT);
  pinMode(A0, INPUT);
  pinMode(7, INPUT);
  pinMode(2, INPUT);
  Serial1.begin(9600);
  // Debug console
 
  // Blynk will work through Serial
  // Do not read or write this serial manually in your sketch
  Serial.begin(9600);

  Blynk.begin(Serial, auth);
  timer.setInterval(2000L,myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
  gas_sensorData = analogRead(A0);
  flame_sensorData = analogRead(A1);
   Blynk.virtualWrite(V5, gas_sensorData);
  if (gas_sensorData > gas_threshold || flame_sensorData < flame_threshold)
  {
    Buzzer_tone();
  }
  
}

void Buzzer_tone ()
{
  int check = 1; // iterate over the notes of the melody:
  while (check) {
    for (int thisNote = 4; thisNote < 6; thisNote++)
    {
      // to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1400 / noteDurations[thisNote];
      tone(buzzerPin, melody[thisNote], noteDuration);
      // to distinguish the notes, set a minimum time between them.// the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.05;
      button = digitalRead(7);
      delay(pauseBetweenNotes);
      noTone(buzzerPin);    // stop the tone playing:
      button = digitalRead(7);
    }
    if (button == 0) {
      check = 0;
      Blynk.virtualWrite(V5, 0);
    }
  }

}
void myTimerEvent()
{
  DHT.read11(DHT11_PIN);
  temperature_sensorData = DHT.temperature;
  humidity_sensorData = DHT.humidity;
  Blynk.virtualWrite(V1, humidity_sensorData);
  Blynk.virtualWrite(V2, temperature_sensorData);
  Blynk.virtualWrite(V4, flame_sensorData);
}
