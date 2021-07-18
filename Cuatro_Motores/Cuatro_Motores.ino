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

#define ENCODER_A       18 // Amarillo
#define ENCODER_B       19 // Verde
#define ENCODER_C       20 // Amarillo
#define ENCODER_D       21 // Verde


// Pines de Controle
const int EAPin = 2;
const int EBPin = 3;
const int ECPin = 4;
const int EDPin = 5;

const int I1APin = 23;
const int I2APin = 22;
const int I1BPin = 24;
const int I2BPin = 25;

const int I1CPin = 26;
const int I2CPin = 27;
const int I1DPin = 28;
const int I2DPin = 29;


//Variable global de pulsos compartida con la interrupción
volatile int pulsos1 = 0, pulsos2 = 0;
volatile int pulsos3 = 0, pulsos4 = 0;


unsigned long timeold, lastTime;
float resolution = 374.22;

//Variáveis controle PID
float rpm1=0,rpm2=0,rpm3=0,rpm4=0;

float w1=0,w2=0,w3=0,w4=0;  //Referencia

float e1[3]={0,0,0};           // Vetor do erro
float u1[2]={0,0};             //Vector da Lei de Controle
float e2[3]={0,0,0};           // Vetor do erro
float u2[2]={0,0};             //Vector da Lei de Controle
float e3[3]={0,0,0};           // Vetor do erro
float u3[2]={0,0};             //Vector da Lei de Controle
float e4[3]={0,0,0};           // Vetor do erro
float u4[2]={0,0};             //Vector da Lei de Controle

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
  byte directionPin1;
  byte directionPin2;
}Motor;

//Creo el motor
const Motor motorA = {EAPin, I1APin, I2APin};
const Motor motorB = {EBPin, I1BPin, I2BPin};
const Motor motorC = {ECPin, I1CPin, I2CPin};
const Motor motorD = {EDPin, I1DPin, I2DPin};

//Estructura Controle PID
typedef struct{
  //Constantes del PID Digital
     float q0,q1,q2;
     //Saturación
     float u_min,u_max;
}PIDz;

PIDz pidA;
PIDz pidB;
PIDz pidC;
PIDz pidD;

//Constantes de dirección del Motor
//Forward = true;
//Backward = false;
bool directionA = false;
bool directionB = false;
bool directionC = false;
bool directionD = false;


/*===========================================================================*/
/*=======================    FUNÇÃO DO CONTROLE PID  =======================*/
/*===========================================================================*/
//Bibliografía da Função PID:
//https://www.udemy.com/course/sistemas-de-control-arduino/?referralCode=E96DE5D712758EC78582
//
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
//Bibliografía da Função PID:
//https://www.udemy.com/course/sistemas-de-control-arduino/?referralCode=E96DE5D712758EC78582
//
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
   update_past(u3,kU);
   update_past(e3,kE);
   update_past(u4,kU);
   update_past(e4,kE);
   
  //Calcula el error actual
   e1[kE] = w1 - rpm1;
   e2[kE] = w2 - rpm2;
   e3[kE] = w3 - rpm3;
   e4[kE] = w4 - rpm4;
   
   // Calcula la Acción de Control PID
   u1[kU] = PID_action(pidA,u1,e1);
   u2[kU] = PID_action(pidB,u2,e2);
   u3[kU] = PID_action(pidC,u3,e3);
   u4[kU] = PID_action(pidD,u4,e4);

    //Activa el motor dirección Forward con la velocidad
    actionMotor(motorA, u1[kU], directionA, w1);
    actionMotor(motorB, u2[kU], directionB, w2);
    actionMotor(motorC, u3[kU], directionC, w3);
    actionMotor(motorD, u4[kU], directionD, w4);
    //Bibliografía da Função PID:
    //https://www.udemy.com/course/sistemas-de-control-arduino/?referralCode=E96DE5D712758EC78582
    //
}

