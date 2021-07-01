// this library includes the ATOMIC_BLOCK macro.
#include <util/atomic.h>

#define ENCODER_A       2 // Amarillo
#define ENCODER_B       3 // Verde
#define BUTTON_MOD      4


// Pin del Potenciometro
const int pot = A0;

// Pines de Control Shield
const int E1Pin = 10;
const int M1Pin = 12;
const int E2Pin = 11;
const int M2Pin = 13;


//Estructura del Motor
typedef struct{
  byte enPin;
  byte directionPin;
}Motor;

//Creo el motor
const Motor motor = {E1Pin, M1Pin};

//Constantes de dirección del Motor
const int Forward = LOW;
const int Backward = HIGH;

void setup(){
  // set timer 1 divisor to  1024 for PWM frequency of 30.64 Hz
  TCCR1B = TCCR1B & B11111000 | B00000101;
  //Configura Motor
  pinMode(motor.enPin, OUTPUT);
  pinMode(motor.directionPin, OUTPUT);
}

void loop(){
  //Activa el motor dirección Forward con la velocidad
    setMotor(motor, 255, true);
    delay(2000);
    setMotor(motor, 0, true);
    delay(2000);
}

//Función para dirección y velocidad del Motor
void setMotor(const Motor motor, int vel, bool dir){
  analogWrite(motor.enPin, vel);
  if(dir)
    digitalWrite(motor.directionPin, Forward);
  else
    digitalWrite(motor.directionPin, Backward);
}
