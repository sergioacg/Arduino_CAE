//Ejemplo 1 - Utilización del Display LCD

#include <LiquidCrystal.h>         
 
LiquidCrystal lcd(10, 9, 8, 5, 4, 3, 2); //(RS, RW, E, D4,D5, D6, D7)
 
void setup()
{
   lcd.begin(16, 2);            // Inicia el LCD 16x02 (columnas, filas)   
   lcd.setCursor(0, 0);         // Coloca el cursor en las coordenadas (0,0)   
   lcd.print("Ctrl Atumatico"); // Escribe no LCD   
   lcd.setCursor(3, 1);         // Coloca el cursor en las coordenadas (3,1) 
   lcd.print("Educacion");      // Escreve no LCD 
}
 
void loop()
{
   //Apagar el cursor
   lcd.noCursor();
   delay(500);
   
   //Encender el cursor
   lcd.cursor();
   delay(500);
}