void setup(){
  // set timer 1 divisor to  1024 for PWM frequency of 30.64 Hz
  TCCR1B = TCCR1B & B11111000 | B00000101;
  Serial.begin(9600);
  //Encoders como entradas
  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);
  pinMode(ENCODER_C, INPUT);
  pinMode(ENCODER_D, INPUT);
  
  //Configura Motor
  pinMode(motorA.enPin, OUTPUT);
  pinMode(motorA.directionPin1, OUTPUT);
  pinMode(motorA.directionPin2, OUTPUT);
  
  pinMode(motorB.enPin, OUTPUT);
  pinMode(motorB.directionPin1, OUTPUT);
  pinMode(motorB.directionPin2, OUTPUT);

  pinMode(motorC.enPin, OUTPUT);
  pinMode(motorC.directionPin1, OUTPUT);
  pinMode(motorC.directionPin2, OUTPUT);

  pinMode(motorD.enPin, OUTPUT);
  pinMode(motorD.directionPin1, OUTPUT);
  pinMode(motorD.directionPin2, OUTPUT);
  
  //Configurar Interrupción
  timeold = 0;
  lastTime = 0;
  attachInterrupt(digitalPinToInterrupt(ENCODER_A),Encoder1,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B),Encoder2,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_C),Encoder3,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_D),Encoder4,RISING);
  

  // ********************************* //
   // ******** SINTONIA DEL PID ZN***** //
   // ********************************* //
   
   kp=(1.2*tau)/(K*L);
   ti=2*L;
   td=0.5*L;
   //kp = 0.42504;
   //ti = 0.5582;
   //td = 0.0473;
   //PIDA
   pidA.q0=kp*(1+Ts/(2*ti)+td/Ts);
   pidA.q1=-kp*(1-Ts/(2*ti)+(2*td)/Ts);
   pidA.q2=(kp*td)/Ts;
   pidA.u_min = 0.0;
   pidA.u_max = 255.0;
   
   //PIDB   
   pidB.q0=kp*(1+Ts/(2*ti)+td/Ts);
   pidB.q1=-kp*(1-Ts/(2*ti)+(2*td)/Ts);
   pidB.q2=(kp*td)/Ts;
   pidB.u_min = 0.0;
   pidB.u_max = 255.0;

   //PIDC   
   pidC.q0=kp*(1+Ts/(2*ti)+td/Ts);
   pidC.q1=-kp*(1-Ts/(2*ti)+(2*td)/Ts);
   pidC.q2=(kp*td)/Ts;
   pidC.u_min = 0.0;
   pidC.u_max = 255.0;

   //PIDD   
   pidD.q0=kp*(1+Ts/(2*ti)+td/Ts);
   pidD.q1=-kp*(1-Ts/(2*ti)+(2*td)/Ts);
   pidD.q2=(kp*td)/Ts;
   pidD.u_min = 0.0;
   pidD.u_max = 255.0;
}

