#include "BluetoothSerial.h"

String device_name = "ESP32_BLUETOOTH";

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

#define RELE 4
#define VERDE 15


char leitura;

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name); //Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  //Serial.printf("The device with name \"%s\" and MAC address %s is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str(), SerialBT.getMacString()); // Use this after the MAC method is implemented
  pinMode(2,OUTPUT);
  pinMode(RELE, OUTPUT);
  pinMode(VERDE, OUTPUT);
}

//Verifica se a conexão com o esp32 foi realizada
void conexao(){
  if(SerialBT.connected()){
    digitalWrite(13, HIGH);
    
  }else {
    digitalWrite(13, LOW);
    
  }
}

void loop() {
  conexao();

  leitura = (char)SerialBT.read();

  if(leitura == 'L' || leitura == 'l'){
    digitalWrite(VERDE, HIGH);
    SerialBT.print("Carga ligada");
    digitalWrite(RELE, HIGH);
  }else if(leitura == 'D' || leitura == 'd'){
    digitalWrite(VERDE, LOW);
    SerialBT.print("Carga desligada");
    digitalWrite(RELE, LOW);
  }


  

  delay(20);


}

