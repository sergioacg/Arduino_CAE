//Sensor de temperatura usando el LM35 o TMP36 junto con un LCD
//Pratica para aprender a utilizar las entradas analogicas del Arduino (ADC)

//Definiciones
const int LM35 = A0; // Define el pin analogico del LM35
float temperatura; // Variable que armazenará la temperatura medida
int value;         //Variable entera para almacenar la conversión ADC

//Bibliotecas
#include <LiquidCrystal.h> 
LiquidCrystal lcd(10, 8, 5, 4, 3, 2); //(RS, E, D4,D5, D6, D7)
 
 
void setup() {
 lcd.begin(16, 2);   // Inicia el LCD 16x02 (columnas,filas)
 lcd.home();         // Coloca el cursor en las coordenadas (0,0)
 lcd.print("ADC Arduino");   // Escribe en el LCD 
 lcd.setCursor(0,1);         // Coloca el cursor en las coordenadas (0,1)
 lcd.print("Temp: ");   // Escribe en el LCD 
} 
 
//Función que será ejecutada continuamente
void loop() {
  //Lectura del Pin A0 (ADC)
  //value=analogRead(LM35); // Almacena el valor entero.
  //Conveierte el valor entero en Temperatura
  //temperatura=(value*5.0/1023.0) / 0.01;
  
  temperatura = (float(analogRead(LM35))*5.0/1023.0)/0.01;     //LM35
  //temperatura = (float(analogRead(LM35))*5.0/1023.0-0.5)/0.01; //TMP36
  
  lcd.setCursor(7,1);         // Coloca el cursor en las coordenadas (7,1)
  lcd.print(temperatura);   // Escribe en el LCD 
  lcd.setCursor(14,1);
  lcd.print("C    ");
  delay(1000);
}
