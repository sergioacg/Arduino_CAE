/******************************************************
 * ****************************************************
 * ****  CONTROL LAMPARA CON RELÉ                 *****
 * ****  By: SERGIO ANDRES CASTAÑO GIRALDO        *****
 * ****  https://controlautomaticoeducacion.com/  *****
 * ****                                           *****
 * ****************************************************
 */

 //Definiciones
#define Rele 3
#define Push 2


void setup() {
  pinMode(Rele,OUTPUT);
  pinMode(Push,INPUT_PULLUP);
}

void loop()
{
   bool RUN=0;
   while(1)                // Haga por siempre
   {
      //Pregunta por el botón
      if( ! digitalRead(Push) )
      {
         delay(200);       //Anti-Debounce
         //Instrucción para contabilizar una pulsación del botón
         while( ! digitalRead(Push) )
         {
            delay(200);       //Anti-Debounce
         }
         RUN = !RUN;      //Cambia el estado lógico de RUN
      }
      
      //Activa o desactiva el relé
      if(RUN)
         digitalWrite(Rele,HIGH);    //Activa Rele
      else 
         digitalWrite(Rele,LOW);  //Desactiva rele
   }
}