void loop(){
  
  String dato,degC;
  int i,ini,fin,flag1=0,flag2=0,dir=0;

    //Calculo das RPM
    if (millis() - timeold >= Tm)
    {
      //Modifica las variables de la interrupción forma atómica
      ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        rpm1 = float((60.0 * 1000.0 / resolution ) / (millis() - timeold) * pulsos1);
        rpm2 = float((60.0 * 1000.0 / resolution ) / (millis() - timeold) * pulsos2);
        rpm3 = float((60.0 * 1000.0 / resolution ) / (millis() - timeold) * pulsos3);
        rpm4 = float((60.0 * 1000.0 / resolution ) / (millis() - timeold) * pulsos4);
        
        
        pulsos1 = 0;
        pulsos2 = 0;
        pulsos3 = 0;
        pulsos4 = 0;
      }

      //Executa as funções do controle PID
      SampleTime();
      timeold = millis();
      
    }

    //Dados pelo Monitor Serial
    if (millis() - lastTime >= 2000)
    {
      Serial.print("RPM1: ");
      Serial.println(rpm1);
      Serial.print("RPM2: ");
      Serial.println(rpm2);
      Serial.print("RPM3: ");
      Serial.println(rpm3);
      Serial.print("RPM4: ");
      Serial.println(rpm4);
      Serial.println();
      lastTime = millis();
    }


    //********* Receber dados pelo Serial Monitor  ***************
    //
    // O inicio da comunicação começa com a letra maiscula do motor
    // ou também com letra minuscula do motor
    // 
    // Se começa com letra maiscula, é para mudar o SETPOINT do PID
    // Se começa com letra minuscula, é para mudar o sentido de giro do motor
    //
    // o fim da mensagem da comunicação é com carater $
    //
    // Se recebe: A70$ --> Muda o setpoint do Motor A em 70
    // Se recebe: B100$ --> Muda o setpoint do Motor B em 100
    if (Serial.available()){
      //leemos el dato enviado
      dato=Serial.readString();
      //Procura as letras maiusculas e minusculas que vem do monitor serial
      for(i=0;i<10;i++){
        if(dato[i]=='A' || dato[i]=='B'|| dato[i]=='C'|| dato[i]=='D' ||
           dato[i]=='a' || dato[i]=='b'|| dato[i]=='c'|| dato[i]=='d'){
          ini=i+1;
          i=10;
          flag1 = 1;
        }
       }
       for(i=ini;i<10;i++){
        if(dato[i]=='$'){
          fin=i;
          i=10;
          flag2 = 1;
        }
       }

      //Se foi encontrado o carater de finalização '$' 
      // e o carater de inicio pode continuar
      if(flag1 == 1 && flag2 == 1){
       // salvo en degC el caracter con el escalon
        degC=dato.substring(ini, fin);

        //Letra MAISCULA muda o SETPOINT
        if(dato[ini-1]=='A')
          w1 = degC.toDouble();   // Convert character string to integers
        if(dato[ini-1]=='B')
          w2 = degC.toDouble();
        if(dato[ini-1]=='C')
          w3 = degC.toDouble();
        if(dato[ini-1]=='D')
          w4 = degC.toDouble();
          
      //********************************************//    
      //********************************************//
      //** Letra MINUSCULA muda o SENTIDO DE GIRO **//
      //********************************************//
      //********************************************//

      // *********  MOTOR A  **********************//
        // a1$  ----> Sentido Forward
        // a0$  ----> Sentido Backward
        if(dato[ini-1]=='a'){
          dir = degC.toInt();
          if(dir == 0)
            directionA = false;
          else
            directionA = true;
        }
        
        // *********  MOTOR B  **********************//
        // b1$  ----> Sentido Forward
        // b0$  ----> Sentido Backward
        if(dato[ini-1]=='b'){
          dir = degC.toInt();
          if(dir == 0)
            directionB = false;
          else
            directionB = true;
        }

        // *********  MOTOR C  **********************//
        // c1$  ----> Sentido Forward
        // c0$  ----> Sentido Backward
        if(dato[ini-1]=='c'){
          dir = degC.toInt();
          if(dir == 0)
            directionC = false;
          else
            directionC = true;
        }

        // *********  MOTOR D  **********************//
        // d1$  ----> Sentido Forward
        // d0$  ----> Sentido Backward
        if(dato[ini-1]=='d'){
          dir = degC.toInt();
          if(dir == 0)
            directionD = false;
          else
            directionD = true;
        }

        flag1 = 0;
        flag2 = 0;
      }
    }
  
}

//Función para dirección y velocidad del Motor
void setMotor(const Motor motor, int vel, bool dir){
  analogWrite(motor.enPin, vel);
  if(dir){
    digitalWrite(motor.directionPin1, HIGH);
    digitalWrite(motor.directionPin2, LOW);
  }
  else{
    digitalWrite(motor.directionPin1, LOW);
    digitalWrite(motor.directionPin2, HIGH);
  }
}

//Evita o barulho do motor quando está parado. 
//Barulho causado pelo PWM
void actionMotor(const Motor motor, int vel, bool dir,float w){
  if(w > 0)
    setMotor(motor, vel, dir);
  else
    setMotor(motor, 0, dir);
}


//Función para la lectura del encoder
void Encoder1(){
  //Lectura de Velocidad
  pulsos1++; //Incrementa una revolución
}

void Encoder2(){
  //Lectura de Velocidad
  pulsos2++; //Incrementa una revolución
}

void Encoder3(){
  //Lectura de Velocidad
  pulsos3++; //Incrementa una revolución
}

void Encoder4(){
  //Lectura de Velocidad
  pulsos4++; //Incrementa una revolución
}
