//*************************************************************//
//*************************************************************//
//****                                                    *****//
//**** CONTADOR CRECIENTE Y DECRECIENTE CON DISPLAY 7 SEG *****//
//****                                                    *****//
//**** By: SERGIO ANDRES CASTAÑO GIRALDO                  *****//
//**** https://controlautomaticoeducacion.com/            *****//
//**** RIO DE JANEIRO - BRASIL - 2018                     *****//
//****                                                    *****//
//*************************************************************//
//*************************************************************//


//Display 7 Segmentos Catodo Común
int display7c[10]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x67};

//Display 7 Segmentos Anodo Común
int display7a[10]= {0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x18};

//Delcaración de Los Pines del Display
byte a=2;
byte b=3;
byte c=4;
byte d=5;
byte e=6;
byte f=7;
byte g=8;
byte inc=9;    //Pulsador Incremento (Resistencia pulldown)
byte dec=10;   //Pulsador Incremento (Resistencia pullup)
int contador=0;

//Función que coloca en el puerto de salida los bits comenzando
// desde el pin ini hasta el pin fin
void puerto(int bits,int ini,int fin){
  for(int i=ini;i<=fin;i++)
  {
    digitalWrite(i,bitRead(bits,i-ini));
  }
}

void setup() {
  //Configura los 8 Pines digitales como SALIDAS
  for(int i=a;i<=g;i++){
    pinMode(i,OUTPUT);
  }
  pinMode(inc,INPUT);
  pinMode(dec,INPUT_PULLUP);
}

void loop() {
    //Incremento del Contador
    if(digitalRead(inc)){
      delay(100);
      while(digitalRead(inc)); //Anti-Rebote
      delay(100);
      contador++;
    }

    //Decremento del Contador
    if(!digitalRead(dec)){
      delay(100);
      while(!digitalRead(dec)); //Anti-Rebote
      delay(100);
      contador--;
    }

    //Si pasa de 9 en el incremento
    if(contador>9){
      contador=0;
    }

    //Si pasa de 0 en el decremento
    if(contador<0){
      contador=9;
    }
    //Muestra los numeros en el Display
    puerto(display7c[contador],a,g);
}
