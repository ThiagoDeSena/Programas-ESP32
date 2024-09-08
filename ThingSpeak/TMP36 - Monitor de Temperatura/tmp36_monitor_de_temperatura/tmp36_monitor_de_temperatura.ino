#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

const int TMP36 = 36; //Pino D36
int tempVal;    // temperature sensor raw readings
float temperatura=0.0,ultimaTemperatura=0.0;

int intervalSensor = 2000;
long prevMillisThingSpeak = 0;
int intervalThingSpeak = 15000; // Intervalo minímo para escrever no ThingSpeak write é de 15 segundos

#include <WiFi.h>
const char ssid[] = "VITOR-2G";
const char password[] = "251717rvt";
WiFiClient client;

#include <ThingSpeak.h>
const long CHANNEL = 2596090;
const char *WRITE_API = "ZCFA5K4I5U1XJ286";
const int relePin = 25; // pino ao qual o Módulo Relé está conectado

void setup()
{
  pinMode(relePin, OUTPUT); // seta o pino como saída
  digitalWrite(relePin, LOW); // seta o pino com nivel logico ALTO
  Serial.begin(115200);
  Serial.println();
  Serial.println("Envia os dados do sensor para o ThingSpeak usando o ESP32");
  Serial.println();

  WiFi.mode(WIFI_STA); //Modo Station
  ThingSpeak.begin(client);  // Inicializa o ThingSpeak
}

void loop(){

  // Conecta ou reconecta o WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Atenção para conectar o SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConectado");
  }

  int ValorLido = analogRead(TMP36);
  temperatura = (ValorLido / 1023.0);
  temperatura = (temperatura-0.5) * 100; // temperatura em Celsius

  if(temperatura>31){
    digitalWrite(relePin, HIGH);
  }else{
    digitalWrite(relePin, LOW);
  }

  Serial.println("Temperatura");
  Serial.println(temperatura);
  Serial.println("°C");
  delay(2000);

  

  if (millis() - prevMillisThingSpeak > intervalThingSpeak) {

    // Configura os campos com os valores
    ThingSpeak.setField(1,temperatura);
    


    // Escreve no canal do ThingSpeak 
    int x = ThingSpeak.writeFields(CHANNEL, WRITE_API);
    if (x == 200) {
      Serial.println("Update realizado com sucesso");
    }
    else {
      Serial.println("Problema no canal - erro HTTP " + String(x));
    }

    prevMillisThingSpeak = millis();
  }
}
