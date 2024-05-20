#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

#define DEBUG

#define L1 23   //pino de saida para acionamento da Lampada rele1


//informações da rede WIFI
const char* ssid = "";                 //SSID da rede WIFI
const char* password =  "";    //senha da rede wifi

const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;
const char* mqttTopicSub ="esp/casa";

 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
 
  Serial.begin(115200);
  pinMode(L1, OUTPUT);
  digitalWrite(L1, HIGH);
  
  
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    #ifdef DEBUG
    Serial.println("Conectando ao WiFi..");
    #endif
  }
  #ifdef DEBUG
  Serial.println("Conectado na rede WiFi");
  #endif
 
  client.setServer(mqttServer);
  client.setCallback(callback);
 
  while (!client.connected()) {
    #ifdef DEBUG
    Serial.println("Conectando ao Broker MQTT...");
    #endif
 
    if (client.connect("ESP32Client", mqttServer)) {
      #ifdef DEBUG
      Serial.println("Conectado");  
      #endif
 
    } else {
      #ifdef DEBUG 
      Serial.print("falha estado  ");
      Serial.print(client.state());
      #endif
      delay(2000);
 
    }
  }

  //subscreve no tópico
  client.subscribe(mqttTopicSub);
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {

  //armazena msg recebida em uma sring
  payload[length] = '\0';
  String strMSG = String((char*)payload);

  #ifdef DEBUG
  Serial.print("Mensagem chegou do tópico: ");
  Serial.println(topic);
  Serial.print("Mensagem:");
  Serial.print(strMSG);
  Serial.println();
  Serial.println("-----------------------");
  #endif

  //aciona saída conforme msg recebida 
  if (strMSG == "acender"){         //se msg "rele1_on"
     digitalWrite(L1, LOW);  //coloca saída em LOW para ligar
  }else if (strMSG == "apagar"){   //se msg "rele1_off"
     digitalWrite(L1, HIGH);   //coloca saída em HIGH para desligar
  }
}

//função pra reconectar ao servidor MQTT
void reconect() {
  //Enquanto estiver desconectado
  while (!client.connected()) {
    #ifdef DEBUG
    Serial.print("Tentando conectar ao servidor MQTT");
    #endif
     
    bool conectado = strlen(mqqtServer) > 0 ?
                     client.connect("ESP32Client", mqqtServer) :
                     client.connect("ESP32Client");

    if(conectado) {
      #ifdef DEBUG
      Serial.println("Conectado!");
      #endif
      //subscreve no tópico
      client.subscribe(mqttTopicSub, 0); 
    } else {
      #ifdef DEBUG
      Serial.println("Falha durante a conexão.Code: ");
      Serial.println( String(client.state()).c_str());
      Serial.println("Tentando novamente em 10 s");
      #endif
      //Aguarda 10 segundos 
      delay(10000);
    }
  }
}
 
void loop() {
  if (!client.connected()) {
    reconect();
  }
  client.loop();
}
