# Projeto Integrador V - Aplicação PWA

Aplicação Web/Mobile (PWA) de irrigação automatizada, para atendimento ao Projeto Integrador em Computação V (UNIVESP) - Turma 001 - Grupo 004.

<!--  -->

## Funcionamento da Aplicação

Através da aplicação PWA o usuário verifica a umidade atual e o status da irrigação, ouvindos os tópicos do Broker MQTT.

É possível através da aplicação iniciar uma irrigação manual, a qual será solicitada ao Broker MQTT.

<!--  -->

## Sobre a Aplicação

Todo o código da aplicação PWA está nesta pasta. Os prints da aplicação encontram-se na pasta *docs/prints*.

A aplicação está hospedada através do Firebase em https://irrigacao-pi-v.web.app/. Lembrando que, por motivos de segurança, essa aplicação hospedada é apenas para teste e/ou demonstração do sistema, e no caso de uma implementação para produção, a aplicação será hospedada em um link específico para cada usuário.

A aplicação foi desenvolvida em *Typescript*, através do Framework *Quasar*.

O usuário pode acessar as informaçoes do sistema utilizando um *token de acesso*, o qual, para o intuito de testes ou demonstração do sistema, foi disponibilizado o *token de acesso 001004* para acesso ao *Sensor 001004*. Lembrando que, por motivos de segurança, esse *token de acesso* disponibilizado é apenas para teste e/ou demontração, e no caso de uma implementação para produção, serão disponibilizados *tokens de acesso* específicos para cada usuário, em cada aplicação.

Na necessidade de testes ou demonstração do sistema, deve-se utilizar o *token de acesso* citado anteriormente, e para *mockar* o envio e recebimento de informações, pode-se enviar e/ou escutar os tópicos citados abaixo em *Broker MQTT*.

<!--  -->

## Recursos Utilizados

- *Typescript*: Linguagem de programação de código aberto desenvolvida pela Microsoft. É um superconjunto sintático estrito de JavaScript e adiciona tipagem estática opcional à linguagem. (https://www.typescriptlang.org/)
- *VueJs*: Framework JavaScript de código-aberto, focado no desenvolvimento de interfaces de usuário e aplicativos de página única. (https://vuejs.org/)
- *QuasarJs*: Framework para desenvolvimento híbrido com um incrível suporte a criação de aplicações para multiplas plataformas, baseado em VueJs. (https://quasar.dev/)
- *PWA* ou *Progressive Web App*: Aplicação web desenvolvida usando tecnologias específicas para aproveitar recursos de apps nativos e da web. É como um site feito usando tecnologias da web, mas que parece um app. Pode ser instalado na plataforma android como um app, através do chrome.
- *Firebase*: Plataforma BaaS. Conjunto de serviços de computação em nuvem de back-end e plataformas de desenvolvimento de aplicativos fornecidos pelo Google. Ele hospeda bancos de dados, serviços, autenticação e integração para uma variedade de aplicativos. Neste projeto, o PWA foi hospedado no Firebase. (https://firebase.google.com/?hl=pt)

<!--  -->

## Broker MQTT

Foi utilizado o Broker MQTT HiveMQ pela sua gratuidade. (https://www.hivemq.com/)

Para testes, foi utilizado o seguinte host *97299651548a4c57a3aabb6700e6e882.s2.eu.hivemq.cloud*, porta *8883* (mqtts, podendo ser utilizada a porta *8884* para wss), sob usuário *projeto-integrador-v* e senha *univesp001004*.

Tópicos utilizados:
- */pi_v/umidade_atual*: Tópico em que a aplicação escuta o percentual de umidade do solo. (valor inteiro)
- */pi_v/status_irrigacao*: Tópico em que a aplicação escuta o status da irrigação. (valor booleano)
- */pi_v/irrigacao_manual*: Tópico em que a aplicação PWA envia uma solicitação de irrigação. (valor booleano)

<!--  -->

## Implementações Pendentes

- [ ] Implementar um sistema de autenticação baseado em usuário e senha, removendo a forma de autenticação atual (token de acesso).
- [ ] Implementar um painel admin para configuração dos usuários e conexões com os dispositivos IoT.

<!--  -->

## Referencias Bibliográficas

- VUE.JS 3.0. Documentação | Introdução. Disponível em: https://pt.vuejs.org/guide/introduction.html. Acesso em: 20 ago. 2023.
- QUASAR Framework 2.0. Docs. Disponível em: https://quasar.dev/docs. Acesso em: 10 ago. 2023.
- EMA. WebSocket connection failed SOLUTION. ln: HiveMQ. Discuss everything about the HiveMQ Community Edition. [S.l.]. 9 abr. 2023. Disponível em: https://community.hivemq.com/t/websocket-connection-failed-solution/1964. Acesso em: 10 ago. 2023.
- KEDIR, Rihanna. How to quickly deploy your Progressive Web Apps using Firebase. ln: Medium. Medium. [S.l.]. 12 mai. 2019. Disponível em: https://medium.com/@rihanna.ke/how-to-quickly-deploy-your-progressive-web-apps-using-firebase-14a0c09e9a11. Acesso em: 20 ago. 2023.
