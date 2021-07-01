#include <Servo.h>

int angle = 12;
int zero = 5;

int td=5;

//Variables para contar el tiempo con millis() 
long Time = 0;

//Variables LDR
int LDR1=870;
int LDR2=896;


// Nombre Servomotor
Servo servo;

void setup()
{
                               
  // A0 como entrada
  pinMode(A0, INPUT);  
  
  // Inicia el monitor serial
  Serial.begin(9600);

  // servo en el pin 2 digital
  servo.attach(2);

  // Coloca el servo en la posicion inicial
  servo.write(zero);  

}

void loop()
{
 
  // Imprime en el monitor serial el valor leido por el LDR
  Serial.print("LDR 1: ");
  Serial.println(analogRead(A0));
  Serial.print("LDR 2: ");
  Serial.println(analogRead(A1));
  
  // Si esta de dia
  //if (analogRead(A1) > LDR2){
    while (analogRead(A0) < LDR1)//   || analogRead(A1) < 892  ) 
    {
      Time = millis(); 
      // Mueva el servomotor.
      servo.write(angle);
      while (servo.read() < angle  ){}
      delay(10);
      }
  /*else{
     // si esta de noche
      while (analogRead(A0) > 670)//   || analogRead(A1) < 892  ) 
      {
      Time = millis(); 
      // Mova o servomotor 20 graus.
      servo.write(angle);
      while (servo.read() < angle  ){}
      delay(12);
      }
  }*/
    
    while (servo.read() >= angle-2 ){
      Time=millis() - Time;
      Serial.print("Time ");
      Serial.println(Time );
      
      if(Time<1)
          Time=3;
          
      // Espere um tempo
       delay(Time*td);
      servo.write(zero);     
    }      
    

}
