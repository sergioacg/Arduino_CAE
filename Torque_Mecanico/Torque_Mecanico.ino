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

//Variable global de pulsos compartida con la interrupción
volatile int pulsos = 0;
unsigned long timeold;
float resolution = 374.22;
//Variable Global Velocidad
int vel = 0;

void setup(){
  // set timer 1 divisor to  1024 for PWM frequency of 30.64 Hz
  TCCR1B = TCCR1B & B11111000 | B00000101;
  //Configura Motor
  pinMode(motor.enPin, OUTPUT);
  pinMode(motor.directionPin, OUTPUT);
  //Pulsadores
  pinMode(BUTTON_MOD, INPUT_PULLUP);
  //Configurar Interrupción
  timeold = 0;
  attachInterrupt(digitalPinToInterrupt(ENCODER_A),leerEncoder,RISING);

}

void loop(){
  float rpm;
  //Cambia Velociadad 
  if(debounce(BUTTON_MOD)){
    vel = vel + 5;
    if(vel>255)
      vel = 0;
  }

  //Activa el motor dirección Forward con la velocidad
    setMotor(motor, vel, true);

    //Espera un segundo para el calculo de las RPM
    if (millis() - timeold >= 1000)
    {
      //Modifica las variables de la interrupción forma atómica
      ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        //rpm = float(pulsos * 60.0 / 374.22); //RPM
        rpm = float((60.0 * 1000.0 / resolution ) / (millis() - timeold) * pulsos);
        timeold = millis();
        pulsos = 0;
      }
      Serial.print("RPM: ");
      Serial.println(rpm);
      Serial.print("PWM: ");
      Serial.println(vel);
    }
    
}

//Función para dirección y velocidad del Motor
void setMotor(const Motor motor, int vel, bool dir){
  analogWrite(motor.enPin, vel);
  if(dir)
    digitalWrite(motor.directionPin, Forward);
  else
    digitalWrite(motor.directionPin, Backward);
}

//Función anti-rebote
bool debounce(byte input){
  bool state = false;
  if(! digitalRead(input)){
    delay(200);
    while(! digitalRead(input));
    delay(200);
    state = true;
  }      
  return state;   
}

//Función para la lectura del encoder
void leerEncoder(){
    pulsos++; //Incrementa una revolución
}
