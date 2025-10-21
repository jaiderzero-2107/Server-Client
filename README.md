# **Comunicação Cliente-Servidor com Sockets**

Este repositorio contem um programa cliente-servidor desenvolvida em C para a plataforma Windows, utilizando a API Winsock. O objetivo do projeto é demonstrar a troca de mensagens entre processos distintos, 
onde um servidor interpreta e responde a comandos enviados por um cliente.

## Implementação

**Servidor (server.c)**

Inicialização: O servidor começa inicializando a biblioteca Winsock (WSAStartup), exclusiva para operações de rede no Windows.

Criação do Socket: Um socket TCP (socket) é criado para escutar por conexões.

Bind: O socket é vinculado a um endereço IP local (neste caso, INADDR_ANY para aceitar conexões de qualquer interface de rede) e a uma porta específica (8080).

<img width="866" height="668" alt="Sem título1" src="https://github.com/user-attachments/assets/41d56151-958b-4dbf-b6e6-8353861e5b83" />

Listen: O servidor entra em estado de escuta, pronto para aceitar novas conexões de clientes. Ao mesmo tempo, se registra um log com o status atual.

<img width="1016" height="273" alt="Capturar1" src="https://github.com/user-attachments/assets/4911f520-87ac-4c24-bd01-5e02c80387a7" />

Loop principal de conexão: Após configurar, o servidor entra em um loop infinito, onde a função accept() aguarda (bloqueia a execução) até que um cliente se conecte.

Função do Cliente: Ao aceitar uma conexão, a lógica de tratamento é delegada a uma função handle_client. Dentro desta função, um segundo loop aguarda o recebimento de mensagens (recv) do cliente conectado e cada mensagem recebida 
é processada: o comando é identificado (usando strcmp e strncmp) e uma resposta é enviada de volta ao cliente (send). Se o comando for EXIT, a resposta é enviada e o loop de tratamento é encerrado, finalizando a conexão com aquele cliente.
<img width="906" height="773" alt="Sem título3" src="https://github.com/user-attachments/assets/eeaa770a-7bc5-47a5-9288-e4665d161dfa" />
<img width="848" height="514" alt="Sem título2" src="https://github.com/user-attachments/assets/53edb563-9926-4395-ad85-0bdb3299e4f1" />

Log: Todas as ações importantes (inicialização, conexão de cliente, comandos recebidos, respostas enviadas e desconexões) são registradas em um arquivo server_log.txt e exibidas no console.

**Cliente (client.c)**

Inicialização: Assim como o servidor, o cliente inicializa a biblioteca Winsock.

Criação do Socket: Um socket TCP é criado para a comunicação.
<img width="779" height="745" alt="Sem título4" src="https://github.com/user-attachments/assets/cb8f44a0-b43c-42a2-9063-fd333a837e7a" />

Connect: O cliente tenta estabelecer uma conexão (connect) com o endereço IP e a porta do servidor.

<img width="1024" height="335" alt="Sem título5" src="https://github.com/user-attachments/assets/b559f4ee-b7ed-49ed-a1ae-3cbf7bbac028" />

Interação: Após a conexão, o cliente entra em um loop onde o usuário é solicitado a digitar um comando. Esse mesmo comando é lido da entrada padrão (fgets) e uma mensagem é enviada ao servidor (send). Seguinte aguarda (recv) 
pela resposta do servidor e a exibe na tela onde se o comando enviado for EXIT, o loop é encerrado após receber a confirmação do servidor.
<img width="861" height="812" alt="Sem título6" src="https://github.com/user-attachments/assets/0ed961f9-2875-4b65-9ac0-9ba60e4b5233" />

Encerramento: A conexão é fechada (closesocket) e os recursos da Winsock são liberados (WSACleanup).

O protocolo TCP/IP usado é síncrono e de requisição-resposta: o cliente envia um comando e aguarda uma resposta do servidor antes de enviar o próximo, simplificando a lógica de controle de fluxo em ambos os lados.

Screenshots:

<img width="782" height="205" alt="Sem título7" src="https://github.com/user-attachments/assets/88fe309f-667b-4149-8e5b-61d29e3d6776" />
<img width="760" height="161" alt="Sem título8" src="https://github.com/user-attachments/assets/bbb986ee-d3f2-4493-bec5-0af29d130869" />
<img width="569" height="399" alt="Sem título9" src="https://github.com/user-attachments/assets/664e8be2-2b42-4303-88c6-e3cbb19de80a" />

## Instruções como executar

Em CLion o Cmake está configurado para poder executar ambos client.c e server.c na mesma pasta; primeramente tem que ser executado o server.c e depois o client.c. Ao inserir os comandos na tela do client.c, eles
terão efeito no server.c.


https://www.youtube.com/shorts/R6WN4_bBB1Q
