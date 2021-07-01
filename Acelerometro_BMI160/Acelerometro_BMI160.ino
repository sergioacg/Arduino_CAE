
#include <DFRobot_BMI160.h>
DFRobot_BMI160 bmi160;
const int8_t i2c_addr = 0x69;


void setup() {
  Serial.begin(115200);
  delay(2000);
  
  //init the hardware bmin160  
  if (bmi160.softReset() != BMI160_OK){
    Serial.println("reset false");
    while(1);
  }
  
  //set and init the bmi160 i2c address
  if (bmi160.I2cInit(i2c_addr) != BMI160_OK){
    Serial.println("init false");
    while(1);
  }

}

void loop() {
  int i, ini, fin;
  int rslt;
  int16_t accelGyro[6]={0};
  
  // Modo 0: Acelerometro y Giroscopio
  // Modo 1: Solo Giroscopio
  // Modo 2: Solo Acelerometro
  int modo = 2;

  rslt = bmi160.getAccelGyroData(accelGyro);
  
  switch(modo){
    case 0: ini = 0; fin = 6;break;
    case 1: ini = 0; fin = 3;break;
    case 2: ini = 3; fin = 6;break;
    default: Serial.println("Modo incorrecto");
             break;
  }

  //Imprime resultados
  if(rslt == 0){
    for(i=ini; i<fin; i++){
      if (i<3){
        //Los 3 primeros datos son del Giroscopio
        Serial.print(accelGyro[i]*3.14/180.0);
        Serial.print("\t");
      }else{
        //Los siguientes 3 datos son del acelerometro
        Serial.print(accelGyro[i]/16384.0);
        Serial.print("\t");
      }
    }
    Serial.println();
  }else{
    Serial.println("Error de lectura");
  }
  delay(100);

  
  
  

}
