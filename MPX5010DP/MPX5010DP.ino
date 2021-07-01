/*=================================================
https://controlautomaticoeducacion.com/arduino/ - 2020
By: Sergio Andres Castaño Giraldo
=================================================*/
// Vout Vacio = 0.15v
// Vout en 0cm = 0.176v
double Level,Vout=0.15,P,Vs=5.0;
double aux;
double tolP=0.04*Vs-Vout; // Ajusta la medida de presión

int adc_filtrado = 0;
int adc_raw = 0;
#define alpha 0.6

int i, rho = 997;
double g=9.8;

void setup()
{
  Serial.begin(9600);
  adc_raw = analogRead(A0);
  adc_filtrado = adc_raw;
}

void loop() 
{
   adc_raw = analogRead(A0);
   adc_filtrado = (alpha*adc_raw) + ((1-alpha)*adc_filtrado);
   Vout = (float(adc_filtrado)*5.0/1023.0); //v
   
   
   /*/Voltaje del Sensor MPC5010DP
   aux=0;
   for(i=0;i<10;i++){
    aux = aux + (float(analogRead(A0))*5.0/1023.0); //v
    delay(5);
   }
   
   Vout=aux/10.0;*/
  
  //Presión en Kpa según gráfica 4 del Datasheet
  P = ( Vout - 0.04*Vs + tolP ) / (0.09 * Vs) ; //kPa
   
  Level = ((P*1000)/(rho*g))*100;  //Medida de Nivel del tanque

  //Level = 1.081349*Level + 0.219574;

  Serial.print("\n\nVoltaje:");
  Serial.print(Vout);
  Serial.println(" v");

  Serial.print("Presión:");
  Serial.print(P);
  Serial.println(" kPa");

  Serial.print("Nivel:");
  Serial.print(Level,1);
  Serial.println(" cm");
  
  delay(3000);
}
