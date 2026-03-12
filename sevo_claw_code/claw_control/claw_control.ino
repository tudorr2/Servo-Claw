#include <Servo.h>

Servo servodr;
Servo servost;

#define sdr 2
#define sst 4
#define jvrx A5
#define ms 3
#define th A2

#define RT0 10000   // Ω
#define B 3977      //  K

#define VCC 5.0    //Supply  voltage
#define R 10000  //R=10KΩ

float RT, VR, ln, TX,  T0, VRT;

void setup() {
  servodr.attach(sdr);
  servost.attach(sst);

  servodr.write(0);
  servost.write(0);


  pinMode(ms, INPUT_PULLUP);

  Serial.begin(9600);

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


  T0 = 25 + 273.15;


}

void loop() {
  
  int vrx = analogRead(jvrx);
  int valdr = map(vrx, 0, 1023, 0, 180);
  int valst = abs(valdr - 180);
  if(digitalRead(ms) == LOW){
    servodr.write(90);
    servost.write(90);


  }
  else {
    servodr.write(valdr);
    servost.write(valst);

  }
  
  int raw = analogRead(th);
  Serial.print("Raw ADC: ");
  Serial.println(analogRead(th));
  VRT = (5.00 / 1023.00) * raw;      //Conversion to voltage
  VR  = VCC - VRT;
  RT  = (VR / VRT) * R;               //Resistance of RT

  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor

  TX =  TX - 273.15;                 //Conversion to Celsius

  Serial.print("Temperature:");
  Serial.print("\	");
  Serial.print(TX);
  Serial.print("C\	\	");


}
