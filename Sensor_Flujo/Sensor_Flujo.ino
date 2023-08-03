//***************************************************//
//***************************************************//
//*****   Sensor de Flujo YF-S201               *****//
//*****                                         *****//
//***** by: Sergio Andres Castaño Giraldo       *****//
//***** https://controlautomaticoeducacion.com/ *****//
//*****                                         *****//
//***************************************************//
//***************************************************//

// BTS7960 - 1 (Bomba 1)
byte RPWM1 = 4;
byte LPWM1 = 6;

// l298 - 1 (Bomba 3)
byte IN3_L1 = 24;
byte IN4_L1 = 25;
byte BEN_L1 = 5;

// Sensor YF-S201 
byte flowSensor1Pin = 2;
volatile unsigned long pulseCount1 = 0;
float flowRate1 = 0.0;

unsigned long oldTime = 0;

//Estructura da Bomba L298
typedef struct{
  byte enPin1;
  byte enPin2;
  byte pwmPin;
}BombaL;

//Estructura da Bomba BTS7960
typedef struct{
  byte pwmA;
  byte pwmB;
}BombaBTS;


//Crear las bombas con las estructuras
const BombaBTS bomba1 = {RPWM1, LPWM1};
const BombaL   bomba3 = {IN3_L1, IN4_L1, BEN_L1};


float inputValue  = 0;
int   velocity = 0;

String inputString = "";         // Una cadena para mantener los datos recibidos
boolean stringComplete = false;  // Ya se recibió toda la cadena?


void setup() {
  Serial.begin(9600); //Comunicación Serial
  inputString.reserve(200); // Reserva espacio para la cadena
  pinMode(flowSensor1Pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(flowSensor1Pin), flowSensor1Interrupt, RISING);
}

void loop() {
  //Transforma el valor de porcentaje a velocidad
  velocity = map(inputValue,0,100,0,255);

  // Ativar bomba1 (BombaBTS)
  digitalWrite(bomba1.pwmA, LOW);
  analogWrite(bomba1.pwmB, velocity);
  
  /*
  // Ativar bomba3 (BombaL) 
  digitalWrite(bomba3.enPin1, HIGH);
  digitalWrite(bomba3.enPin2, LOW);
  analogWrite(bomba3.pwmPin, velocity);
  */  

  //Calcular flujo cada 1segundo
  if ((millis() - oldTime) > 1000) { 
    detachInterrupt(digitalPinToInterrupt(flowSensor1Pin));

    flowRate1 = (pulseCount1 * 60.0) / 7.5; // L/min (YF-S201: 1 pulso = 2.25 mL)

    // Reiniciar conteo de pulsos y tiempo
    pulseCount1 = 0;
    oldTime = millis();

    attachInterrupt(digitalPinToInterrupt(flowSensor1Pin), flowSensor1Interrupt, RISING);

    // Mostrar la tasa de vazão
    Serial.print("Flujo en el tanque 1: ");
    Serial.print(flowRate1);
    Serial.println(" L/min \n\n");
  }
}

// Esta función se llama cada vez que el puerto serie recibe un byte:
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read(); // Obtiene el nuevo byte
    inputString += inChar; // Añade el byte a la cadena
    if (inChar == '\n') { // Si es un byte de nueva línea, la entrada está completa
      inputValue = inputString.toInt(); // Convierte la cadena a int
      // Si inputValue está en el rango de 0 a 100
      if(inputValue > 100){inputValue = 100;}
      if(inputValue < 0){inputValue = 0;}
      inputString = ""; // Limpia la cadena
    }
  }
}



//Función de la interrupción
void flowSensor1Interrupt() {
  pulseCount1++;
}