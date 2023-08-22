# Projeto Integrador V

Sistema para irrigação automatizada, composto por aplicação Web/Mobile (PWA) e Componentes IoT, para atendimento ao Projeto Integrador em Computação V (UNIVESP) - Turma 001 - Grupo 004.

<!--  -->

## Funcionamento do Sistema

A cada 5 segundos o NodeMCU verifica a umidade do solo atual e envia ao Broker MQTT.

Caso seja verificada uma umidade inferior a 70% o NodeMCU aciona a irrigação automática durante 10 segundos, enviando o status da irrigação ao Broker MQTT.

Através da aplicação PWA o usuário verifica a umidade atual e o status da irrigação. É possível atevés da aplicação iniciar uma irrigação manual, a qual ocorrerá durante 10 segundos, e o status será informado ao Broker MQTT.

<!--  -->

## Aplicação Web/Mobile (PWA)

Todo o código da aplicação PWA está na pasta *pwa*. Os prints da aplicação encontram-se na pasta *midia/pwa*.

A aplicação está hospedada através do Firebase em https://irrigacao-pi-v.web.app/. Lembrando que, por motivos de segurança, essa aplicação hospedada é apenas para teste e/ou demonstração do sistema, e no caso de uma implementação para produção, a aplicação será hospedada em um link específico para cada usuário.

A aplicação foi desenvolvida em *Typescript*, através do Framework *Quasar*.

O usuário pode acessar as informaçoes do sistema utilizando um *token de acesso*, o qual, para o intuito de testes ou demonstração do sistema, foi disponibilizado o *token de acesso 001004* para acesso ao *Sensor 001004*. Lembrando que, por motivos de segurança, esse *token de acesso* disponibilizado é apenas para teste e/ou demontração, e no caso de uma implementação para produção, serão disponibilizados *tokens de acesso* específicos para cada usuário, em cada aplicação.

Na necessidade de testes ou demonstração do sistema, deve-se utilizar o *token de acesso* citado anteriormente, e para *mockar* o envio e recebimento de informações, pode-se enviar e/ou escutar os tópicos citados abaixo em *Broker MQTT*.

Sobre os recursos utilizados:
- *Typescript*: Linguagem de programação de código aberto desenvolvida pela Microsoft. É um superconjunto sintático estrito de JavaScript e adiciona tipagem estática opcional à linguagem. (https://www.typescriptlang.org/)
- *VueJs*: Framework JavaScript de código-aberto, focado no desenvolvimento de interfaces de usuário e aplicativos de página única. (https://vuejs.org/)
- *QuasarJs*: Framework para desenvolvimento híbrido com um incrível suporte a criação de aplicações para multiplas plataformas, baseado em VueJs. (https://quasar.dev/)
- *PWA* ou *Progressive Web App*: Aplicação web desenvolvida usando tecnologias específicas para aproveitar recursos de apps nativos e da web. É como um site feito usando tecnologias da web, mas que parece um app. Pode ser instalado na plataforma android como um app, através do chrome.
- *Firebase*: Plataforma BaaS. Conjunto de serviços de computação em nuvem de back-end e plataformas de desenvolvimento de aplicativos fornecidos pelo Google. Ele hospeda bancos de dados, serviços, autenticação e integração para uma variedade de aplicativos. Neste projeto, o PWA foi hospedado no Firebase. (https://firebase.google.com/?hl=pt)

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

- VUE.JS 3.0. Documentação | Introdução. Disponível em: https://pt.vuejs.org/guide/introduction.html. Acesso em: 20 ago. 2023.
- QUASAR Framework 2.0. Docs. Disponível em: https://quasar.dev/docs. Acesso em: 10 ago. 2023.
- EMA. WebSocket connection failed SOLUTION. ln: HiveMQ. Discuss everything about the HiveMQ Community Edition. [S.l.]. 9 abr. 2023. Disponível em: https://community.hivemq.com/t/websocket-connection-failed-solution/1964. Acesso em: 10 ago. 2023.
- QUINTINO, Eduardo de Castro. Como conectar o ESP8266 à internet?. ln: MakerHero. Maker Hero. [S.l.]. 23 nov. 2020. Disponível em: https://www.makerhero.com/blog/como-conectar-o-esp8266-a-internet/. Acesso em: 18 ago. 2023.
- LOCATELLI, Caroline. Monitoramento e Controle por Aplicativo - MQTT. ln: Curto Circuito Componentes Eletrônicos Ltda ME. Curto Circuito. [S.l.]. 2 jun. 2020. Disponível em: https://curtocircuito.com.br/blog/Categoria%20IoT/monitoramento-e-controle-por-aplicativo-mqtt. Acesso em: 18 ago. 2023.
- VIRTUINO MQTT + HiveMQ. [S. l.:s. n.], 2021. 1 vídeo (11:45). Publicado pelo YouTube. Disponível em: https://www.youtube.com/watch?v=WGx06lPTNyQ. Acesso em: 19 ago. 2023.
- KEDIR, Rihanna. How to quickly deploy your Progressive Web Apps using Firebase. ln: Medium. Medium. [S.l.]. 12 mai. 2019. Disponível em: https://medium.com/@rihanna.ke/how-to-quickly-deploy-your-progressive-web-apps-using-firebase-14a0c09e9a11. Acesso em: 20 ago. 2023.
- COMMANDERFRANZ. Fritzing - a Tutorial. ln: Autodesk. Instructables. [S.l.]. Disponível em: https://www.instructables.com/Fritzing-A-Tutorial/. Acesso em: 20 ago. 2023.