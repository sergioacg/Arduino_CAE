/******************************************************
 * ****************************************************
 * ****     BALANZA ELECTRÓNICA CON HX711         *****
 * ****                                           *****
 * ****  By: SERGIO ANDRES CASTAÑO GIRALDO        *****
 * ****  https://controlautomaticoeducacion.com/  *****
 * ****                                           *****
 * ****************************************************
 */

//Incluye las librerías
#include "HX711.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

//Define las variables globales
byte DT=3;
byte CLK=2;
byte modo = 7;
byte tara = 6;
int peso_conocido[4] = {500,1000,3000,5000};
long escala;

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,16,2);  

//Crear el objeto balanza
HX711 balanza;

//Función de Anti-debounce (Evitar el rebote del pulsador)
void anti_debounce(byte boton){
      delay(100);
      while(digitalRead(boton)); //Anti-Rebote
      delay(100);
}

//Función de calibración y ajuste
void calibration(){
  
  int i = 0,cal=1;
  long adc_lecture;
  
  // Escribimos el Mensaje en el LCD
  lcd.setCursor(2, 0);
  lcd.print("Calibracion de");
  lcd.setCursor(4, 1);
  lcd.print("Balanza");
  delay(1500);
  balanza.read();
  balanza.set_scale(); //La escala por defecto es 1
  balanza.tare(20);  //El peso actual es considerado Tara.
  
  lcd.clear();

  //Inicia el proceso de ajuste y calibración
  while(cal == 1){
    
    lcd.setCursor(1, 0);
    lcd.print("Peso Conocido:");
    lcd.setCursor(1, 1);
    lcd.print(peso_conocido[i]);
    lcd.print(" g        ");

    //Busca el peso conocido con el botón tara
    if(digitalRead(tara)){  
      anti_debounce(tara);
      i =(i>2) ? 0:i+1; //if-else en una linea
    }

    //Selecciona el peso conocido con el boton modo
    if(digitalRead(modo)){
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Ponga el Peso");
      lcd.setCursor(1, 1);
      lcd.print("y espere ...");
      delay(2000);

      //Lee el valor del HX711
      adc_lecture = balanza.get_value(100);

      //Calcula la escala con el valor leido dividido el peso conocido
      escala = adc_lecture / peso_conocido[i];

      //Guarda la escala en la EEPROM
      EEPROM.put( 0, escala );
      delay(100);
      cal = 0; //Cambia la bandera para salir del while
      lcd.clear();
    }
    
  }
}

void setup() {
  //Configura la balanza
  balanza.begin(DT, CLK);
  
  //Configura los botones
  pinMode(modo, INPUT);
  pinMode(tara, INPUT);

  // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();

  //Lee el valor de la escala en la EEPROM
  EEPROM.get( 0, escala );

  //Pregunta si se desea entrar en calibración
  if(digitalRead(modo) && digitalRead(tara))
    calibration();

  //Mensaje inicial en el LCD
  lcd.setCursor(1, 0);
  lcd.print("Retire el Peso");
  lcd.setCursor(1, 1);
  lcd.print("y Espere");
  delay(2000);
  balanza.set_scale(escala); // Establecemos la escala
  balanza.tare(20);  //El peso actual es considerado Tara.

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Listo....");  
  delay(1000);
  lcd.clear();
}

void loop() {
  float peso;

  //Mide el peso de la balanza
  peso = balanza.get_units(10);

  //Muestra el resultado
  lcd.setCursor(1, 0);
  lcd.print("Peso: ");
  lcd.print(peso,0);
  lcd.println(" g        ");
  delay(5);

  //Botón de Tara
    if(digitalRead(tara)){  
      anti_debounce(tara);
      balanza.tare(10);  //El peso actual es considerado Tara.
    }
}
