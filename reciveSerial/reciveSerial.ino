//Creamos un contador
int contador=0;
 
void setup(){
  //Configuramos el puerto serial
  Serial.begin(9600);
}
 
void loop(){
  //Imprimimos el valor del contador en el monitor serial
  Serial.print("Contador: ");
  Serial.println(contador);  
  //incrementamos el contador y esperamos medio segundo
  contador++;
  delay(500);
}
