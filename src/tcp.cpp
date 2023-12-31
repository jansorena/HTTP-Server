#include "tcp.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

void tcp_server_create(struct tcp_server_t* server, int port) {
  // Crear socket de escucha (se guarda en server->listen_sock)
  server->listen_sock = socket(AF_INET, SOCK_STREAM,
                               0);  // parametros: ipv4,socket tco, api layer 0
  if (server->listen_sock < 0) {
    std::cerr << "No se pudo crear el socket." << std::endl;
    exit(1);
  } else {
    std::cout << "TCP server socket creado correctamente." << std::endl;
  }

  memset(&server->server_addr, 0,
         sizeof(server->server_addr));  // inicializar la direccion del server
  server->server_addr.sin_family = AF_INET;    // usar ipv4
  server->server_addr.sin_port = htons(port);  // puerto definido (10000)
  server->server_addr.sin_addr.s_addr =
      htonl(INADDR_ANY);  // acepta conexiones de cualquier IP

  // Bindear socket a puerto
  if (bind(server->listen_sock, (struct sockaddr*)&server->server_addr,
           sizeof(server->server_addr)) < 0) {
    std::cerr << "Error al bindear el socker a puerto." << std::endl;
    exit(1);
  }

  // Escuchar conexiones entrantes
  int wait_size = 16;  // maximo numero de clientes esperando
  if (listen(server->listen_sock, wait_size) < 0) {
    std::cerr << "No se puede escuchar conexiones entrantes." << std::endl;
    exit(1);
  } else {
    std::cout << "Servidor HTTP iniciado en el puerto " << port << std::endl;
    std::cout << std::endl;
  }
}

int tcp_server_accept(struct tcp_server_t* server,
                      struct sockaddr_in* client_addr,
                      socklen_t* client_addr_len) {
  // Aceptar primera conexion entrante (guardar dirección del cliente en client_addr)
  int client_sock = accept(server->listen_sock, (struct sockaddr*)client_addr,
                           client_addr_len);
  // Retorna descriptor de archivo del socket de comunicación con el cliente
  if (client_sock < 0) {
    std::cerr << "No se puede abrir un socket." << std::endl;
    exit(1);
  } else {
    return client_sock;
  }
}

void tcp_client_connect(struct tcp_client_t* client, const char* host,
                        int port) {
  // Crear socket de cliente (se guarda en client->sock)
  client->sock = socket(PF_INET, SOCK_STREAM, 0);
  if (client->sock < 0) {
    std::cerr << "No se pudo crear el socket." << std::endl;
    exit(1);
  }
  // Conectar con host y puerto indicados (se guarda en client->server_addr y
  // se usa en llamada a connect())
  memset(&client->server_addr, 0, sizeof(client->server_addr));
  client->server_addr.sin_family = AF_INET;
  inet_pton(AF_INET, host, &client->server_addr.sin_addr);
  client->server_addr.sin_port = htons(port);

  if (connect(client->sock, (struct sockaddr*)&client->server_addr,
              sizeof(client->server_addr)) < 0) {
    std::cerr << "No se pudo conectar al servidor." << std::endl;
    exit(1);
  }
}

void tcp_send(int sock, const void* data, size_t size) {
  if (send(sock, data, size, 0) < 0) {
    std::cerr << "No se puede enviar el mensaje." << std::endl;
    exit(1);
  }
}

void tcp_recv(int sock, void* data, size_t size) {
  if (recv(sock, data, size, 0) < 0) {
    std::cerr << "No se puede recibir el mensaje" << std::endl;
    exit(1);
  }
}

void tcp_close(int sock) {
  close(sock);
  std::cout << "\nConexion cerrada." << std::endl;
}

void ssl_send(SSL* ssl, const void* data, size_t size) {
  if (SSL_write(ssl, data, size) < 0) {
    std::cerr << "No se puede enviar el mensaje." << std::endl;
    exit(1);
  }
}

void ssl_recv(SSL* ssl, void* data, size_t size) {
  if (SSL_read(ssl, data, size) < 0) {
    std::cerr << "No se puede recibir el mensaje" << std::endl;
    exit(1);
  }
}

void configureSSL(SSL_CTX* ssl_ctx) {
  SSL_CTX_use_certificate_file(ssl_ctx, "../ssl/certificate.crt",
                               SSL_FILETYPE_PEM);
  SSL_CTX_use_PrivateKey_file(ssl_ctx, "../ssl/private.key", SSL_FILETYPE_PEM);
}