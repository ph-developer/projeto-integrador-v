#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "./config.cpp"

WiFiClient wifiClient; // Cliente WiFi.
PubSubClient client(wifiClient); // Cliente MQTT.
#define MSG_BUFFER_SIZE (50) // Tamanho máximo do buffer do Cliente MQTT.
char msg[MSG_BUFFER_SIZE]; // Variável para armazenamento da mensagem recebida do Broker MQTT.

float umidade_atual; // Variável para armazenamento do percentual de umidade atual.
bool irrigacao_manual = false; // Variável para armazenamento do status de irrigação manual.
unsigned long tempo_sensor = millis(); // Variável para armazenamento da última verificação de umidade.

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
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callbackMQTT);
}

/**
 * Realiza a conexão ao Broker MQTT.
*/
void conectarMQTT() {
  while (!client.connected()) {
    Serial.print("Aguardando conexão MQTT...");
    
    String clientId = "PROJETO_INTEGRADOR_V" + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqtt_usuario, mqtt_senha)) {
      Serial.println("MQTT conectado");
      client.subscribe(mqtt_topico_irrigacao_manual);
    } else {
      Serial.print("Falha ao conectar ao MQTT, rc=");
      Serial.print(client.state());
      Serial.println("Tentando conectar novamente em 5 segundos");
      delay(5000);
    }
  }
}

/**
 * Lida com as mensagens recebidas do Broker MQTT.
*/
void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  callbackIrrigacaoManual(topic, payload, length);
}

/**
 * Lida com as mensagens recebidas no tópico de Irrigação Manual do Broker MQTT.
*/
void callbackIrrigacaoManual(char* topic, byte* payload, unsigned int length) {
  if (topic != mqtt_topico_irrigacao_manual) return;

  char* message = "";
  for (int i = 0; i < length; i++) {
    message += (char) payload[i];
  }

  if (message != "true") return;

  Serial.println("Comando de irrigação manual recebido");
  irrigacao_manual = true;
}

/**
 * Inicia o processo de irrigação.
*/
void fazerIrrigacao() {
  digitalWrite(pin_valvula_irrigacao, HIGH);
  publicarIrrigacao(true);

  delay(tempo_irrigacao);

  digitalWrite(pin_valvula_irrigacao, LOW);
  publicarIrrigacao(false);

  irrigacao_manual = false;
}

/**
 * Envia o status do processo de irrigação ao Broker MQTT.
*/
void publicarIrrigacao(bool status) {
  Serial.print("Status da irrigação: ");
  Serial.print(status ? "ligada" : "desligada");
  sprintf(msg, "%f", status);
  client.publish(mqtt_topico_irrigacao_status, msg, true);
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
 * Inicia a aplicação.
*/
void setup() {
  pinMode(pin_valvula_irrigacao, OUTPUT);
  pinMode(pin_sensor_umidade, INPUT);

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