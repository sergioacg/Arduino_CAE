#include <Servo.h>  //Include the Servo Library

int dir=0,menu=1,opc=0;
char c='\0';
String words;       
Servo servo1;   // "Servos" objects are created

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Servo 360 - Arduino");
  //servo1.attach(5); //Default
  servo1.attach(5);
  servo1.write(90);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(menu==1){
   Serial.println(" ");
   Serial.println("------------  DIGUITE UNA OPCIÓN ENTRE 1 Y 3   -------------");
   Serial.println("1)  Detener Motor");
   Serial.println("2)  Sentido Horario");
   Serial.println("3)  Sentido Anti-Horario");
   menu=0;
  }
 

  while (Serial.available()) {    // Read the value sent by the Serial Port
    delay(5);
    c  = Serial.read();     // Read the characters
    words += c;              // Convert Characters to character string    
  }  
  opc=words.toInt();
  if (opc!=0){     
    if(opc>=1 && opc<=3){
      switch(opc){
        case 1:
                  servo1.write(90); 
                  Serial.println(">>>>> Motor DETENIDO! <<<<<<");
                  break;
        case 2:
                  servo1.write(0); 
                  Serial.println(">>>>> GIRANDO SENTIDO CW <<<<<<");
                  break;
        case 3:
                  servo1.write(180); 
                  Serial.println(">>>>> GIRANDO SENTIDO CCW <<<<<<");
                  break;
      }
    }else{
      Serial.println("   OPCIÓN INVALIDA!!!! INTENTE NUEVAMENTE");
    }
    menu=1;
    opc=0;
  }  

  words = "";   // Initialize the string of characters received 

}
