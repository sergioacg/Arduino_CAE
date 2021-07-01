#define DOUT 2
#define PD_SCK 3
#define factor 2880068.0
#define rho 997.0 //Kg/m3
#define Gr 9.8 //m/s2

unsigned long value;
long weight;
  
long get_weight()
{
   digitalWrite(PD_SCK, LOW);
   delayMicroseconds(1);
   // wait for the chip to become ready:
   while (digitalRead(DOUT) == HIGH);
   value = 0;
  for (int i = 23; i > -1; i--){ //bitWrite23 =bit24 
   digitalWrite(PD_SCK, HIGH);
   delayMicroseconds(1);
   digitalWrite(PD_SCK, LOW);
   if (digitalRead(DOUT) == HIGH){bitSet(value, i);}
   }  
    // para que siga leyendo muestras a 10Hz: 
    digitalWrite(PD_SCK, HIGH);
    delayMicroseconds(1);
    digitalWrite(PD_SCK, LOW);
    delayMicroseconds(1);
 
 return value; // todos 1 = 1677215
}

void setup() {
   Serial.begin(9600);
   pinMode(DOUT, INPUT);
   pinMode(PD_SCK, OUTPUT);
}

void loop() {
  weight = get_weight();
  float Pr;
  float h;
  h = (weight-1000000.0)/67693.0;
  //h= (Pr*1000.0)/(rho*Gr); //metros
 // Serial.print("Presion (kPa): ");
  //Serial.println(Pr);
  Serial.print("Nivel (cm): ");
  Serial.println(h);
  //Serial.println(weight);
  delay(3000);
  
}
