#ifndef HTTP_H
#define HTTP_H

#include <string>

// Generar respuesta a la request
std::string generateResponse(const std::string& status,
                             const std::string& mimeType,
                             const std::string& content, bool redirect,
                             const std::string& redirectURL);

// Obtener MIME
std::string getMimeType(const std::string& filePath);

// Enviar respuesta
void sendResponse(std::string path, std::string code, int sock, bool redirect,
                  std::string redirectURL);

// Enviar archivo solicitado
void sendFile(std::string filePath, int sock);

// Obtener metodo de la request
std::string method(std::string request);

// Obtener ruta de la request
std::string path(std::string request);

// Obtener version HTTP de la request
std::string HTTPVersion(std::string request);

// Verificar metodo de la request
bool checkMethod(std::string, int sock);

// Verificar HTTP de la request
bool checkHTTP(std::string versionString, int sock);

// Parsear la ruta
std::string fileRoute(std::string path, int sock);

#endif