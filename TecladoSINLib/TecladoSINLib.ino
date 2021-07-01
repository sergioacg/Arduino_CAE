byte pinesFilas[] = {9,8,7,6}; 
byte pinesColumnas[] = {5,4,3,2}; 
char teclas[4][4] = {{'1','2','3','A'}, 
                     {'4','5','6','B'}, 
                     {'7','8','9','C'}, 
                     {'*','0','#','D'}};
void setup()
{
  for (int nL = 0; nL <= 3; nL++) {
     pinMode(pinesFilas[nL], OUTPUT);
     digitalWrite(pinesColumnas[nL], HIGH);
  }

  for (int nC = 0; nC <= 3; nC++) {
     pinMode(pinesColumnas[nC], INPUT_PULLUP);
  } 
   
  Serial.begin(9600);
  Serial.println("Teclado 4x4");
  Serial.println();
}
 
void loop()
{
    //Barrido por las filas
    for (int nL = 0; nL <= 3; nL++)
    {
      digitalWrite(pinesFilas[nL], LOW);
      
      //Barrido en columnas buscando un LOW
      for (int nC = 0; nC <= 3; nC++) {
        if (digitalRead(pinesColumnas[nC]) == LOW)
        {
          Serial.print("Tecla: ");
          Serial.println(teclas[nL][nC]);
          while(digitalRead(pinesColumnas[nC]) == LOW){}
        }
      }

      digitalWrite(pinesFilas[nL], HIGH);
    }
   delay(10);
}
