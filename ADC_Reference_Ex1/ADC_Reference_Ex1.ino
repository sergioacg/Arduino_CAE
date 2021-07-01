//Voltaje de Referencia usando el PIN AREF de Arduino

//Definiciones
const int POT = A3; // Define el pin Analógico (Potenciometro)
int value;
float volt;

//Bibliotecas
#include <LiquidCrystal.h> 
LiquidCrystal lcd(10, 8, 5, 4, 3, 2); //(RS, E, D4,D5, D6, D7)
 
 
//Función que es ejecutada una vez cuando enciende o resetea el Arduino
void setup() {
 lcd.begin(16, 2);   // Inicia el LCD 16x02 (colunas,filas)
 //analogReference (DEFAULT);    //Referencia analógica default (5v ou 3.3v)
 analogReference (EXTERNAL); //Referencia analógica PIN AREF
} 
 
//Función que será ejecutada continuamente
void loop() {
  
  //Lectura del Pin A3 (ADC)
  value=analogRead(POT); // Almacena el valor entero.
  //Convierte el valor enteiro en Tensión
  volt=(value*3.3/1023.0);
  //volt=(value*5/1023.0);

  //Visualiza en el LCD
  lcd.home();               // Coloca el cursor en las coordenadas (0,0)
  lcd.print("POT Int: ");   // Escribe en el LCD 
  lcd.print(value);         // Escribe en el LCD
  lcd.setCursor(0,1);       // Coloca el cursor en las coordenadas (0,0)
  lcd.print("POT (V): ");   // Escribe en el LCD  
  lcd.print(volt);          // Escribe en el LCD
  delay(1000);
  lcd.clear();              //Limpia todo el LCD
}
