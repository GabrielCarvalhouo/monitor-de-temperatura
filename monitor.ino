#include <ArduinoJson.h>
#include "EspMQTTClient.h"

//variáveis para Json
char json_temp[100];
char json_tensao[100];
char json_vent[100];

//pinos de entrada e saída
const int temperatura = 34;
const int Sensor_tensao = 35;
const int ventoinha = 33;
const int buzzer = 26;
const int LedRed = 32;
const int LedYellow = 25;

  //variáveis internas ---------------------------------
  //Definindo os valores iniciais

  float temp = 0;
  int ADCLido = 0;

  float tensao = 0;
  int tensao_Lida = 0;

  int inc_buzz = 0;
  bool buzz = false;
  int vent = 0;

  // ---------------------------------------------------

  // Realizando a conexão com o servidor
EspMQTTClient client
(
  "FIESC_IOT", //nome da rede Wi-Fi
  "C6qnM4ag81", //senha da rede Wi-Fi
  "mqtt.tago.io",  // MQTT Broker server ip padrão da tago
  "Token",   // username
  "4b72c5e7-a000-4cfa-828c-34b9efd20f14",   // Código do Token
  "DISP_SA",     // Nome do cliente que identifica exclusivamente seu dispositivo
  1883              // Porta MQTT, o padrão é 1883. esta linha pode ser omitida
);

void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(LedRed, OUTPUT);
  pinMode(LedYellow, OUTPUT);
  pinMode(ventoinha, OUTPUT);
}

void leitura_sinais()
{
  ADCLido = analogRead(temperatura);
  Serial.print(ADCLido);
  digitalWrite(buzzer, LOW); //desaciona o buzzer
  delay(150);
  temp = (float)ADCLido * (3300.0/2048.0)/10.0;
  
  Serial.print("  Temperatura = "); //mostra valor na tela
  Serial.print(temp);
  Serial.println(" *C");

  tensao_Lida = analogRead(Sensor_tensao);
  Serial.print(tensao_Lida);
  tensao = (float)tensao_Lida * 5.0 / 1024.0; //
  Serial.print("  Tensão = "); //mostra valor na tela
  Serial.print(tensao);
  Serial.println(" V");
}

void verifica_sinais()
{
  if(temp > 25 && tensao >= 0.4) {
    digitalWrite(LedRed, HIGH);  //aciona o led
    digitalWrite(LedYellow, HIGH);
    digitalWrite(ventoinha, HIGH);
  }else if(temp > 20){
    digitalWrite(LedRed, HIGH);  //aciona o led
    digitalWrite(ventoinha, HIGH);
  } else if (tensao >= 4) {
    digitalWrite(LedYellow, HIGH);
    digitalWrite(ventoinha, HIGH);
  } else {
    digitalWrite(LedRed, LOW);  //desaciona o led
    digitalWrite(LedYellow, LOW);
    digitalWrite(ventoinha, LOW);
  }
  delay(500);
 
  if(buzz != false && (tensao >= 0.4 || temp > 20)){
    digitalWrite(buzzer, HIGH); //aciona o buzzer
    Serial.println("buzzer ligou");  
  } else {
    digitalWrite(buzzer, LOW); //desaciona o buzzer
    Serial.println("buzzer desligou");
  }
}

void converte_json()
{
  StaticJsonDocument<300> sjson_temp;
  StaticJsonDocument<300> sjson_tensao;
  StaticJsonDocument<300> sjson_vent;

  sjson_temp["variable"] = "temperatura";
  sjson_temp["value"] = temp;
  serializeJson(sjson_temp, json_temp);

  sjson_tensao["variable"] = "tensao";
  sjson_tensao["value"] = tensao;
  serializeJson(sjson_tensao, json_tensao);

  sjson_vent["variable"] = "vent";
  Serial.println(digitalRead(ventoinha));
  if(digitalRead(ventoinha) == HIGH){
    sjson_vent["value"] = "ON";
  } else {
    sjson_vent["value"] = "OFF";
  }
  serializeJson(sjson_vent, json_vent);
}

void envia_msg()
{
  client.publish("node/var", json_temp); // You can activate the retain flag by setting the third parameter to true
  client.publish("node/var", json_tensao); // You can activate the retain flag by setting the third parameter to true
  client.publish("node/var", json_vent); // You can activate the retain flag by setting the third parameter to true
}

void loop() {
  leitura_sinais();
  verifica_sinais();
  converte_json();
  envia_msg();

  delay(1000);

  client.loop();
}


void processa_msg(const String payload)
{
  StaticJsonDocument<300> msg;

  DeserializationError err = deserializeJson(msg, payload);
  if (err) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.f_str());
  } 
  String var = msg["variable"];
  if(var == "buzz")
  {
    String val = msg["value"];
    int var = val.toInt();
    if(var == 1) {
      buzz = true;
      Serial.println("BUZZER ON");  
    } else {
      buzz = false;
      digitalWrite(buzzer, LOW);
      Serial.println("BUZZER OFF");  
    }
  } 
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
   client.subscribe("node/envia", [] (const String &payload)  {
   // Serial.println(payload);
   processa_msg(payload);
  });
}
