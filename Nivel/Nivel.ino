
#include "HX711.h"
// HX711 circuit wiring
const int DOUT = 2;
const int CLK = 3;

HX711 tank1;


void setup() {
  Serial.begin(9600);
  tank1.begin(DOUT, CLK);
}

void loop() {

  if (tank1.is_ready()) {
    long nivel1 = tank1.read_average(4);
    long level;
    Serial.print("Leyendo el HX710B: ");
    Serial.println(nivel1);
    level=0.0001594509*nivel1 - 150;
    Serial.print("Nivel: ");  
    Serial.println(float(level)/10.0);
    //Serial.println(0.0000155046967070413*nivel1 - 14.6);
    //Serial.println(-0.00000000000025353787*nivel1^2 + 0.0000163605368203193*nivel1 - 17.8164873876384);
  } else {
    Serial.println("HX710B no fue encontrado");
  }

  delay(2000);
}
