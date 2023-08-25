# Sistema IoT de Exemplo

Exemplo básico de sistema para irrigação automatizada (parte de componentes IoT).

<!--  -->

## Funcionamento do Sistema

A cada 5 segundos o NodeMCU verifica a umidade do solo atual e envia ao Broker MQTT.

Caso seja verificada uma umidade inferior a 70% o NodeMCU aciona a irrigação automática durante 10 segundos, enviando o status da irrigação ao Broker MQTT.

Através da aplicação PWA o usuário verifica a umidade atual e o status da irrigação. É possível atevés da aplicação iniciar uma irrigação manual, a qual ocorrerá durante 10 segundos, e o status será informado ao Broker MQTT.

<!--  -->

## Broker MQTT

Para a comunicação entre a aplicação PWA e os Componentes IoT, foi utilizado o Broker MQTT HiveMQ, pela sua gratuidade. (https://www.hivemq.com/)

Para testes e/ou demontração, foi utilizado o seguinte host *97299651548a4c57a3aabb6700e6e882.s2.eu.hivemq.cloud*, porta *8883* (mqtts, podendo ser utilizada a porta *8884* para wss), sob usuário *projeto-integrador-v* e senha *univesp001004*.

Tópicos utilizados:
- */pi_v/umidade_atual*: Tópico em que o NodeMCU envia o percentual de umidade do solo, o qual é lido pela aplicação PWA.
- */pi_v/status_irrigacao*: Tópico em que o NodeMCU envia o status da irrigação, o qual é lido pela aplicação PWA.
- */pi_v/irrigacao_manual*: Tópico em que a aplicação PWA envia uma solicitação de irrigação, o quel é lido pelo NodeMCU.

<!--  -->

## Componentes IoT

O NodeMCU conecta-se a uma rede WiFi (a qual deve ser de 2.4GHz) e logo após conecta-se ao Broker MQTT. Após as conexões serem estabelecidas, o NodeMCU inicia o processo de análise dos dados de umidade do solo e execução da irrigação caso seja necessário.

Todo o código do NodeMCU encontra-se na pasta *nodemcu*. Destaco que o único arquivo que contém código desenvolvido para o projeto está em *nodemcu/src/main.cpp*.

As mídias do protótipo/implementação encontram-se em *midia/prototipo*.

Os diagramas elétricos encontram-se na pasta *diagramas*.

Na pasta *componentes* existe uma arquivo chamado *Custos do Projeto* o qual possui um print da lista de compras na Shopee dos componentes utilizados, com seus respectivos valores.

No caso de implementação, além da enventual alteração nas variáveis de configuração do *Broker MQTT*, deve-se configurar as variáveis do *wifi* (ssid e senha) no arquivo *main.cpp*. Lembrando que o NodeMCU conecta-se apenas em redes 2.4GHz.

Foram utilizados os seguintes componentes IoT:
- *NodeMCU v3*: Plataforma IoT de baixo custo e open-source, que utiliza o ESP8266 como processador. É o componente onde é feito o upload do código fonte e é o responsável por executar a lógica do sistema IoT.
- *Módulo Relé 5v 1 Canal*: Módulo para acionamento de cargas através de um microcontrolador. Apesar de o relé utilizado ser de 5v, este apresentou um bom funcionamento na tensão de 3.3v de saída do NodeMCU, assim, não foi preciso conectá-lo no 5v. Acredito que deve-se ao fato da carga utilizada ser pequena.
- *Sensor de Umidade de Solo*: Sensor que realiza a medição da umidade do solo através de uma sonda, e envia através de um sinal analógico ao microcontrolador.
- *Step-Down de 12v para 5v*: Módulo para redução da tensão de entrada de 12v para uma saída de 5v, uma vez que o NodeMCU opera em 5v.
- *Válvula Solenóide 12v*: Válvula para bloquear ou liberar a passagem da água.
- *Fonte 12v*: Fonte para alimentação do sistema.
- *Fios, Jumpers, Borne P4, e Mangueira*.

<!--  -->

## Referencias Bibliográficas

- QUINTINO, Eduardo de Castro. Como conectar o ESP8266 à internet?. ln: MakerHero. Maker Hero. [S.l.]. 23 nov. 2020. Disponível em: https://www.makerhero.com/blog/como-conectar-o-esp8266-a-internet/. Acesso em: 18 ago. 2023.
- LOCATELLI, Caroline. Monitoramento e Controle por Aplicativo - MQTT. ln: Curto Circuito Componentes Eletrônicos Ltda ME. Curto Circuito. [S.l.]. 2 jun. 2020. Disponível em: https://curtocircuito.com.br/blog/Categoria%20IoT/monitoramento-e-controle-por-aplicativo-mqtt. Acesso em: 18 ago. 2023.
- VIRTUINO MQTT + HiveMQ. [S. l.:s. n.], 2021. 1 vídeo (11:45). Publicado pelo YouTube. Disponível em: https://www.youtube.com/watch?v=WGx06lPTNyQ. Acesso em: 19 ago. 2023.
- COMMANDERFRANZ. Fritzing - a Tutorial. ln: Autodesk. Instructables. [S.l.]. Disponível em: https://www.instructables.com/Fritzing-A-Tutorial/. Acesso em: 20 ago. 2023.
