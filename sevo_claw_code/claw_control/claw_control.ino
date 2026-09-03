#include <Servo.h>

Servo servodr;

// pins
#define sdr 22
#define jvrx A1
#define ms 5
#define th A0

// thermistor
#define RT0 10000
#define B 3977

// constants
#define VCC 3.3
#define ADC_MAX 4095.0
#define R 10000

float RT, VR, TX, T0, VRT;

 
#define SERVO_MIN 10
#define SERVO_MAX 170
#define SERVO_DEADBAND 3

int currentServoPos = 90;
int targetServoPos = 90;

void setup() {

  servodr.attach(sdr);
  servodr.write(currentServoPos);

  pinMode(ms, INPUT_PULLUP);

  analogReadResolution(12);

  Serial.begin(9600);

  T0 = 25.0 + 273.15;
}

void loop() {



  int vrx = analogRead(jvrx);
  targetServoPos = map(vrx, 0, ADC_MAX, SERVO_MAX, SERVO_MIN);


  if (digitalRead(ms) == LOW) {
    targetServoPos = 170;
  }

  targetServoPos = constrain(targetServoPos, SERVO_MIN, SERVO_MAX);

 
  if (abs(targetServoPos - currentServoPos) >= SERVO_DEADBAND) {

    currentServoPos = targetServoPos;

    servodr.write(currentServoPos);

    Serial.print("Servo position: ");
    Serial.println(currentServoPos);
  }




  int raw = analogRead(th);

    VRT = (VCC / ADC_MAX) * raw;
    RT = R * VRT / (VCC - VRT);
    float ln_val = log(RT / RT0);
    TX = 1.0 / ((ln_val / B) + (1.0 / T0));
    TX = TX - 273.15;

    Serial.print("Raw ADC: ");
    Serial.print(raw);

    Serial.print("\tTemperature: ");
    Serial.print(TX);

    Serial.println(" C");
  

  delay(100);
}