#include <arpa/inet.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <cstring>
#include <iostream>
#include "http.h"
#include "tcp.h"
#define SIZE 32000

int main() {

  SSL_CTX* ssl_ctx = SSL_CTX_new(SSLv23_server_method());
  configureSSL(ssl_ctx);

  struct tcp_server_t server;
  tcp_server_create(&server, 10000);

  while (true) {
    std::cout << "Escuchando conexiones entrantes ..." << std::endl;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Socket para esperar las conexiones
    int sock = tcp_server_accept(&server, &client_addr, &client_addr_len);
    printf("Cliente conectado con direccion IP: %s\n\n",
           inet_ntoa(client_addr.sin_addr));

    SSL* ssl = SSL_new(ssl_ctx);
    SSL_set_fd(ssl, sock);
    if (SSL_accept(ssl) <= 0) {
      ERR_print_errors_fp(stderr);
      SSL_shutdown(ssl);
      SSL_free(ssl);
      tcp_close(sock);
      continue;
    }

    // Solicitud del cliente
    char requestBuffer[SIZE];
    memset(requestBuffer, 0, sizeof(requestBuffer));
    ssl_recv(ssl, requestBuffer, sizeof(requestBuffer));

    // Parsear la request
    std::string request = requestBuffer;
    std::cout << "Request solicitada:" << request << std::endl;

    // Obtener method, path y version HTTP
    std::string methodString = method(request);
    std::string pathString = path(request);
    std::string versionString = HTTPVersion(request);

    std::cout << methodString << " " << pathString << " " << versionString
              << std::endl;
    // Verificar metodo
    if (!checkMethod(methodString, ssl)) {
      SSL_shutdown(ssl);
      SSL_free(ssl);
      tcp_close(sock);
      continue;
    }

    // Verificar version HTTP
    if (!checkHTTP(versionString, ssl)) {
      SSL_shutdown(ssl);
      SSL_free(ssl);
      tcp_close(sock);
      continue;
    }

    // Obtener la ruta final del archivo
    std::string filePath = fileRoute(pathString, ssl);
    if (filePath == "301" || filePath == "400") {
      SSL_shutdown(ssl);
      SSL_free(ssl);
      tcp_close(sock);
      continue;
    }

    // Enviar el archivo
    sendFile(filePath, ssl);

    // Cerrar conexion
    SSL_shutdown(ssl);
    SSL_free(ssl);
    tcp_close(sock);
  }
}