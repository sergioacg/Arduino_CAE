//Declara los nombres de los pines del arduino
const byte push = 2;  //Pulsador
byte led = 12;        //Led
volatile int ISR_Cont = 0; //Variable volatil para la interrupción
int Cont = 0;

//Variables para contar el tiempo con millis() dentro de la interrupción
const int Banda = 150;
long Time = 0;
 
void setup()
{
  //Configura las entradas y salidas
   pinMode(led, OUTPUT);   
   pinMode(push, INPUT_PULLUP);
   //Inicializa la comunicación serial a 9600 baudios
   Serial.begin(9600);
   //Configura la interrupción en el PIN del pulsador en modo FALLING
   attachInterrupt(digitalPinToInterrupt(push), InterContador, FALLING);
}
 
void loop()
{
   //Enciende y apaga un LED con mucho tiempo de retardo
   //simplemente para consumir tiempo dentro del Arduino
   digitalWrite(led, HIGH);
   delay(3000);
   digitalWrite(led, LOW);
   delay(3000);
 
   //Esta etapa  incrementa el contador cuando ISR_Cont
   //es modificado dentro de la interrupción
   if (Cont != ISR_Cont)
   {
      Cont = ISR_Cont;
      Serial.print("Contador: ");
      Serial.println(Cont);
   }
}

//Función que se ejecuta cuando se ACTIVA la INTERRUPCIÓN
void InterContador()
{
  //Incrementa el contador
  //ISR_Cont++;

  // Debounce o Antirebote por código
  if (millis() - Time > Banda)
   {
      ISR_Cont++;
      Time = millis();
   }
}
