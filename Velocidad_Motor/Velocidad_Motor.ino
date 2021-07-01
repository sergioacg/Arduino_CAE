//***************************************************//
//***************************************************//
//*****   Recepción y transmición de datos      *****//
//*****   de velocidad de un encoder de un      *****//
//*****   motor DC hacia Simulink para          *****//
//*****   Validación de modelo matematico       *****//
//*****                                         *****//
//***** by: Sergio Andres Castaño Giraldo       *****//
//***** https://controlautomaticoeducacion.com/ *****//
//*****                                         *****//
//***************************************************//
//***************************************************//

// this library includes the ATOMIC_BLOCK macro.
#include <util/atomic.h>

#define ENCODER_A       2 // Amarillo
#define ENCODER_B       3 // Verde
#define BUTTON_MOD      4

// Pines de Control Shield
const int E1Pin = 10;
const int M1Pin = 12;

//Estructura del Motor
typedef struct{
  byte enPin;
  byte directionPin;
}Motor;

//Estructura Union
typedef union{
  float number;
  uint8_t bytes[4];
}valor;

//Variable Union
valor velocidad;

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
float value;
float rpm=0;

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
  //Pregunta si tenemos un dato a recibir
  if(Serial.available() > 0){
      value = recepcion();
  }
  
  //Transforma el valor del voltaje (0-5) a velocidad
  vel = map(value,0,5,0,255);

  //Activa el motor dirección Backward con la velocidad
  setMotor(motor, vel, true);

  //Espera un segundo para el calculo de las RPM
  if (millis() - timeold >= 50)
  {
      //Modifica las variables de la interrupción forma atómica
      ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        rpm = float((60.0 * 1000.0 / resolution ) / (millis() - timeold) * pulsos);
        timeold = millis();
        pulsos = 0;
         velocidad.number = rpm * 0.10472; //rad/s
         Serial.write('V');
         for(int i=0; i<4; i++){
              Serial.write(velocidad.bytes[i]);
         }
         Serial.write('\n');
      }
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

//Recibir Flotante
float recepcion(){
  int i;
  valor buf;
  for(i=0; i<4; i++)
    buf.bytes[i] = Serial.read();  

  return buf.number;
}

//Función para la lectura del encoder
void leerEncoder(){
    pulsos++; //Incrementa una revolución
}
