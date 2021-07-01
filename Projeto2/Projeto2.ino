#include <Keypad.h>
#include <LiquidCrystal.h>   

const byte rows = 4; 
const byte col = 4;
int PW,CurrentPW=1234;
float FR,bat,TMP;
char Key;

byte pRows[]  = {7,6,5,4};
byte pCol[] = {3,2,1,0};
char teclas[4][4] = {{'1','2','3','A'},
                     {'4','5','6','B'},
                     {'7','8','9','C'},
                     {'*','0','#','D'}};

Keypad teclado = Keypad( makeKeymap(teclas), pRows, pCol, rows, col); 
LiquidCrystal lcd(13, 12, 11, 10, 9, 8); //(RS, E, D4,D5, D6, D7)

void setup() {
  int bandera=1;
  lcd.begin(16, 2);   // Inicia o LCD 16x02 (colunas,linhas)
  teclado.setHoldTime(1000); //Tempo de espera para o teclado
  lcd.setCursor(4,0);          // Coloca el cursor en las coordenadas (0,0)
  lcd.print("MEDIDOR");   // Escreve no LCD  
  while(bandera==1){
    PW=password();
    if(PW != CurrentPW){
      lcd.setCursor(0,1);
      lcd.print("Password ERRADO!!"); // Escribe no LCD  
      delay(2000); 
      lcd.clear();
      lcd.setCursor(4,0);          // Coloca el cursor en las coordenadas (0,0)
      lcd.print("MEDIDOR");   // Escreve no LCD
    }
    else
      bandera=0;
  }
  
}

int password(void){
  int pass,i=0;
  String KeyWord;
  //Pergunta por os 4 digitos da senha
  lcd.setCursor(0, 1);
  lcd.print("Pass:");
  while(i<4){
    Key=teclado.waitForKey(); //Espera até presionar botão
    if(Key>='0' && Key<='9'){
      lcd.print('*');
      KeyWord += Key;   //Armazena os caracteres
      i++;
    }
    if(Key=='D'){ 
      i=4;
    }
  }
  if(KeyWord.length()>0){
    pass=KeyWord.toInt(); //Converte para Inteiro
  }else{
    pass=0;
  }
  
  return(pass);
}
void loop() {
  char KeyOp;
  int i,j;
  //Fotoresistencia
  FR=(float(analogRead(A0))*5/1023); //TMP36 
  
  //bateria
  bat=(float(analogRead(A1))*9/921); 
  
  //temperatura
  TMP=(float(analogRead(A2))*5/1023-0.5)/0.01; //TMP36 
  

  lcd.home();         // Coloca el cursor en las coordenadas (0,0)
  lcd.print("B=");   // Escreve no LCD 
  lcd.print(bat);
  lcd.print("v T=");
  lcd.print(TMP);
  lcd.setCursor(0,1);         // Coloca el cursor en las coordenadas (0,0) 
  lcd.print("Dia= ");   // Escreve no LCD
  
  if(FR>2.5)
    lcd.print("Escuro");
  else
    lcd.print("Claro  ");


}
