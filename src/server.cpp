#include "tcp.h"
#include "http.h"
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#define SIZE 32000

int main(){
    struct tcp_server_t server;
    tcp_server_create(&server, 10000);

    while(true){
        std::cout <<"Escuchando conexiones entrantes ..." << std::endl;
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        // Socket para esperar las conexiones
        int sock = tcp_server_accept(&server, &client_addr, &client_addr_len);
        printf("Cliente conectado con direccion IP: %s\n\n",inet_ntoa(client_addr.sin_addr));

        // Solicitud del cliente
        char requestBuffer[SIZE];
        memset(requestBuffer,0,sizeof(requestBuffer));
        tcp_recv(sock,requestBuffer,sizeof(requestBuffer));

        // Parsear la request
        std::string request = requestBuffer;
        std::cout << "Request solicitada:" << request << std::endl;

        // Obtener method, path y version HTTP
        std::string methodString = method(request);
        std::string pathString = path(request);
        std::string versionString = HTTPVersion(request);

        // Verificar metodo
        if(!checkMethod(methodString,sock)){
            tcp_close(sock);
            continue;
        }

        // Verificar version HTTP
        if(!checkHTTP(versionString,sock)){
            tcp_close(sock);
            continue;
        }

        // Obtener la ruta final del archivo
        std::string filePath = fileRoute(pathString,sock);
        if(filePath == "301" || filePath == "400"){
            tcp_close(sock);
            continue;
        }

        // Enviar el archivo
        sendFile(filePath,sock);
        
        // Cerrar conexion
        tcp_close(sock);
    }
}