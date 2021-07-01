//Sensor de temperatura usando el LM35 o TMP36 junto con un LCD
//Pratica para aprender a utilizar las entradas analogicas del Arduino (ADC)

//Definiciones
const int LM35 = A0; // Define el pin analogico del LM35
float temperatura; // Variable que armazenará la temperatura medida
int value;         //Variable entera para almacenar la conversión ADC

 
 
void setup() {
  analogReference (EXTERNAL); //Referencia analógica PIN AREF (3,3v)
  //Configuramos el puerto serial
  Serial.begin(9600);
} 
 
//Función que será ejecutada continuamente
void loop() {
  //Lectura del Pin A0 (ADC)
  //value=analogRead(LM35); // Almacena el valor entero.
  //Conveierte el valor entero en Temperatura
  //temperatura=(value*5.0/1023.0) / 0.01;
  
  temperatura = (float(analogRead(LM35))*3.3/1023.0)/0.01;     //LM35
  //temperatura = (float(analogRead(LM35))*5.0/1023.0-0.5)/0.01; //TMP36
  
  Serial.println(temperatura);
  delay(1000);
}
