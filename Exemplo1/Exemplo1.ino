byte LED=5; //Nomear o pino 5 como LED
void setup() {
  // put your setup code here, to run once:
  pinMode(5,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED,HIGH); //Liga o Led
  delay(2000); //Atraso de 2 segundos
  digitalWrite(LED,LOW); //desliga o Led
  delay(2000); //Atraso de 1 segundos
}
