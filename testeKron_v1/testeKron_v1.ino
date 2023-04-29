#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define   PIN_BT    D3

const char *ssid = "TP LINK";
const char *password = "morais1212";

const char *broker = "m16.cloudmqtt.com";//"mqtt.thingspeak.com";
const int portMqtt = 14391;
const char *userNameMqtt = "lbhkbooa";
const char *passwordMqtt = "8fgN0cELBlX8";
char* topic = "testeGrafana";
char* topic2 = "testeEdu";
bool libera = 0;
unsigned long lastTime = 0, cont = 0;

WiFiManager wm; //Gerenciador WiFi
WiFiClient clientWifi;
PubSubClient clientMqtt(broker, portMqtt, clientWifi);

void mqttConnect(void);
void callbackTeste(char *topic, byte *payload, unsigned long length){
  Serial.print("Mensagem Recebida: "); 
  String recebido = "";
  for(int i = 0; i < length; i++){
    recebido += char(payload[i]);
  }
  Serial.println(recebido);
}

void setup(){
  Serial.begin(9600);

  pinMode(PIN_BT, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, 1); //Led desligado
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  digitalWrite(LED_BUILTIN, 0); //Led ligado
  Serial.print("Wifi Conectado!");
  
  mqttConnect();
  randomSeed(analogRead(0));
}

void loop(){
  //if(!digitalRead(PIN_BT) && !libera){
  //  libera = 1;
  if(millis() - lastTime >= 7000){
    lastTime = millis();
    if(clientMqtt.connected()){
      //cont += 1;
      cont = random(10, 200);
      char *texto = "";
      sprintf(texto,  "%d", cont);
      if(clientMqtt.publish(topic, texto)){
        Serial.println("Publicado!");    
      }
      else{
        Serial.println("Erro ao publicar!"); 
      }
    }
    else{
      mqttConnect();      
    }
  }
  //if(libera) if(millis() - lastTime >= 1500) libera = 0;
}

void mqttConnect(){
  do{
    //clientMqtt.setServer(broker, portMqtt);
    String clientMqtt_id = "edu";
    if(clientMqtt.connect((char*) clientMqtt_id.c_str(), userNameMqtt, passwordMqtt)){
      Serial.println("Conectado ao broker MQTT."); 
    }
    else{
      Serial.println("Falha ao conectar no broker MQTT.");
    }
    delay(500);
  }while(!clientMqtt.connected());
  clientMqtt.setCallback(callbackTeste);
  clientMqtt.subscribe(topic2);
}


