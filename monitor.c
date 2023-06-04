#include <ArduinoJson.h>
#include "EspMQTTClient.h"

//variáveis para Json
char json_btn[100];

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
// Valida o valor da temperatura atual
  if(temp > 23){
    if(buzz == false){
      buzz = true;
      digitalWrite(buzzer, HIGH); //aciona o buzzer
      int inc_buzz = 0;
    } 
    else if(inc_buzz >= 10){
      digitalWrite(buzzer, LOW);
      buzz = false;
    } else {
      inc_buzz++;
    }
    digitalWrite(LedRed, HIGH);  //aciona o led
    digitalWrite(ventoinha, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
    digitalWrite(LedRed, LOW);  //aciona o led
    digitalWrite(ventoinha, LOW);
  }

// Valida o valor da tensão atual
  if(tensao <= 0.4 && buzz == false) {
    if(buzz == false){
      buzz = true;
      digitalWrite(buzzer, HIGH); //aciona o buzzer
      int inc_buzz = 0;
    } 
    else if(inc_buzz >= 10){
      digitalWrite(buzzer, LOW);
      buzz = false;
    } else {
      inc_buzz++;
    }
    digitalWrite(LedYellow, HIGH);
  } else {
    digitalWrite(LedYellow, LOW);
    digitalWrite(buzzer, LOW);
  }
  delay(500);
 
}

void converte_json()
{
  StaticJsonDocument<300> sjson_btn;

  sjson_btn["variable"] = "temperatura";
  sjson_btn["value"] = temp;
  serializeJson(sjson_btn, json_btn);
}

void envia_msg()
{
  client.publish("node/btn", json_btn); // You can activate the retain flag by setting the third parameter to true
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
  Serial.print("var:");  
  String var = msg["variable"];
  Serial.println(var);
  if(var == "led")
  {
    Serial.print("value:");
    String val = msg["value"];
    int var = val.toInt();
    Serial.println(var);
    digitalWrite(LedRed, var);
  }
  else if(var == "temperatura") {
    Serial.print("value:");
    String val = msg["value"];
    int var = val.toInt();
    Serial.println(var);
    analogWrite(temperatura, var);
  }
 
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
   client.subscribe("node/led", [] (const String &payload)  {
   Serial.println(payload);
   processa_msg(payload);
  });
}
