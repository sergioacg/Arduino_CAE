// Declaración de los pines del Arduino
#define yellow 7
#define blue 6
#define red 5

//Initial state
bool state_yellow = LOW;
bool state_blue = LOW;
bool state_red = LOW;

void setup() {
  Serial.begin(9600); //Velocidad de comunicación Serial

  //Configura los pines de salida
  for(int i=5;i<=7;i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

}

void loop() {
  char crt; //Variable caracter que recibe los datos via Bluetooth

  //Preguntar si hay dato disponible en el puerto serial
  if(Serial.available()){
    crt = Serial.read();

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
  }
}
