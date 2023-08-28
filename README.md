# Projeto Integrador V - Aplicação PWA

Aplicação Web/Mobile (PWA) de irrigação automatizada, para atendimento ao Projeto Integrador em Computação V (UNIVESP) -
Turma 001 - Grupo 004.

<!--  -->

## Funcionamento da Aplicação

Através da aplicação PWA o usuário verifica a umidade atual e o estado da irrigação, ouvindo os tópicos do Broker MQTT.

É possível através da aplicação acionar a irrigação, a qual será solicitada ao Broker MQTT.

<!--  -->

## Sobre a Aplicação

Todo o código da aplicação PWA está nesta pasta. As capturas de tela da aplicação encontram-se na pasta *docs/prints*.

A aplicação está hospedada através do Firebase em https://irrigacao-pi-v.web.app.

A aplicação foi desenvolvida em *Typescript + Vue*, através do Framework *Quasar*.

O usuário pode acessar as informações do sistema utilizando-se de usuário e senha, os quais, para o intuito de testes ou
demonstração do sistema, foi disponibilizado o usuário *teste* com senha *teste*.

O usuário tem a possibilidade de cadastrar, editar e/ou excluir os sensores cadastrados em seu perfil, sendo que cada
usuário possui seus próprios sensores cadastrados.

<!--  -->

## Recursos Utilizados

- *Typescript*: Linguagem de programação de código aberto desenvolvida pela Microsoft. É um superconjunto sintático
estrito de JavaScript e adiciona tipagem estática opcional à linguagem. (https://www.typescriptlang.org)
- *VueJs*: Framework JavaScript de código-aberto, focado no desenvolvimento de interfaces de usuário e aplicativos de
página única. (https://vuejs.org)
- *QuasarJs*: Framework para desenvolvimento híbrido com um incrível suporte a criação de aplicações para múltiplas
plataformas, baseado em VueJs. (https://quasar.dev)
- *PWA* ou *Progressive Web App*: Aplicação web desenvolvida usando tecnologias específicas para aproveitar recursos de
aplicativos nativos e da web. É como um site feito usando tecnologias da web, mas que parece um app. Pode ser instalado
na plataforma android como um aplicativo, através do chrome.
- *Firebase*: Plataforma BaaS. Conjunto de serviços de computação em nuvem de back-end e plataformas de desenvolvimento
de aplicativos fornecidos pelo Google. Ele hospeda bancos de dados, serviços, autenticação e integração para uma
variedade de aplicativos. Neste projeto, o PWA foi hospedado no Firebase. (https://firebase.google.com/?hl=pt)

<!--  -->

## Broker MQTT

Foi utilizado o Broker MQTT HiveMQ pela sua gratuidade. (https://www.hivemq.com)

Para testes, foi utilizado o host *97299651548a4c57a3aabb6700e6e882.s2.eu.hivemq.cloud*, porta *8883* (mqtts, podendo
ser utilizada a porta *8884* para wss), sob usuário *projeto-integrador-v* e senha *univesp001004*.

Tópicos utilizados para testes:
- */pi_v/umidade_atual*: tópico em que a aplicação escuta o percentual de umidade do solo. (valor inteiro)
- */pi_v/status_irrigacao*: tópico em que a aplicação escuta o estado atual da irrigação. (valor boolean)
- */pi_v/acionar_irrigacao*: tópico em que a aplicação PWA envia uma solicitação de irrigação. (valor boolean)

<!--  -->

## Implementações Pendentes

- [x] Implementar um sistema de autenticação baseado em usuário e senha, removendo a forma de autenticação atual (token
de acesso).
- [ ] Implementar adição, remoção e edição dos sensores.

<!--  -->

## Referencias Bibliográficas

- VUE.JS 3.0. Documentação | Introdução. Disponível em: https://pt.vuejs.org/guide/introduction.html. Acesso em: 20 ago. 2023.
- QUASAR Framework 2.0. Docs. Disponível em: https://quasar.dev/docs. Acesso em: 10 ago. 2023.
- EMA. WebSocket connection failed SOLUTION. ln: HiveMQ. Discuss everything about the HiveMQ Community Edition. [S.l.]. 9 abr. 2023. Disponível em: https://community.hivemq.com/t/websocket-connection-failed-solution/1964. Acesso em: 10 ago. 2023.
- KEDIR, Rihanna. How to quickly deploy your Progressive Web Apps using Firebase. ln: Medium. Medium. [S.l.]. 12 mai. 2019. Disponível em: https://medium.com/@rihanna.ke/how-to-quickly-deploy-your-progressive-web-apps-using-firebase-14a0c09e9a11. Acesso em: 20 ago. 2023.
