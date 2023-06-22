#include <iostream>
#include <sstream>
#include <string>
#include "tcp.h"
#define SIZE 32000

int main() {
  std::string request;
  printf("Ingrese la request: ");
  getline(std::cin, request);

  char requestBuffer[SIZE];
  request.copy(requestBuffer, SIZE);

  struct tcp_client_t server;
  tcp_client_connect(&server, "localhost", 10000);

  tcp_send(server.sock, requestBuffer, SIZE);

  char response[SIZE];
  tcp_recv(server.sock, response, SIZE);

  return 0;
}