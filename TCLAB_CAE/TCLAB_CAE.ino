/******************************************************
 * ****************************************************
 * ****  LABORATORIO DE TEMPERATURA               *****
 * ****  By: SERGIO ANDRES CASTAÑO GIRALDO        *****
 * ****  https://controlautomaticoeducacion.com/  *****
 * ****                                           *****
 * ****************************************************
 */

// Definiciones componentes de la tarjeta
#define senT1 A0  //TMP36
#define senT2 A2  //TMP36
#define senT3 A1  //TMP36
#define senI1 A3  //Corriente H1
#define senI2 A4  //Corriente H2
#define actH1 3   //TIP31C
#define actH2 5   //TIP31C
#define hot     9   //Led

int cont=0;

float TempRead(int sen)
{
  float S1,aux;
  int i;
  //Filtro de promedio movil en la lectura ADC
  aux=0;
  for(i=0;i<10;i++){
    aux = aux + (float(analogRead(sen))*3.3/1023.0-0.5)/0.01; //TMP36
    //delay(5);
  }
  S1 = aux/10.0;
  return(S1);
}

float CurrentRead(int sen)
{
  float S1,aux;
  int i;
  //Filtro de promedio movil en la lectura ADC
  aux=0;
  for(i=0;i<10;i++){
    aux = aux + (float(analogRead(sen))*3.3/1023.0); //TMP36
    //delay(5);
  }
  S1 = aux/10.0;
  return(S1);
}

void setup() {
  pinMode(hot,OUTPUT);  //Led "Caliente" como salida
  digitalWrite(hot,LOW);
  analogReference (EXTERNAL); //Referencia analógica PIN AREF (3,3v)
  //Configuramos el puerto serial
  Serial.begin(9600);
  analogWrite(actH1,0);
  analogWrite(actH2,0);
}

void loop() {
  float T1,T2,T3,I1,I2;
  

  T1 = TempRead(senT1);
  T2 = TempRead(senT2);
  T3 = TempRead(senT3);

  I1 = CurrentRead(senI1);
  I2 = CurrentRead(senI2);

  Serial.print("Cont: ");
  Serial.println(cont);

  Serial.print("Temperatura 1: ");
  Serial.println(T1);

  Serial.print("Temperatura 2: ");
  Serial.println(T2);

  Serial.print("Temperatura 3: ");
  Serial.println(T3);

  Serial.print("Corriente 1: ");
  Serial.println(I1);

  Serial.print("Corriente 2: ");
  Serial.println(I2);

  if(cont==2){
    analogWrite(actH1,255);
    Serial.println("Heater 1: 100");
  }

  if(cont==6){
    analogWrite(actH1,0);
    Serial.println("Heater 1: 0");
  }

  if(cont==8){
    analogWrite(actH2,255);
    Serial.println("Heater 2: 100");
  }

  if(cont==12){
    analogWrite(actH2,0);
    Serial.println("Heater 2: 0");
  }

  delay(5000);
  digitalWrite(hot, !digitalRead(hot)); //Led Toggle
  cont++;

  if(cont==20){
    cont=0;
  }
  
}
