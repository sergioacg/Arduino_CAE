

//Display 7 Segmentos Cathode Comúm
int display7c[10]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x67};


//Declara os PINO do Arduino
const byte push = 2;
const byte motor = 13;
//Delcaração dos Pinos do Display
byte a=3;
byte b=4;
byte c=5;
byte d=6;
byte e=7;
byte f=8;
byte g=9;
// Pinos dos Transistors
byte t1=10;
byte t2=11;
byte t3=12;
byte t4=13;

const int TMP36 = A3; // Define o pino que lera a saída do LM35

volatile int START = 0; //Variável da interrupção deve ser volatile
float temperatura; //Temperatura
int tempF;         //Temperatura Fahrenheit
int mens=1;        //Bandeira para Mostrar Menssagem
const int UP = 86; //Limite máximo de temperatura [F]
const int DOWN = 77; //Limite mínimo de temperatura [F]

//Função Display 7 Segmentos
// desde el pin ini hasta el pin fin
void display7seg(int bits,int a,int b,int c,int d,int e,int f,int g){

  //Mostra o numero bits no display 7 segmentos
  digitalWrite(a,bitRead(bits,0));
  digitalWrite(b,bitRead(bits,1));
  digitalWrite(c,bitRead(bits,2));
  digitalWrite(d,bitRead(bits,3));
  digitalWrite(e,bitRead(bits,4));
  digitalWrite(f,bitRead(bits,5));
  digitalWrite(g,bitRead(bits,6));
}

//Função da multiplexação
void mostrar( ) 
{
   int dig[3];   //Vetor dos digitos
   //Dígito Centena
   dig[0]=tempF/100;
   //Dígito Dezena
   dig[1]=(tempF-dig[0]*100)/10;
   //Dígito Unidade
   dig[2]=(tempF-dig[0]*100-dig[1]*10);
   

   //Rutina de Multiplexación
   for(int i=t1;i<=t3;i++){ 
     //Chama a Função Display 7 Segmentos
     display7seg(display7c[dig[i-t1]],a,b,c,d,e,f,g); 
     digitalWrite(i,HIGH);  //Liga um display
     delay(1);               //espera um tempinho
     digitalWrite(i,LOW);   //desliga o display
   }                      
}

//Mostra 3 linhas quando o sistema está parado
void parado (void)
{
  display7seg(0,a,b,c,d,e,f,g);
  //Rutina de Multiplexación
   for(int i=t1;i<=t3;i++){ 
     digitalWrite(g,HIGH);  //Liga o segmento g
     digitalWrite(i,HIGH);  //Liga um display
     delay(1);               //espera um tempinho
     digitalWrite(i,LOW);   //desliga o display
   }
  digitalWrite(t4,LOW);
}

void setup()
{
  Serial.begin(9600);            //Comunicação Serial a 9600 Baudios
  pinMode(push, INPUT_PULLUP);   //PINO 2 (push) como entrada com resistência PullUp
  
  //Configuração da Interrupção do Arduino, define o PINO 2 (push) como evento de interrupção
   //define a função (InterContador) a ser executado na interrupção
   //e configura que se ative quando o PINO 2 pasa de alto para baixo (FALLING)
   attachInterrupt(digitalPinToInterrupt(push), Inicio, FALLING);
  //Configura os 8 Pinos digitais como SAIDAS
  for(int i=a;i<=t4;i++){
    pinMode(i,OUTPUT);
  }
}

void loop()
{
  if(START == 0) //Indica que a maquina está parada
  {
    parado();  //Mostra nos Display que o sistema está parado
    if(mens != START && START == 0) //Mostra a menssagem 1 vez só
    {
      Serial.println("Sistema Desligado"); 
        mens=0;  
    }
  }
  else
  {
    if(mens != START && START == 1) //Mostra a menssagem 1 vez só
    {
      Serial.println("Sistema Ligado"); 
        mens=1;  
    }
    
    //Letura do Sensor em Celcius
    temperatura = (float(analogRead(TMP36))*5/1023-0.5)/0.01; //TMP36
    //Conversão para Fahrenheit
    tempF= (temperatura * 9/5) + 32;
    mostrar();
    
    if(tempF > 100)
      digitalWrite(motor,HIGH); //Liga Motor
    if(tempF < 77)
      digitalWrite(motor,LOW);  //Desliga Motor
  }
}

void Inicio (void)
{
  START = !START;
}
