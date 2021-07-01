/******************************************************
 * ****************************************************
 * ****  CURVA DE REACCIÓN DE TEMPERATURA         *****
 * ****  By: SERGIO ANDRES CASTAÑO GIRALDO        *****
 * ****  https://controlautomaticoeducacion.com/  *****
 * ****                                           *****
 * ****************************************************
 */

// Definiciones componentes de la tarjeta
#define sensor1 A0  //TMP36
#define sensor2 A2  //TMP36
#define heater1 3   //TIP31C
#define heater2 5   //TIP31C
#define hot     9   //Led

//Variables Globales
float T1,aux;       //Temperatura del Heater 1

void setup() {
  pinMode(hot,OUTPUT);  //Led "Caliente" como salida
  digitalWrite(hot,LOW);
  analogReference (EXTERNAL); //Referencia analógica PIN AREF (3,3v) 
  //Configuramos el puerto serial
  Serial.begin(9600);
}

void loop() {

  int i;

  //Filtro de promedio movil en la lectura ADC
  aux=0;
  for(i=0;i<10;i++){
    aux = aux + (float(analogRead(sensor1))*3.3/1023.0-0.5)/0.01; //TMP36
    //delay(5);
  }
  T1 = aux/10.0;

 analogWrite(heater1,255*0.4);

 //Usar el Serial Plotter
    Serial.println("Temperatura_1");
    Serial.print(T1);

    delay(1000);
}
