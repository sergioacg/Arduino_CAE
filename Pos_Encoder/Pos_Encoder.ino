
#include <util/atomic.h> // this library includes the ATOMIC_BLOCK macro.

#define ENCODER_A       2 // Amarillo
#define ENCODER_B       3 // Verde
#define BUTTON_VEL      4
#define BUTTON_DIR      5

// Pines de Control Shield
const int E1Pin = 10;
const int M1Pin = 12;
const int E2Pin = 11;
const int M2Pin = 13;

//Variable global de posición compartida con la interrupción
volatile int theta = 0;
//Variable global de velocidad compartida con la interrupción
volatile int rev = 0;
//Variable Global Velocidad
int vel = 100;
//Variable Global Direccion
bool dir = true;

typedef struct{
  byte enPin;
  byte directionPin;
}Motor;

//Creo el motor
const Motor motor = {E1Pin, M1Pin};

const int Forward = LOW;
const int Backward = HIGH;
float posicion = 0;

void setup(){
  Serial.begin(9600);
  //Encoders como entradas
  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);
  //Pulsadores
  pinMode(BUTTON_VEL, INPUT_PULLUP);
  pinMode(BUTTON_DIR, INPUT_PULLUP);
  //Configura Motor
  pinMode(motor.enPin, OUTPUT);
  pinMode(motor.directionPin, OUTPUT);
  //Configurar Interrupción
  attachInterrupt(digitalPinToInterrupt(ENCODER_A),leerEncoder,RISING);
}

void loop(){
  
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    posicion = theta * 360.0 /374.2;
  }
  

  //Aumentamos o disminuimos velocidad
  if(posicion >= 180)
      setMotor(motor, 0, dir);
  else
      setMotor(motor, 100, dir);

  Serial.println(posicion);
}

void setMotor(const Motor motor, int vel, bool dir){
  analogWrite(motor.enPin, vel);
  if(dir)
    digitalWrite(motor.directionPin, Forward);
  else
    digitalWrite(motor.directionPin, Backward);
}

void leerEncoder(){
  rev++; //Incrementa una revolución
  int b = digitalRead(ENCODER_B);
  if(b > 0){
    //Incremento variable global
    theta++;
  }
  else{
    //Decremento variable global
    theta--;
  }
}
