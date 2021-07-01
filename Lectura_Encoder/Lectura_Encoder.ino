//***************************************************//
//***************************************************//
//*****   Posición y Velocidad de Motor DC      *****//
//*****                                         *****//
//***** by: Sergio Andres Castaño Giraldo       *****//
//***** https://controlautomaticoeducacion.com/ *****//
//*****                                         *****//
//***************************************************//
//***************************************************//

#define ENCODER_A       2 // Amarillo
#define ENCODER_B       3 // Verde
#define BUTTON_FORWARD  4
#define BUTTON_BACKWARD 5

// Pines de Control Shield
const int E1Pin = 10;
const int M1Pin = 12;
const int E2Pin = 11;
const int M2Pin = 13;

typedef struct{
  byte enPin;
  byte directionPin;
}Motor;

//Creo el motor
const Motor motor = {E1Pin, M1Pin};

const int Forward = LOW;
const int Backward = HIGH;

void setup(){
  Serial.begin(9600);
  //Encoders como entradas
  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);
  //Pulsadores
  pinMode(BUTTON_FORWARD, INPUT_PULLUP);
  pinMode(BUTTON_BACKWARD, INPUT_PULLUP);
  //Configura Motor
  pinMode(motor.enPin, OUTPUT);
  pinMode(motor.directionPin, OUTPUT);
}

void loop(){
  // Pulsador hacia adelante
  if(! digitalRead(BUTTON_FORWARD)){
    digitalWrite(motor.directionPin, Forward);
    digitalWrite(motor.enPin, HIGH);
    imprimir_cuadratura();
  }
  else if(! digitalRead(BUTTON_BACKWARD)){
    digitalWrite(motor.directionPin, Backward);
    digitalWrite(motor.enPin, HIGH);
    imprimir_cuadratura();
  }
  else{
    digitalWrite(motor.enPin, LOW);
  }
}

void imprimir_cuadratura(){
  int a = digitalRead(ENCODER_A);
  int b = digitalRead(ENCODER_B);
  Serial.print(a*5);
  Serial.print(" ");
  Serial.println(b*5);
}
