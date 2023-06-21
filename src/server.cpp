#include "tcp.h"
#include "http.h"
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

#define SIZE 1024

int main(){
    struct tcp_server_t server;
    tcp_server_create(&server, 10000);

    while(true){
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        // Socket para esperar las conexiones
        int sock = tcp_server_accept(&server, &client_addr, &client_addr_len);
        printf("Cliente conectado con direccion IP: %s\n\n",inet_ntoa(client_addr.sin_addr));

        // Solicitud del cliente
        char requestBuffer[SIZE];
        memset(requestBuffer,0,sizeof(requestBuffer));
        tcp_recv(sock,requestBuffer,sizeof(requestBuffer));
;
        // Parsear la request
        std::string request = requestBuffer;

        // Obtener method, path y version HTTP
        std::string methodString = method(request);
        std::string pathString = path(request);
        std::string versionString = HTTPVersion(request);

        // Obtener la ruta final del archivo
        std::string filePath = fileRoute(pathString);
        std::cout << filePath << std::endl;

        // Enviar el archivo
        std::string content;
        std::ifstream file(filePath);
        std::string responseContent;

        if (file) {
            std::ostringstream contentStream;
            contentStream << file.rdbuf();
            responseContent = contentStream.str();
            std::string mimeType = getMimeType(filePath);
            std::string response = generateResponse("200 OK", mimeType, responseContent);
            send(sock, response.c_str(), response.length(), 0);
            std::cout << response << std::endl;
        } else {
            std::string response = generateResponse("404 Not Found", "text/plain", "404 Not Found");
            send(sock, response.c_str(), response.length(), 0);
            std::cout << response << std::endl;
        }
        
        // Cerrar conexion
        tcp_close(sock);
        
        std::cout <<"\nEscuchando conexiones entrantes ..." << std::endl;

    }
}