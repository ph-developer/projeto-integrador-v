#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

// Pinagem
#define pin_humidity_sensor A0 // Pino do sensor de umidade.
#define pin_irrigation_relay D1 // Pino do relé da válvula de irrigação.

// WiFi
const char* wifi_ssid = "*****"; // ID da rede WiFi.
const char* wifi_password = "*****"; // Senha da rede WiFi.

// Broker MQTT
const char* mqtt_server = "97299651548a4c57a3aabb6700e6e882.s2.eu.hivemq.cloud"; // IP do Broker MQTT.
const int mqtt_port = 8883; // Porta do Broker MQTT.
const char* mqtt_username = "projeto-integrador-v";
const char* mqtt_password = "univesp001004";
const char* mqtt_topic_trigger_irrigation = "pi_v/acionar_irrigacao"; // Tópico de acionamento da irrigação.
const char* mqtt_topic_irrigation_status = "pi_v/status_irrigacao"; // Tópico de envio do status da irrigação.
const char* mqtt_topic_current_humidity = "pi_v/umidade_atual"; // Tópico de envio do percentual de umidade atual.

// Funcionalidades
const int sensor_check_interval = 5000; // Intervalo entre as verificações de umidade. (ms)
const int irrigation_timeout = 10000; // Tempo de irrigação. (ms)
const int min_allowed_humidity = 70; // Percentual de umidade mínimo para acionar a irrigação. (%)

// Variáveis
WiFiClientSecure wifiClient; // Cliente WiFi.
PubSubClient client(wifiClient); // Cliente MQTT.
#define MSG_BUFFER_SIZE (50) // Tamanho máximo do buffer do Cliente MQTT.
char msg[MSG_BUFFER_SIZE]; // Variável para armazenamento da mensagem recebida do Broker MQTT.

float current_humidity; // Variável para armazenamento do percentual de umidade atual.
bool trigger_irrigation = false; // Variável para armazenamento do status de acionamento da irrigação.
unsigned long last_sensor_check = millis(); // Variável para armazenamento da última verificação de umidade.

/**
 * Lida com as mensagens recebidas no tópico de Irrigação Manual do Broker MQTT.
*/
void callbackTriggerIrrigation(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, mqtt_topic_trigger_irrigation) == -1) return;

  char* message = (char*)"";
  for (unsigned int i = 0; i < length; i++) {
    message += (char) payload[i];
  }

  if (strcmp(message, "true") == -1) return;

  Serial.println("Comando de irrigação manual recebido");
  trigger_irrigation = true;
}

/**
 * Lida com as mensagens recebidas do Broker MQTT.
*/
void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  callbackTriggerIrrigation(topic, payload, length);
}

/**
 * Envia o status do processo de irrigação ao Broker MQTT.
*/
void publishIrrigationStatus(bool status) {
  Serial.print("Status da irrigação: ");
  Serial.println(status == true ? "ligada" : "desligada");
  sprintf(msg, "%s", status == true ? "true" : "false");
  client.publish(mqtt_topic_irrigation_status, msg, true);
}

/**
 * Inicia o processo de irrigação.
*/
void triggerIrrigation() {
  digitalWrite(pin_irrigation_relay, LOW); // Invertido devido ao tipo de relé utilizado.
  publishIrrigationStatus(true);

  delay(irrigation_timeout);

  digitalWrite(pin_irrigation_relay, HIGH); // Invertido devido ao tipo de relé utilizado.
  publishIrrigationStatus(false);

  trigger_irrigation = false;
}

/**
 * Captura a umidade atual do sensor e armazena o percentual em uma variável global.
*/
void checkHumidity() {
  float humidity = analogRead(pin_humidity_sensor);
  current_humidity = map(humidity, 0, 1023, 100, 0);
}

/**
 * Envia o percentual de umidade atual ao Broket MQTT.
*/
void publishCurrentHumidity() {
  Serial.print("Percentual de umidade: ");
  Serial.print(current_humidity);
  Serial.println(F("%"));
  sprintf(msg, "%f", current_humidity);
  client.publish(mqtt_topic_current_humidity, msg, true);
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
  WiFi.begin(wifi_ssid, wifi_password);

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
void connectMQTT() {
  while (!client.connected()) {
    Serial.println("Conectando ao Broker MQTT...");

    String clientId = "NodeMCU_" + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Broker MQTT conectado");
      client.subscribe(mqtt_topic_trigger_irrigation);
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
  pinMode(pin_irrigation_relay, OUTPUT);
  pinMode(pin_humidity_sensor, INPUT);
  digitalWrite(pin_irrigation_relay, HIGH); // Iniciado em HIGH devido ao tipo de relé utilizado.

  Serial.begin(115200);

  setupWiFi();
  setupMQTT();
  connectMQTT();
}

/**
 * Loop da aplicação.
*/
void loop() {
  if ((millis() - last_sensor_check) >= sensor_check_interval) {
    checkHumidity();
    publishCurrentHumidity();
    last_sensor_check = millis();
  }
  if (current_humidity < min_allowed_humidity || trigger_irrigation) {
    triggerIrrigation();
  }

  delay(100);

  if (!client.connected()) {
    connectMQTT();
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
