#include <Servo.h>

Servo servodr;
Servo servost;

// ── Pins
#define sdr  2         
#define sst  4          
#define jvrx A0        
#define ms   3         
#define th   A1       
// ── Thermistor constants 
#define RT0  10000      
#define B    3977       

// ── Pico 2W 
#define VCC      3.3    
#define ADC_MAX  4095.0 
#define R        10000  

float RT, VR, TX, T0, VRT;

void setup() {
  servodr.attach(sdr);
  servost.attach(sst);

  servodr.write(0);
  servost.write(0);

  pinMode(ms, INPUT_PULLUP);

  
  analogReadResolution(12);

  Serial.begin(9600);
  while (!Serial) delay(10); 

  if (servodr.attached()) {
    Serial.println("Servodr is attached!");
  } else {
    Serial.println("Servodr is NOT attached!");
  }

  if (servost.attached()) {
    Serial.println("Servost is attached!");
  } else {
    Serial.println("Servost is NOT attached!");
  }

  T0 = 25.0 + 273.15; 
}

void loop() {

  // ── Servo control
  int vrx   = analogRead(jvrx);
  int valdr = map(vrx, 0, ADC_MAX, 0, 180); 
  int valst = abs(valdr - 180);              

  if (digitalRead(ms) == LOW) {
    servodr.write(90);
    servost.write(90);
  } else {
    servodr.write(valdr);
    servost.write(valst);
  }

  // ── Thermistor reading 
  int raw = analogRead(th);

  if (raw <= 0 || raw >= 4095) {
    Serial.println("Bad thermistor reading — check wiring!");
    delay(500);
    return;
  }

  VRT = (VCC / ADC_MAX) * raw;  
  VR  = VCC - VRT;
  RT  = (VR / VRT) * R;         

  float ln_val = log(RT / RT0);
  TX = (1.0 / ((ln_val / B) + (1.0 / T0))); 
  TX = TX - 273.15;                           

  // ── Serial output ──────────────────────────────────────────────────────────
  Serial.print("Raw ADC: ");
  Serial.print(raw);
  Serial.print("\t");
  Serial.print("Temperature: ");
  Serial.print(TX);
  Serial.print("C\t");
  Serial.print("Servo DR: ");
  Serial.print(valdr);
  Serial.print("°\t");
  Serial.print("Servo ST: ");
  Serial.print(valst);
  Serial.println("°");

  delay(100);
}