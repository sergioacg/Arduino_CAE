#define LED 13 //Declara el PIN 13 como LED

//Llama las librerías
#include <LiquidCrystal.h>     
#include <TimerOne.h>    

//Variable para la interrupción
volatile long int Time=0;

//Configuración del LCD
LiquidCrystal lcd(9, 8, 5, 4, 3, 2); //(RS, E, D4,D5, D6, D7)
 
void setup()
{
   pinMode(LED,OUTPUT);          //LED como Saída
   lcd.begin(20, 4);            // Inicia el LCD 20x04 (columnas, filas)   
   lcd.setCursor(2, 0);         // Coloca el cursor en las coordenadas (2,0)   
   lcd.print("TIMER COM ARDUINO"); // Escribe no LCD   
   lcd.setCursor(0, 1);         // Coloca el cursor en las coordenadas (0,1) 
   lcd.print("Temporizador:");      // Escribe en el LCD
   Timer1.initialize(1000000);      //Configura el TIMER en 1 Segundo
   Timer1.attachInterrupt(Temporizador) ; //Configura la interrupción del Timer 1
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED,HIGH);
  delay(2000);
  digitalWrite(LED,LOW);
  delay(2000);
}

//Función de la Interrupción cuando se ejecuta el TIMER
void Temporizador(void)
{
  //Incrementa el timer
  Time++;
  //Resetea el contador cuando llega a 1000 segundos
  if(Time>1000){
    Time=0;
  }
  //Muestra en el LCD el valor actual del temporizador
  lcd.setCursor(14, 2);
  lcd.print("    ");
  lcd.setCursor(14, 2);
  lcd.print(Time);
}
