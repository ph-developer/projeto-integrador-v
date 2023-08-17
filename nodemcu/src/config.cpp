/**
 * Arquivo de configurações do sistema de irrigação.
*/

// Pinagem
#define pin_sensor_umidade A0
#define pin_valvula_irrigacao D1

// WiFi
const char* wifi_ssid = "Casa"; // ID da rede WiFi.
const char* wifi_pass = "974010516"; // Senha da rede WiFi.

// Broker MQTT
const char* mqtt_server = "97299651548a4c57a3aabb6700e6e882.s2.eu.hivemq.cloud"; // IP do Broker MQTT.
const int mqtt_port = 8883; // Porta do Broker MQTT.
const char* mqtt_usuario = "projeto-integrador-v";
const char* mqtt_senha = "univesp001004";
const char* mqtt_topico_irrigacao_manual = "pi_v/irrigacao_manual"; // Tópico de recebimento de irrigação manual.
const char* mqtt_topico_irrigacao_status = "pi_v/status_irrigacao"; // Tópico de envio do status da irrigação.
const char* mqtt_topico_umidade_atual = "pi_v/umidade_atual"; // Tópico de envio do percentual de umidade atual.

// Funcionalidades 
const int intervalo_sensor = 3000; // Intervalo entre as verificações de umidade. (em milisegundos)
const int tempo_irrigacao = 3000; // Tempo de irrigação. (em milisegundos)
const int umidade_minima = 40; // Percentual de umidade mínimo para acionar a irrigação. (em porcentagem)