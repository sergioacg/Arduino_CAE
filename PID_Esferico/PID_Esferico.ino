//***************************************************//
//***************************************************//
//*****   PID Tanque Esferico                   *****//
//*****                                         *****//
//***** by: Sergio Andres Castaño Giraldo       *****//
//***** https://controlautomaticoeducacion.com/ *****//
//*****                                         *****//
//***************************************************//
//***************************************************//

#include <TimerOne.h> 

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

// Sensor MPX5010DP (Presión Diferencial)
byte MPX1 = A0;

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
float tankLevel = 0;

String inputString = "";         // Una cadena para mantener los datos recibidos
boolean stringComplete = false;  // Ya se recibió toda la cadena?

float r1=0.0;  //Referencia del Heater 1
volatile float u=0.0,u_1=0.0;    //Acción de Control
byte Ts = 1; //Periodo de muestreo
//Parámetros del PID
float kp,ti,td;
float q0,q1,q2;  
volatile float e=0.0,e_1=0.0,e_2=0.0;
      
float k=0.14,tau=35,theta=4+Ts/2;   //Parámetros del Modelo del sistema

void setup() {
  Serial.begin(9600); //Comunicación Serial
  inputString.reserve(200); // Reserva espacio para la cadena
  pinMode(flowSensor1Pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(flowSensor1Pin), flowSensor1Interrupt, RISING);
  
    //Valor máximo del Timer es 8.3 Segundos
  Timer1.initialize(1000000);      //Configura el TIMER en 8 Segundos
  Timer1.attachInterrupt(SampleTime) ; //Configura la interrupción del Timer 1


  //*************************************************************************//
   //*****************   SINTONIA POR ZIEGLER y NICHOLS    *******************//
   //*************************************************************************//

   kp=(0.9*tau)/(k*theta);
   ti=3.33*theta;
   td=0.0*theta;
  

   //*************************************************************************//
   //*****************           PID DIGITAL               *******************//
   //*************************************************************************//

  // Calculo do controle PID digital
   q0=kp*(1+Ts/(2.0*ti)+td/Ts);
   q1=-kp*(1-Ts/(2.0*ti)+(2.0*td)/Ts);
   q2=(kp*td)/Ts;

}

void loop() {
  
  
  /*
  // Ativar bomba3 (BombaL) 
  digitalWrite(bomba3.enPin1, HIGH);
  digitalWrite(bomba3.enPin2, LOW);
  analogWrite(bomba3.pwmPin, velocity);
  */  

  //Leitura da altura via pressão
  tankLevel = readPressureLevel(MPX1, 5.0, 0.03);
  if(tankLevel < 0){tankLevel = 0;}
  //Serial.println("Setpoint (cm),Nivel (cm),Bomba (%)");
  Serial.print(r1);
  Serial.print(",");
  Serial.print(tankLevel);
  Serial.print(",");
  Serial.print(0);
  Serial.println();

  //Calcular flujo cada 1segundo
  if ((millis() - oldTime) > 1000) { 
    detachInterrupt(digitalPinToInterrupt(flowSensor1Pin));

    flowRate1 = (pulseCount1 * 60.0) / 7.5; // L/min (YF-S201: 1 pulso = 2.25 mL)

    // Reiniciar conteo de pulsos y tiempo
    pulseCount1 = 0;
    oldTime = millis();

    attachInterrupt(digitalPinToInterrupt(flowSensor1Pin), flowSensor1Interrupt, RISING);
  }
  delay(1000);
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
      r1 = inputValue;
    }
  }
}



//Función de la interrupción
void flowSensor1Interrupt() {
  pulseCount1++;
}

//Función del Periodo de Muestreo (Timer 1)
void SampleTime(void)
{
  PID();
}


//Lectura sensor de presion
double readPressureLevel(byte mpxPin, float Vs, float tolP) {
  // mpcPin = Pino do sensor
  // Vs = 5.0 Voltagem de alimentação (pode botar o valor exato para maior presição)
  // tolP = Tolerancia do sensor, usado para ajustar a medida
  
  double Vout, P, aux = 0;
  const int numSamples = 10;
  double rho = 997;
  double g=9.8;

  // Ler voltagem do sensor (filtro de meia móvel)
  for (int i = 0; i < numSamples; i++) {
    aux += (double(analogRead(mpxPin)) * 5.0 / 1023.0);
    delay(5);
  }
  Vout = aux / numSamples;

  // Calcular a pressão em kPa segundo equação do DataSheet (Gráfico)
  P = (Vout - 0.04 * Vs) / (0.09 * Vs) + tolP;

  // Calcular o nível do tanque em cm
  double Level = ((P * 1000) / (rho * g)) * 100;

  return Level;
}


/*===========================================================================*/
/*=======================    FUNCION DEL CONTROL PID  =======================*/
/*===========================================================================*/
void PID(void)
{
    e=(r1-tankLevel);
    // Controle PID
      u = u_1 + q0*e + q1*e_1 + q2*e_2; //Ley del controlador PID discreto
    
    if (u >= 100.0)        //Saturo la accion de control 'uT' en un tope maximo y minimo
     u = 100.0;
    
    if (u <= 0.0 || r1==0 || tankLevel >=17)
     u = 0.0;

     //u = r1;
     //Retorno a los valores reales
     e_2=e_1;
     e_1=e;
     u_1=u;
     
     //La accion calculada la transformo en PWM
     //Transforma el valor de porcentaje a velocidad
    if(r1 == 0){
      velocity = 0;
    }
    else {
      velocity = map(u, 0,100,150,255);
    }
    

    // Ativar bomba1 (BombaBTS)
    digitalWrite(bomba1.pwmA, LOW);
    analogWrite(bomba1.pwmB, velocity);
     
}