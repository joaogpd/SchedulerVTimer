#include "SchedulerVTimer.h"
#include <avr/sleep.h>
#include <LiquidCrystal.h>

#define sensorPin A0
#define sensorPwr 7
#define timerLength 60000 // in ms, 60 seconds
#define UNUSED_VALUE 0

// LCD Setup
const int rs = 9, en = 8, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Global Variables
double tempC = 0; // holds the temperature value
int counter = 0; // counter for debugging 

// Program Functions 
void LCDPrintOut();
void readTemperature();

void setup() {
  // Starting LCD and writing starting message to it
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Iniciando...");
  // Serial Setup (unused in real application)
  // Serial.begin(9600);
  // Temperature Sensor Setup
  analogReference(INTERNAL); // more accurate measure, INTERNAL = 1.1V, default is 5V
  pinMode(sensorPwr, OUTPUT); // pin for temperature sensor power
  pinMode(sensorPin, INPUT); // analog pin for temperature reading
  // Setup Sleep
  set_sleep_mode(SLEEP_MODE_IDLE);
  // Setup Scheduler
  setupTasks(vtimer, readTemperature);
  setupTasks(app, LCDPrintOut);
  initSchedulerVTTimer();
  startVTimer(readTemperature, timerLength);

  readTemperature();

  delay(4000);
}

void loop() { 
  procTasks();
  sleep_enable();
  sleep_mode();
}

// Reads the temperature from a LM35 sensor
void readTemperature() {
  // Turn on sensor
  digitalWrite(sensorPwr, HIGH);
  delay(4); // delay before read

  double reading = analogRead(sensorPin);
  tempC = (reading * (1.1 / 1024.0)) * 100;

  // Turn off sensor
  digitalWrite(sensorPwr, LOW);

  // Restarts timer for task to execute again 
  startVTimer(readTemperature, timerLength);

  // Counter to ensure execution
  counter++;

  // Posts LCD Print task after having temperature read
  postTask(app, LCDPrintOut, UNUSED_VALUE);
}

// Prints out the value of the tempC variable to the LCD, accompanied
// by how many times the sensor was read in the current execution
void LCDPrintOut() {
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(tempC);
  lcd.print("*C");
  lcd.setCursor(0, 1);
  lcd.print("N: ");
  lcd.print(counter);
}