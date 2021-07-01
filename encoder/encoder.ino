/*
#define ENCODER_A 2 // Amarillo
#define ENCODER_B 3 // Verde
#define MOTOR 12    // Blanco
#define ENABLE 10
#define BUTON_FOREWARD 4
#define BUTON_BACKWARD 5
*/

#define ENCODER_A       2 // Amarillo
#define ENCODER_B       3 // Verde
#define BUTTON_FORWARD  4
#define BUTTON_BACKWARD 5

// Pines de Control Sheld
const int E1Pin = 10;
const int M1Pin = 12;
const int E2Pin = 11;
const int M2Pin = 13;

// Estructura del Motor
typedef struct {
  byte enPin;
  byte directionPin;
} Motor;

const int M1 = 0;

//Creo el Motor
const Motor motor = { E1Pin, M1Pin } ;

const int Forward = LOW;
const int Backward = HIGH;

void setup() {
  Serial.begin(9600);
  //Configura los Encoders como Entradas
  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);
  //Configura los pulsadores como entradas con PullUp
  pinMode(BUTTON_FORWARD, INPUT_PULLUP);
  pinMode(BUTTON_BACKWARD, INPUT_PULLUP);
  
  //Configura el motor
  pinMode(motor.enPin, OUTPUT);        //Pin del PWM
  pinMode(motor.directionPin, OUTPUT); //Pin de Sentido de giro
}

void loop() {

  //Si el boton de adelante se presiona...
  if(!digitalRead(BUTTON_FORWARD)){
    digitalWrite(motor.directionPin, Forward);
    digitalWrite(motor.enPin, HIGH);
    cuadratura();
  }
  //si el boton de atras se presiona
  else if(!digitalRead(BUTTON_BACKWARD)){
    digitalWrite(motor.directionPin, Backward);
    digitalWrite(motor.enPin, HIGH);
    cuadratura();
  }
  //Caso contrario pare el motor
  else{
    digitalWrite(motor.enPin, LOW);
  }
}

//Imprime señal de cuadratura en el serial plotter
void cuadratura(){
  int a = digitalRead(ENCODER_A);
  int b = digitalRead(ENCODER_B);
  Serial.print(a*2.5); 
  Serial.print(" ");
  Serial.print(b*2.5+2.52);
  Serial.println();
}
