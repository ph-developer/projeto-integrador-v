#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

// Pinagem
#define pin_sensor_umidade A0
#define pin_valvula_irrigacao D1

// WiFi
const char* wifi_ssid = "*****"; // ID da rede WiFi.
const char* wifi_pass = "*****"; // Senha da rede WiFi.

// Broker MQTT
const char* mqtt_server = "97299651548a4c57a3aabb6700e6e882.s2.eu.hivemq.cloud"; // IP do Broker MQTT.
const int mqtt_port = 8883; // Porta do Broker MQTT.
const char* mqtt_usuario = "projeto-integrador-v";
const char* mqtt_senha = "univesp001004";
const char* mqtt_topico_irrigacao_manual = "pi_v/irrigacao_manual"; // Tópico de recebimento de irrigação manual.
const char* mqtt_topico_irrigacao_status = "pi_v/status_irrigacao"; // Tópico de envio do status da irrigação.
const char* mqtt_topico_umidade_atual = "pi_v/umidade_atual"; // Tópico de envio do percentual de umidade atual.

// Funcionalidades 
const int intervalo_sensor = 5000; // Intervalo entre as verificações de umidade. (em milisegundos)
const int tempo_irrigacao = 10000; // Tempo de irrigação. (em milisegundos)
const int umidade_minima = 70; // Percentual de umidade mínimo para acionar a irrigação. (em porcentagem)

// Variáveis
WiFiClientSecure wifiClient; // Cliente WiFi.
PubSubClient client(wifiClient); // Cliente MQTT.
#define MSG_BUFFER_SIZE (50) // Tamanho máximo do buffer do Cliente MQTT.
char msg[MSG_BUFFER_SIZE]; // Variável para armazenamento da mensagem recebida do Broker MQTT.

float umidade_atual; // Variável para armazenamento do percentual de umidade atual.
bool irrigacao_manual = false; // Variável para armazenamento do status de irrigação manual.
unsigned long tempo_sensor = millis(); // Variável para armazenamento da última verificação de umidade.

/**
 * Lida com as mensagens recebidas no tópico de Irrigação Manual do Broker MQTT.
*/
void callbackIrrigacaoManual(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, mqtt_topico_irrigacao_manual) == -1) return;

  char* message = (char*)"";
  for (unsigned int i = 0; i < length; i++) {
    message += (char) payload[i];
  }

  if (strcmp(message, "true") == -1) return;

  Serial.println("Comando de irrigação manual recebido");
  irrigacao_manual = true;
}

/**
 * Lida com as mensagens recebidas do Broker MQTT.
*/
void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  callbackIrrigacaoManual(topic, payload, length);
}

/**
 * Envia o status do processo de irrigação ao Broker MQTT.
*/
void publicarIrrigacao(bool status) {
  Serial.print("Status da irrigação: ");
  Serial.println(status == true ? "ligada" : "desligada");
  sprintf(msg, "%s", status == true ? "true" : "false");
  client.publish(mqtt_topico_irrigacao_status, msg, true);
}

/**
 * Inicia o processo de irrigação.
*/
void fazerIrrigacao() {
  digitalWrite(pin_valvula_irrigacao, LOW); // Invertido devido ao tipo de relé utilizado.
  publicarIrrigacao(true);

  delay(tempo_irrigacao);

  digitalWrite(pin_valvula_irrigacao, HIGH); // Invertido devido ao tipo de relé utilizado.
  publicarIrrigacao(false);

  irrigacao_manual = false;
}

/**
 * Captura a umidade atual do sensor e armazena o percentual em uma variável global.
*/
void capturarUmidade() {
  float umidade = analogRead(pin_sensor_umidade);
  umidade_atual = map(umidade, 0, 1023, 100, 0);
}

/**
 * Envia o percentual de umidade atual ao Broket MQTT.
*/
void publicarUmidade() {
  Serial.print("Percentual de umidade: ");
  Serial.print(umidade_atual);
  Serial.println(F("%"));
  sprintf(msg, "%f", umidade_atual);
  client.publish(mqtt_topico_umidade_atual, msg, true);
}

/**
 * Configura e realiza a conexão à rede WiFi.
*/
void setupWiFi() {
  delay(10);

  Serial.println();
  Serial.print("Conectando à rede ");
  Serial.println(wifi_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println();
  Serial.println("WiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

/**
 * Configura a conexão ao Broker MQTT.
*/
void setupMQTT() {
  wifiClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callbackMQTT);
}

/**
 * Realiza a conexão ao Broker MQTT.
*/
void conectarMQTT() {
  while (!client.connected()) {
    Serial.println("Conectando ao Broker MQTT...");
    
    String clientId = "NodeMCU_" + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqtt_usuario, mqtt_senha)) {
      Serial.println("Broker MQTT conectado");
      client.subscribe(mqtt_topico_irrigacao_manual);
    } else {
      Serial.print("Falha ao conectar ao Broker MQTT, rc=");
      Serial.print(client.state());
      Serial.println(". Tentando conectar-se novamente em 5 segundos");
      delay(5000);
    }
  }
}

/**
 * Inicia a aplicação.
*/
void setup() {
  pinMode(pin_valvula_irrigacao, OUTPUT);
  pinMode(pin_sensor_umidade, INPUT);
  digitalWrite(pin_valvula_irrigacao, HIGH); // Iniciado em HIGH devido ao tipo de relé utilizado.

  Serial.begin(115200);

  setupWiFi();
  setupMQTT();
  conectarMQTT();
}

/**
 * Loop da aplicação.
*/
void loop() {
  if ((millis() - tempo_sensor) >= intervalo_sensor) {
    capturarUmidade();
    publicarUmidade();
    tempo_sensor = millis();
  }
  if (umidade_atual < umidade_minima || irrigacao_manual) {
    fazerIrrigacao();
  }

  delay(100);

  if (!client.connected()) {
    conectarMQTT();
  }
  client.loop();
}

/**
 * 
 * CÓDIGO PARA ESCANEAMENTO DE REDES
 * Esse código não faz parte do projeto. Entretanto pode ser utilizado para verificar os ssids das redes que
 * o NodeMCU consegue se conectar.
 * As vezes um ssid pode conter caracteres ocultos, como espaços, o que pode ser verificado com esse código.
 * 
*/

// #include "ESP8266WiFi.h"

// void setup() {
//   Serial.begin(115200);
//   WiFi.mode(WIFI_STA);
//   WiFi.disconnect();
//   delay(2000);
//   Serial.println("Setup done");
// }

// void loop() {
//   Serial.println("scan start");


//   int n = WiFi.scanNetworks(); // WiFi.scanNetworks will return the number of networks found
//   Serial.println("scan done");
//   if (n == 0)
//     Serial.println("no networks found");
//   else
//   {
//     Serial.print(n);
//     Serial.println(" networks found");
//     for (int i = 0; i < n; ++i)
//     {
//       Serial.print(i + 1);
//       Serial.print(": ");
//       Serial.print(WiFi.SSID(i));
//       Serial.print(" (");
//       Serial.print(WiFi.RSSI(i));
//       Serial.print(")");
//       Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
//       delay(10);
//     }
//   }
//   Serial.println("");

//   delay(5000);
// }