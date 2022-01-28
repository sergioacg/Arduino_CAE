// Declaración de los pines del Arduino
#define lamp 8
#define yellow 7
#define blue 6
#define red 5


//Initial state
bool state_yellow = LOW;
bool state_blue = LOW;
bool state_red = LOW;

void setup() {
  Serial.begin(9600); //Velocidad de Monitor Serial
  Serial1.begin(38400); //Velocidad Hc-05

  //Configura los pines de salida
  for(int i=5;i<=8;i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

}

void loop() {
  char crt; //Variable caracter que recibe los datos via Bluetooth

  //Preguntar si hay dato disponible en el puerto serial
  if(Serial1.available()){
    crt = Serial1.read();
    Serial.write(crt);
    
    // Preguntar si se activó el led Amarillo
    if(crt == 'u'){
      state_yellow = !state_yellow; // Cambia el estado de la variable bool
      digitalWrite(yellow, state_yellow);
    }

    // Preguntar si se activó el led Azul
    if(crt == 'h'){
      state_blue = !state_blue; // Cambia el estado de la variable bool
      digitalWrite(blue, state_blue);
    }

    // Preguntar si se activó el led Rojo
    if(crt == 'k'){
      state_red = !state_red; // Cambia el estado de la variable bool
      digitalWrite(red, state_red);
    }

    //Encender la Lampara
    if(crt == '1')
      digitalWrite(lamp, HIGH);

    if(crt == '0')
      digitalWrite(lamp, LOW);
    
  }

  //Mostrar en el monitor Serial el chat o el terminal de la app
  if (Serial.available()){
    crt = Serial.read();
    Serial.write(crt);
    Serial1.write(crt);
  }
  
}
