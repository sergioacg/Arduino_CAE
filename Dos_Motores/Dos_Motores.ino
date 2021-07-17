//***************************************************//
//***************************************************//
//*****    Velocidad de 2 Motores DC con PID    *****//
//*****                                         *****//
//***** by: Sergio Andres Castaño Giraldo       *****//
//***** https://controlautomaticoeducacion.com/ *****//
//*****                                         *****//
//***************************************************//
//***************************************************//

// this library includes the ATOMIC_BLOCK macro.
#include <util/atomic.h>

#define ENCODER_A1       2 // Amarillo
#define ENCODER_B1       4 // Verde
#define ENCODER_A2       3 // Amarillo
#define ENCODER_B2       5 // Verde


// Pines de Control Shield
const int E1Pin = 10;
const int M1Pin = 12;
const int E2Pin = 11;
const int M2Pin = 13;


//Variable global de pulsos compartida con la interrupción
volatile int pulsos1 = 0, pulsos2 = 0;
unsigned long timeold, lastTime;
float resolution = 374.22;
float resolution1 = 341.2;

//Variables controle PID
float rpm1,rpm2;
float w1=50,w2=30;                    //Referencia
float e1[3]={0,0,0};           // Vetor do erro
float u1[2]={0,0};             //Vector da Lei de Controle
float e2[3]={0,0,0};           // Vetor do erro
float u2[2]={0,0};             //Vector da Lei de Controle
int kU = sizeof(u1)/sizeof(float)-1;
int kE = sizeof(e1)/sizeof(float)-1;
float kp,ti,td;

//Modelo em função de transferência dos Motores
float K=4.2079,tau=0.4776,theta=0.1475;
//Periodo de amostragem
float Ts = 0.1;  
float L = theta + Ts/2; 
int Tm = 100; //Periodo de amostragem função millis dentro do Loop()

//Estructura del Motor
typedef struct{
  byte enPin;
  byte directionPin;
}Motor;
//Creo el motor
const Motor motor1 = {E1Pin, M1Pin};
const Motor motor2 = {E2Pin, M2Pin};

//Estructura Controle PID
typedef struct{
  //Constantes del PID Digital
     float q0,q1,q2;
     //Saturación
     float u_min,u_max;
}PIDz;

PIDz pid1;
PIDz pid2;

//Constantes de dirección del Motor
const int Forward = LOW;
const int Backward = HIGH;


/*===========================================================================*/
/*=======================    FUNÇÃO DO CONTROLE PID  =======================*/
/*===========================================================================*/
float PID_action(PIDz pid, float *u, float *e){
    float lu;
    // Controle PID
    // e[2] = e(k)
    // e[1] = e(k-1)
    // e[0] = e(k-2)
    // u[0] = u(k-1)
    lu = u[0] + pid.q0*e[2] + pid.q1*e[1] + pid.q2*e[0]; //Ley del controlador PID discreto
    
    // Anti - Windup
    if (lu >= pid.u_max)        
     lu = pid.u_max;
    
    if (lu <= pid.u_min)
     lu = pid.u_min;
     
     return(lu);     
}

//Função para o PID Discreto atualizar os valores do passado
void update_past(float v[],int kT){
   int i;
   for(i=1;i<=kT;i++){
      v[i-1]=v[i];
   }
}

//Função de interrupção pelo TIMER 1 para o periodo de amostragem do PID
void SampleTime(void)
{
  //Actualiza los vectores u y e
   update_past(u1,kU);
   update_past(e1,kE);
   update_past(u2,kU);
   update_past(e2,kE);
   
  //Calcula el error actual
   e1[kE] = w1 - rpm1;
   e2[kE] = w2 - rpm2;
   // Calcula la Acción de Control PID
   u1[kU] = PID_action(pid1,u1,e1);
   u2[kU] = PID_action(pid2,u2,e2);

    //Activa el motor dirección Forward con la velocidad
    setMotor(motor1, u1[kU], false);
    setMotor(motor2, u2[kU], false);
}

void setup(){
  // set timer 1 divisor to  1024 for PWM frequency of 30.64 Hz
  //TCCR1B = TCCR1B & B11111000 | B00000101;
  Serial.begin(9600);
  //Encoders como entradas
  pinMode(ENCODER_A1, INPUT);
  pinMode(ENCODER_B1, INPUT);
  pinMode(ENCODER_A2, INPUT);
  pinMode(ENCODER_B2, INPUT);
  
  //Configura Motor
  pinMode(motor1.enPin, OUTPUT);
  pinMode(motor1.directionPin, OUTPUT);
  pinMode(motor2.enPin, OUTPUT);
  pinMode(motor2.directionPin, OUTPUT);
  //Configurar Interrupción
  timeold = 0;
  lastTime = 0;
  attachInterrupt(digitalPinToInterrupt(ENCODER_A1),Encoder1,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A2),Encoder2,RISING);


  // ********************************* //
   // ******** SINTONIA DEL PID ZN***** //
   // ********************************* //
   
   kp=(1.2*tau)/(K*L);
   ti=2*L;
   td=0.5*L;
   //kp = 0.42504;
   //ti = 0.5582;
   //td = 0.0473;
   //PID1
   pid1.q0=kp*(1+Ts/(2*ti)+td/Ts);
   pid1.q1=-kp*(1-Ts/(2*ti)+(2*td)/Ts);
   pid1.q2=(kp*td)/Ts;
   pid1.u_min = 0.0;
   pid1.u_max = 255.0;
   
   //PID2
   kp=(1.2*tau)/(K*L);
   ti=2*L;
   td=0.5*L;
   
   pid2.q0=kp*(1+Ts/(2*ti)+td/Ts);
   pid2.q1=-kp*(1-Ts/(2*ti)+(2*td)/Ts);
   pid2.q2=(kp*td)/Ts;
   pid2.u_min = 0.0;
   pid2.u_max = 255.0;
}

void loop(){
  
  String dato,degC;
  int i,ini,fin,flag=0;

    //Calculo das RPM
    if (millis() - timeold >= 100)
    {
      //Modifica las variables de la interrupción forma atómica
      ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        rpm1 = float((60.0 * 1000.0 / resolution1 ) / (millis() - timeold) * pulsos1);
        rpm2 = float((60.0 * 1000.0 / resolution ) / (millis() - timeold) * pulsos2);
        timeold = millis();
        pulsos1 = 0;
        pulsos2 = 0;
      }
      
      Serial.print("RPM1: ");
      Serial.println(rpm1);
      Serial.print("RPM2: ");
      Serial.println(rpm2);
    }

    //Periodo de Amostragem
    if (millis() - lastTime >= Tm)
    {
       SampleTime();
       lastTime = millis();
    }


    //********* Receber dados pelo Serial Monitor  ***************
    // Se recebe: A70$ --> Muda o setpoint do Motor 1 em 70
    // Se recebe: S100$ --> Muda o setpoint do Motor 2 em 100
    if (Serial.available()){
      //leemos el dato enviado
      dato=Serial.readString();
      //Busco el valor del escalon en los datos recibidos
      for(i=0;i<10;i++){
        if(dato[i]=='A' || dato[i]=='S'){
          ini=i+1;
          i=10;
        }
       }
       for(i=ini;i<10;i++){
        if(dato[i]=='$'){
          fin=i;
          i=10;
          flag = 1;
        }
       }
      if(flag == 1){
       // salvo en degC el caracter con el escalon
        degC=dato.substring(ini, fin);
        if(dato[ini-1]=='A')
          w1 = degC.toDouble();   // Convert character string to integers
        if(dato[ini-1]=='S')
          w2 = degC.toDouble();

        flag = 0;
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


//Función para la lectura del encoder
void Encoder1(){
  //Lectura de Velocidad
  pulsos1++; //Incrementa una revolución
    
  //Lectura de Posición  
    int b = digitalRead(ENCODER_B2);
    if(b > 0){
      //Incremento variable global
      //theta++;
    }
    else{
      //Decremento variable global
      //theta--;
    }
}

void Encoder2(){
  //Lectura de Velocidad
  pulsos2++; //Incrementa una revolución
    
  //Lectura de Posición  
    int b = digitalRead(ENCODER_B2);
    if(b > 0){
      //Incremento variable global
      //theta++;
    }
    else{
      //Decremento variable global
      //theta--;
    }
}
